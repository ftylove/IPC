/*
	服务端
*/

#include "bank.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

//首先需要创建两个全局存储消息队列id的变量
static int msgid1;
static int msgid2;

//函数，用来创建两个消息队列，用到msgget函数
void init()
{
	//megget函数，第一个参数是key，bank.h中有定义
	//IPC_CREAT表示没有此消息队列则创建
	//IPC_EXCL表示如果队列存在，则提示错误
	//0666是给此消息队列对象添加一些权限设置，类似文件系统
	
	//创建消息队列1
	msgid1 = msgget(key1,IPC_CREAT|IPC_EXCL|0666);
	if(msgid1 == -1){
		perror("消息队列1创建失败\n");
		exit(-1);
	}
	printf("消息队列1创建成功\n");

	//创建消息队列2
	msgid2 = msgget(key2,IPC_CREAT|IPC_EXCL|0666);
	if(msgid2 == -1){
		perror("消息队列2创建失败\n");
		exit(-1);
	}
	printf("消息队列2创建成功\n");
}

//启动服务器，创建子进程来执行用户不同选择的结果
void start()
{
	printf("服务器正在启动\n");
	sleep(2);
	
	//创建子进程
	pid_t open_pid = vfork();
	if(open_pid == -1){
		perror("创建子进程失败\n");
		exit(-1);
	}
	else if(open_pid == 0){//进入子进程
		//printf("处于子进程中\n");
		execl("open","open",NULL);//不再和父进程共用代码段
	}
	else{
		printf("正在等待客户端选择\n");
		waitpid(open_pid,0,0);//阻塞，等待子进程结束
	}
}

void sig_exit()
{
	printf("服务器正在关闭...\n");
	sleep(2);
	if(msgctl(msgid1,IPC_RMID,NULL) == -1){
		perror("消息队列1删除失败\n");
		exit(-1);
	}
	else
		printf("消息队列1删除成功\n");

	if(msgctl(msgid2,IPC_RMID,NULL) == -1){
		perror("消息队列2删除失败\n");
		exit(-1);
	}
	else
		printf("消息队列2删除成功\n");

	printf("服务器成功关闭\n");
	exit(0);//直接结束程序
}

int main()
{
	//我们退出服务器用一个信号处理函数来实现
	printf("退出服务器请按CTRL+C\n");//发送SIGINT信号
	signal(SIGINT,sig_exit);//自定义信号处理函数，进行退出操作
	//做好了善后工作，从头启动服务器
	
	//创建消息队列
	init();

	//启动服务
	start();
	return 0;
}
