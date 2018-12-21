#pragma once
//mysql_demo.cpp

#include <mysql.h>	// mysqlÌØÓĞ
#include <string>
#include <vector>
void MYMD5(char data[],int data_len,char hash_res[]);

struct SERVER_MYSQL{
private:
MYSQL     *mysql; 

public:

SERVER_MYSQL();
//void sql_connect();
//void sql_reconnect();
~SERVER_MYSQL();

//int sql_setup(){
//   return 0;
//}


int check_user(const char usr[],char passwd[]);

int create_user(char usr[],char passwd[]);

int need_set_passwd(char usr[]);

int set_passwd(const char usr[],char passwd[]);

int get_userlist(std::vector<std::string> &ve);

int get_settings(char usr[]);

int set_settings(char usr[],int settings);


int sql_update_msg(char send_usr[],char recv_usr[],char msg[]);
int sql_update_broadcast(char send_usr[],char msg[]);
int get_msglist(char from_id[],char to_id[],int num,std::vector<std::string> &ve);
int test_db();
};