/*
	客户端
*/

#include <stdio.h>
#include "bank.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>

//定义两个接受消息队列id的变量
static int msgid1;
static int msgid2;

//获取消息队列的函数
void getID()
{
	msgid1 = msgget(key1,0);
	if(msgid1 == -1){
		perror("获取消息队列1失败\n");
		exit(-1);
	}
	msgid2 = msgget(key2,0);
	if(msgid2 == -1){
		perror("获取消息队列2失败\n");
		exit(-1);
	}
}

//开户功能
void createUser()
{
	struct Account acc = {};
	printf("请输入用户名:\n");
	scanf("%s",acc.name);
	printf("请输入密码:\n");
	scanf("%s",acc.password);
	printf("请输入金额:\n");
	scanf("%lf",&acc.balance);
	struct Msg msg = {M_OPEN,acc};

	//获取消息队列
	getID();

	//将消息发送到消息队列1中
	msgsnd(msgid1,&msg,sizeof(msg.acc),0);

	//接收消息队列2中的消息
	msgrcv(msgid2,&msg,sizeof(msg.acc),0,0);
	if(msg.mtype == M_SUCCESS){
		printf("开户成功\n");
		printf("您的帐号是%d\n请牢记！\n",msg.acc.id);
	}
	else
		printf("开户失败\n");
}

//销户功能
void destroyUser()
{
	struct Account acc;
	printf("请输入需要删除的帐号:\n");
	scanf("%d",&acc.id);
	printf("请再次输入需要删除的账号:\n");
	
	//确认账户是否正确输入想删除的帐号
	int confirmAcc;
	scanf("%d",&confirmAcc);

	//如果输入有问题，则重新输入
	while(confirmAcc != acc.id){
		printf("输入不正确，请重新输入:\n");
		scanf("%d",&acc.id);
		printf("请再次确认要删除的账号:\n");
		scanf("%d",&confirmAcc);
	}
	//如果输入没有问题，开始处理销户
	struct Msg msg = {M_DESTROY,acc};
	getID();
	msgsnd(msgid1,&msg,sizeof(msg.acc),0);
	msgrcv(msgid2,&msg,sizeof(msg.acc),0,0);
	if(msg.mtype == M_SUCCESS)
		printf("销户成功\n");
	else
		printf("销户失败\n");
}

//存钱功能
void saveMoney()
{
	struct Account acc;
	printf("请输入需要存钱的帐号:\n");
	scanf("%d",&acc.id);
	printf("请再次输入以确认帐号:\n");

	int confirmAcc;
	scanf("%d",&confirmAcc);
	
	//如果输入有问题，则重新输入
	while(confirmAcc != acc.id){
		printf("两次输入不同，请重新输入:\n");
		scanf("%d",&acc.id);
		printf("请再次输入以确认账号:\n");
		scanf("%d",&confirmAcc);
	}

	printf("请输入需要存入的金额:\n");
	scanf("%lf",&acc.balance);
	double confirmMoney;
	printf("请再次输入存入的金额:\n");
	scanf("%lf",&confirmMoney);
	while(confirmMoney != acc.balance){
		printf("两次输入的金额数目不同，请重新输入:\n");
		scanf("%lf",&acc.balance);
		printf("请再次输入存入金额:\n");
		scanf("%lf",&confirmMoney);
	}

	printf("好了，把钱放进来，我们会处理的\n");
	sleep(5);

	//转入服务器进行处理
	struct Msg msg = {M_SAVE,acc};
	getID();
	msgsnd(msgid1,&msg,sizeof(msg.acc),0);
	msgrcv(msgid2,&msg,sizeof(msg.acc),0,0);
	if(msg.mtype == M_SUCCESS)
		printf("存钱成功\n");
	else
		printf("存钱失败，请联系工作人员取出吞入钞票\n");
}

//取钱功能
void getMoney()
{
	struct Account acc;
	printf("请输入需要取钱的帐号:\n");
	scanf("%d",&acc.id);
	printf("请再次输入以确认帐号:\n");

	int confirmAcc;
	scanf("%d",&confirmAcc);
	while(confirmAcc != acc.id){
		printf("两次输入不同，请重新输入:\n");
		scanf("%d",&acc.id);
		printf("请再次输入以确认账号:\n");
		scanf("%d",&confirmAcc);
	}

	printf("请输入需要取出的金额:\n");
	scanf("%lf",&acc.balance);
	double confirmMoney;
	printf("请再次输入取出的金额:\n");
	scanf("%lf",&confirmMoney);
	while(confirmMoney != acc.balance){
		printf("两次输入的金额数目不同，请重新输入:\n");
		scanf("%lf",&acc.balance);
		printf("请再次输入需要取出的金额:\n");
		scanf("%lf",&confirmMoney);
	}

	printf("拿上你的钱，离我远点\n");
	sleep(5);

	struct Msg msg = {M_TAKE,acc};
	getID();
	msgsnd(msgid1,&msg,sizeof(msg.acc),0);
	msgrcv(msgid2,&msg,sizeof(msg.acc),0,0);
	if(msg.mtype == M_SUCCESS)
		printf("取钱成功\n");
	else
		printf("取钱失败\n");
}

//查询功能
void checkMoney()
{
	struct Account acc = {};
	
	printf("请输入您的帐号:\n");
	scanf("%d",&acc.id);
	printf("请输入您的密码:\n");
	scanf("%s",acc.password);
	
	getID();
	struct Msg msg = {M_QUERY,acc};
	msgsnd(msgid1,&msg,sizeof(msg.acc),0);
	if(msgrcv(msgid2,&msg,sizeof(msg.acc),0,0) == -1)
		printf("帐号密码不合，请重新输入\n");
	else{
		printf("%s,您好\n",msg.acc.name);
		printf("您的帐号:%d\n",msg.acc.id);
		printf("余额为%.2lf\n\n",msg.acc.balance);
	}
}

//转帐功能
void moveMoney()
{

}

//客户端界面
void manPage()
{
	while(1){
		printf("  欢迎使用迷你ATM机\n");
		printf("-------------------\n");
		printf("[1] 开户");
		printf("      [2] 销户\n");
		printf("[3] 存钱");
		printf("      [4] 取钱\n");
		printf("[5] 查询");
		printf("      [6] 转帐\n");
		printf("[0] 退出\n");
		printf("-------------------\n");

		int num = 0;
		scanf("%d",&num);
		switch(num){
			case 1:
				createUser();
				break;
			case 2:
				destroyUser();
				break;
			case 3:
				saveMoney();
				break;
			case 4:
				getMoney();
				break;
			case 5:
				checkMoney();
				break;
			case 6:
				moveMoney();
				break;
			case 0:
				printf("谢谢使用！\n");
				return;
			default:
				printf("输入错误\n");
				break;
		}
	}
}

int main()
{
	manPage();
	return 0;
}
