
#include <iostream>
#include <map>
using namespace std ;

int main()
{
	int a  = 4;

	int &b = a ;

	map <string , int > indexMap ;

	indexMap.insert(make_pair("user10",10));

	cout << indexMap["user10"]<<endl;

	return 0;
}