#ifndef BANK_H_
#define BANK_H_

//声明两个key，用来给消息队列用，这两个key在另一个文件中定义
extern const int key1;			//客户端向服务器发送消息队列的key
extern const int key2;			//服务器向客户端发送消息队列的key

//消息类型，定义为宏变量，方便处理
#define M_OPEN			1		//代表开户功能
#define M_DESTROY		2		//代表销户功能
#define M_SAVE			3		//代表存钱功能
#define M_TAKE			4		//代表取钱功能
#define M_QUERY			5		//代表查询功能
#define M_TRANSF		6		//代表转帐功能
#define M_SUCCESS		7		//代表处理成功
#define M_FAILED		0		//代表处理失败

//包含账户信息的账户结构体
struct Account{
	int id;						//帐号
	char name[10];				//用户名
	char password[10];			//密码
	double balance;				//金额
};

//消息的结构体
struct Msg{
	long mtype;					//消息的类型
	struct Account acc;			//账户信息的结构体
};

#endif
