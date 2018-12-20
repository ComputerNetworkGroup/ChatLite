
#include <iostream>
using namespace std ;

int main()
{
	int a  = 4;

	int &b = a ;

	cout << hex << &a <<endl;
	cout << hex << &b <<endl;

	return 0;
}