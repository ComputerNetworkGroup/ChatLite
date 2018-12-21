//mysql_demo.cpp
#include <iostream>	// cin,coutµÈ
#include <iomanip>	// setwµÈ
#include "sql.h"
using namespace std;

int main(int argc, char* argv[])
{
    SERVER_MYSQL dbsql;
    while(1){
        char id[100];
        int color=0;
        cin>>id>>color;
        if(dbsql.set_settings(id,color)){
            //cout<<color<<endl;
            cout<<dbsql.get_settings(id)<<endl;
        }
        else 
            cout<<"11111111111111111"<<endl;
    }

    return 0;
}
