#include "dao.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

//定义一个文件来存储帐号
const char *ID_FILE = "id.dat";

//生成不重复的帐号
int generator_id()
{
	static int x = 100000;
	if(access(ID_FILE,F_OK) == -1){//判断文件是否存在，不存在返回-1,而且创建
		int fd = open(ID_FILE,O_WRONLY|O_CREAT|O_EXCL,0600);
		if(fd == -1){
			perror("文件打开失败\n");
			return -1;
		}
		write(fd,&x,sizeof(x));//写入文件
		close(fd);
		return x;
	}

	//如果文件存在
	int fd = open(ID_FILE,O_RDWR);
	if(fd == -1){
		perror("文件打开失败\n");
		return -1;
	}
	//将文件中的数值读到x中
	read(fd,&x,sizeof(x));
	x++;//保证帐号的唯一性
	//将读写位置设置到文件头
	lseek(fd,0,SEEK_SET);
	//再将x的值写到文件中，覆盖原来的x
	write(fd,&x,sizeof(x));
	close(fd);
	
	return x;
}

//将一个新帐号存到文件中
int createUser(struct Account acc)
{
	char filename[20] = {};
	sprintf(filename,"%d.dat",acc.id);//为每个帐号建立一个文件
	int fd = open(filename,O_WRONLY|O_CREAT|O_EXCL,0600);
	if(fd == -1){
		perror("创建账户文件失败\n");
		return -1;
	}
	if(write(fd,&acc,sizeof(acc)) < 0)
		return -1;
	
	close(fd);
	printf("创建用户成功\n");
	return 0;
}

//删除账户
int deleteUser(struct Account acc)
{
	char filename[20] = {};
	sprintf(filename,"%d.dat",acc.id);
	int res = remove(filename);
	if(res == -1){
		perror("删除账户文件失败\n");
		return -1;
	}
	printf("删除账户文件成功\n");
	return 0;
}

//存钱
int saveMoney(struct Account acc)
{
	struct Account accMove = {};
	char filename[20] = {};
	sprintf(filename,"%d.dat",acc.id);
	int fd = open(filename,O_RDWR);
	if(fd == -1){
		perror("打开文件失败");
		return -1;
	}
	read(fd,&accMove,sizeof(acc));
	accMove.balance += acc.balance;
	lseek(fd,0,SEEK_SET);
	if(write(fd,&accMove,sizeof(accMove)) < 0)
		return -1;

	close(fd);
	
	return 0;
}

//取钱
int getMoney(struct Account acc)
{
	struct Account accMove = {};
	char filename[20] = {};
	sprintf(filename,"%d.dat",acc.id);
	int fd = open(filename,O_RDWR);
	if(fd == -1){
		perror("打开文件失败");
		return -1;
	}

	read(fd,&accMove,sizeof(acc));
	accMove.balance -= acc.balance;
	if(accMove.balance < 0){
		printf("余额不足，无法取钱\n");
		close(fd);
		return -1;
	}
	lseek(fd,0,SEEK_SET);
	if(write(fd,&accMove,sizeof(accMove)) < 0)
		return -1;

	close(fd);
	
	return 0;
}

//查询
int checkMoney(struct Account *acc)
{
	struct Account accMove = {};
	char filename[20] = {};
	sprintf(filename,"%d.dat",(*acc).id);
	int fd = open(filename,O_RDONLY);
	if(fd == -1){
		perror("打开文件失败");
		return -1;
	}

	read(fd,&accMove,sizeof(accMove));
	if(strcmp(accMove.password,(*acc).password) != 0){
		printf("对比错误\n");
		close(fd);
		return -1;
	}
	strcpy((*acc).name,accMove.name);
	(*acc).balance = accMove.balance;
	
	close(fd);

	return 0;
}
