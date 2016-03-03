//数据对象的存储

#ifndef DAO_H_
#define DAO_H_
#include "bank.h"

//生成不重复的帐号
int generator_id();

//开户功能
int createUser(struct Account acc);

//销户功能
int deleteUser(struct Account acc);

//存钱功能
int saveMoney(struct Account acc);

//取钱功能
int getMoney(struct Account acc);

//查询功能
int checkMoney(struct Account *acc);

//转帐功能
int moveMoney(struct Account acc1,struct Account acc2,struct Account *p);

#endif
