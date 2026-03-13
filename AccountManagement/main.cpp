#include <iostream>
#include "header.h"

int main()
{
	int input;
	do
	{	
		outputMenu();
		std::cin >> input;
		switch (input)
		{
			case 1: addCard(); break;
			case 2: queryCard(); break;
			case 3: login(); break;
			case 4: logout(); break;
			case 5: recharge(); break;
			case 6: refund(); break;
			case 7: queryStatistics(); break;
			case 8: logoutCard(); break;
			case 0: std::cout << "退出系统。" << std::endl; break;
			default: std::cout << "请检查您输入的数字是否为0 ~ 8" << std::endl; break;
		}
	} while (input);
	return 0;
}