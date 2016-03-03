/*
   处理客户端发来的消息
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "bank.h"
#include "dao.h"

int main()
{
	//接收客户端消息的消息队列
	int msgid1 = msgget(key1,0);
	if(msgid1 == -1){
		perror("获取消息队列1失败\n");
		printf("服务器启动失败\n");
		exit(-1);
	}
	//发送消息到客户端的消息队列
	int msgid2= msgget(key2,0);
	if(msgid2 == -1){
		perror("获取消息队列2失败\n");
		printf("服务器启动失败\n");
		exit(-1);
	}

	//获取到消息队列之后，开始接受消息
	while(1){
		struct Msg msg;//存储消息的结构体
		struct Account accMove,accResult;//存储账户信息

		//首先从客户那里收取消息队列1的信息，msgrcv函数
		if(msgrcv(msgid1,&msg,sizeof(msg.acc),0,0) <= 0)
			break;

		//如果接受到了消息，根据消息的不同类型进行不同的操作
		if(msg.mtype == M_OPEN){//开户
			int id = generator_id();
			accMove = msg.acc;
			accMove.id = id;
			if(createUser(accMove) == -1){
				printf("开户失败\n");
				msg.mtype = M_FAILED;
			}
			else{
				printf("开户成功\n");
				msg.acc.id = id;//把分配出的帐号放入消息，以便让客户知晓
				msg.mtype = M_SUCCESS;
			}
			msgsnd(msgid2,&msg,sizeof(msg.acc),0);
		}
		else if(msg.mtype == M_DESTROY){//销户
			if(deleteUser(msg.acc) == -1){
				printf("删除账户失败\n");
				msg.mtype = M_FAILED;
			}
			else{
				printf("删除账户成功\n");
				msg.mtype = M_SUCCESS;
			}
			msgsnd(msgid2,&msg,sizeof(msg.acc),0);
		}
		else if(msg.mtype == M_SAVE){//存钱
			if(saveMoney(msg.acc) == -1){
				printf("存钱失败\n");
				msg.mtype = M_FAILED;
			}
			else{
				printf("存钱成功\n");
				msg.mtype = M_SUCCESS;
			}
			msgsnd(msgid2,&msg,sizeof(msg.acc),0);
		}
		else if(msg.mtype == M_TAKE){//取钱
			if(getMoney(msg.acc) == -1){
				printf("取钱失败\n");
				msg.mtype = M_FAILED;
			}
			else{
				printf("取钱成功\n");
				msg.mtype = M_SUCCESS;
			}
			msgsnd(msgid2,&msg,sizeof(msg.acc),0);
		}
		else if(msg.mtype == M_QUERY){//查询
			if(checkMoney(&(msg.acc)) == -1){
				printf("查询失败\n");
				msg.mtype = M_FAILED;
			}
			else{
				printf("查询成功\n");
				msg.mtype = M_SUCCESS;
			}
			if(msgsnd(msgid2,&msg,sizeof(msg.acc),0) == -1){
				printf("发送失败\n");
				msgctl(msgid2,IPC_RMID,0);//发送失败需要清空消息队列
			}
			else
				printf("发送成功\n");
		}
		else if(msg.mtype == M_TRANSF){

		}
	}
	return 0;
}
