#include <iostream>

void outputMenu()
{
	std::cout << "----------菜单----------" << std::endl;
	std::cout << "1. 添加卡" << std::endl;
	std::cout << "2. 查询卡" << std::endl;
	std::cout << "3. 上机" << std::endl;
	std::cout << "4. 下机" << std::endl;
	std::cout << "5. 充值" << std::endl;
	std::cout << "6. 退费" << std::endl;
	std::cout << "7. 查询统计" << std::endl;
	std::cout << "8. 注销卡" << std::endl;
	std::cout << "0. 退出" << std::endl;
	std::cout << "请选择菜单项编号(0 ~ 8): ";
}