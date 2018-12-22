#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include<openssl/md5.h>
#include "sql.h"
using namespace std;
/* #define USRNAME "u1651025"
#define PASSWD "u1651025"
#define HOSTNAME "localhost"
#define DBNAME "db1651025"
 */

#define USRNAME "root"
#define PASSWD ""
#define HOSTNAME "localhost"
#define DBNAME "test"
//д�˴󲿷�֮����sprintf��strcat����д���Ȳ���

SERVER_MYSQL::SERVER_MYSQL(){
   if ((mysql = mysql_init(NULL))==NULL) {  //��ʼ������
    	cout << "mysql_init failed" << endl;  
      exit(0);
   }

   if (mysql_real_connect(mysql,HOSTNAME,USRNAME, PASSWD,DBNAME,0, NULL, 0)==NULL) {//�������ݿ⣬ʧ�ܷ���NULL
      cout << "mysql_real_connect failed(" << mysql_error(mysql) << ")" << endl;
      exit(0);
   }
   mysql_set_character_set(mysql, "gbk"); // �����ַ���������������ַ�����
}
SERVER_MYSQL::~SERVER_MYSQL(){
   mysql_close(mysql);
}



int SERVER_MYSQL::test_db(){
   //������
   MYSQL_RES *result;
   MYSQL_ROW  row;
   if (mysql_query(mysql, "select * from test")) {
      cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
      return -1;
   }

   /* ����ѯ����洢���������ִ����򷵻�NULL
      ע�⣺��ѯ���ΪNULL�����᷵��NULL */
   if ((result = mysql_store_result(mysql))==NULL) {
   cout << "mysql_store_result failed" << endl;
   return -1;
   }

   /* ��ǰ��ѯ���ļ�¼������ */
   int res=(int)mysql_num_rows(result);

   /* ѭ����ȡ�������������ļ�¼
   1�����ص���˳����selectָ������˳����ͬ����row[0]��ʼ
   2���������ݿ�����ʲô���ͣ�C�ж��������ַ��������д���������б�Ҫ����Ҫ�Լ�����ת��
   3�������Լ�����Ҫ��֯�����ʽ */
   while((row=mysql_fetch_row(result))!=NULL) {
      cout << row[0];
      cout << endl;
      }

   /* �ͷ�result */
   mysql_free_result(result);
   return res;
}
//mysql_free_result(result);   


int SERVER_MYSQL::check_user(char usr[],char passwd[]){
   //usr:�û���
   //passwd:����
   //��ȷ����1��������󷵻�0 �����д��󷵻�-1
   MYSQL_RES *result;
   MYSQL_ROW  row;
   char str[128]="select count(*) from userlist where id=\"";
   strcat(str,usr);
   strcat(str,"\" and passwd=MD5(\"");
   strcat(str,passwd);
   strcat(str,"\");");

   if (mysql_query(mysql, str)) {
      cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
      return -1;
   }

   if ((result = mysql_store_result(mysql))==NULL) {
      cout << "mysql_store_result failed" << endl;
      return -1;
   }
   row=mysql_fetch_row(result);
   int res = atoi(row[0]);

   mysql_free_result(result);
   return res;
}

int SERVER_MYSQL::create_user(char usr[],char passwd[]){
   //usr:�û���
   //passwd:����
   //��ȷ����0�����д��󷵻�-1
   char str[128]="insert into userlist(id,passwd,log_time) values(\"";
   strcat(str,usr);
   strcat(str,"\",MD5(\"");
   strcat(str,passwd);
   strcat(str,"\"),\"1970-1-1 00:00:00\");");  //��Ҫ�޸������ʱ��
   if (mysql_query(mysql, str)) {
      cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
      return -1;
   }
   strcpy(str,"insert into settings(id,set_color) values(\"");
   strcat(str,usr);
   strcat(str,"\",0);");
   if (mysql_query(mysql, str)) {
      cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
      return -1;
   }
   return 0;
}

int SERVER_MYSQL::need_set_passwd(char usr[]){
   //�ж��Ƿ���Ҫ�������룬�����Ҫ����1�����򷵻�0
   MYSQL_RES *result;
   MYSQL_ROW  row;
   char str[128]="select count(*) from userlist where id=\"";
   strcat(str,usr);
   strcat(str,"\" and log_time=\"1970-1-1 00:00:00\";");

   if (mysql_query(mysql, str)) {
      cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
      return -1;
   }

   if ((result = mysql_store_result(mysql))==NULL) {
      cout << "mysql_store_result failed" << endl;
      return -1;
   }
   row=mysql_fetch_row(result);
   int res = atoi(row[0]);
   mysql_free_result(result);
   return res;
}

int SERVER_MYSQL::set_passwd(const char usr[],char passwd[]){
   //�������룬�ɹ�����0�����򷵻�-1
   char str[128]="update userlist set passwd=MD5(\"";
   strcat(str,passwd);
   strcat(str,"\"),log_time=now() where id=\"");
   strcat(str,usr);
   strcat(str,"\";");
   if (mysql_query(mysql, str)) {
      cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
      return -1;
   }
   return 0;
}

int SERVER_MYSQL::get_userlist(vector<string> & ve){
   //��ѯ�����û����ɹ������û�����,�ַ����ŵ�vector���У����򷵻�-1
   ve.clear();
   MYSQL_RES *result;
   MYSQL_ROW  row;
   char str[128]="select id from userlist;";
   if (mysql_query(mysql, str)) {
      cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
      return -1;
   }

   if ((result = mysql_store_result(mysql))==NULL) {
      cout << "mysql_store_result failed" << endl;
      return -1;
   }
   int res=(int)mysql_num_rows(result);
   while((row=mysql_fetch_row(result))!=NULL) {
      ve.push_back(row[0]);
   }

   mysql_free_result(result);
   return res;
}

int SERVER_MYSQL::get_settings(char usr[]){
   //��ѯ��ɫ����ѯ�ɹ�����ֵ��ʧ�ܷ���-1
   MYSQL_RES *result;
   MYSQL_ROW  row;
   char str[128]="select set_color from settings where id=\"";
   strcat(str,usr);
   strcat(str,"\";");
   if (mysql_query(mysql, str)) {
      cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
      return -1;
   }

   if ((result = mysql_store_result(mysql))==NULL) {
      cout << "mysql_store_result failed" << endl;
      return -1;
   }
   if((int)mysql_num_rows(result)>0){
      row=mysql_fetch_row(result);
      int res=atoi(row[0]);
      mysql_free_result(result);
      return res;
   }else{
      mysql_free_result(result);
      return -1;
   }

   return 0;
}
int SERVER_MYSQL::set_settings(char usr[],int settings){
   //������ɫ����ȷ������ɫ�����󷵻�-1
   char str[128]="update settings set set_color=";
   int len=strlen(str);
   sprintf(str+len,"%d",settings);
   strcat(str," where id=\"");
   strcat(str,usr);
   strcat(str,"\";");
   cout<<str<<endl;
   if (mysql_query(mysql, str)) {
      cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
      return -1;
   }
   return settings;
}

int SERVER_MYSQL::sql_update_msg(char send_usr[],char recv_usr[],char msg[]){
//���������¼,�ɹ�����0�����򷵻�-1
   char str[512]="insert into msgs(from_id,to_id,msg_time,msg) values(\"";
   strcat(str,send_usr);
   strcat(str,"\",\"");
   strcat(str,recv_usr);
   strcat(str,"\",now(),\"");
   strcat(str,msg);
   strcat(str,"\");");  //��Ҫ�޸������ʱ��
   if (mysql_query(mysql, str)) {
      cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
      return -1;
   }

   return 0;
}

int SERVER_MYSQL::get_msglist(char usr[],int num,std::vector<std::string> &ve){
   //��ѯ���շ�Ϊusr�����num�Ŀǰ��ʵ��һ��Ч�������Ż�
   ve.clear();
   MYSQL_RES *result;
   MYSQL_ROW  row;
   char str[128];
   sprintf(str,"select from_id,msg from msgs where to_id=\"%s\" or to_id=NULL order by msg_time desc limit %d;",usr,num);
   if (mysql_query(mysql, str)) {
      cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
      return -1;
   }

   if ((result = mysql_store_result(mysql))==NULL) {
      cout << "mysql_store_result failed" << endl;
      return -1;
   }
   int res=(int)mysql_num_rows(result);
   while((row=mysql_fetch_row(result))!=NULL) {
      string tmp=row[0];
      ve.push_back(tmp+row[1]);
   }
   mysql_free_result(result);
   return res;
}