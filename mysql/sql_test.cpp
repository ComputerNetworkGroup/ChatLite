//mysql_demo.cpp
#include <iostream>	// cin,coutµÈ
#include <iomanip>	// setwµÈ
#include "sql.h"
using namespace std;

int main(int argc, char* argv[])
{
    SERVER_MYSQL dbsql;
    vector<string> ve;
    while(1){
        char id[100],id2[100],msg[100];
        int num;
        cin>>id>>id2>>num;

        if(dbsql.get_msglist(id,id2,num,ve)>=0){
            for(auto i:ve){
                cout<<i<<endl;
            }
            //cout<<color<<endl;
            cout<<1<<endl;
        }
        else 
            cout<<"-1"<<endl;
    }

    return 0;
}
