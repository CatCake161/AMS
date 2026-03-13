#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include "header.h"
#include "card_info.h"
#include "card_storage.h"

void recharge()
{
	std::vector<CardInfo> cards;
	loadCards(cards);

	std::string name;
	std::cout << "请输入要充值的卡号：";
	std::cin >> name;

	CardInfo *p = findCardByName(cards, name);
	if (!p || p->nDel == 1)
	{
		std::cout << "未找到有效的卡号或卡已删除：" << name << std::endl;
		return;
	}

	std::string pwd;
	std::cout << "请输入密码以确认充值：";
	std::cin >> pwd;
	if (!p->checkPwd(pwd))
	{
		std::cout << "密码错误，充值取消。" << std::endl;
		return;
	}

	double amount = 0.0;
	std::cout << "请输入充值金额（正数）：";
	if (!(std::cin >> amount) || amount <= 0.0)
	{
		std::cin.clear();
		std::cin.ignore(10000, '\n');
		std::cout << "金额输入无效。" << std::endl;
		return;
	}

	p->fBalance += static_cast<float>(amount);
	p->setLastFromTimeT(std::time(nullptr));

	if (updateCardInStore(cards, *p))
	{
		std::cout << "充值成功。当前余额：" << p->fBalance
		          << "，最后操作时间：" << CardInfo::timeToString(p->tLast) << std::endl;
	}
	else
	{
		std::cout << "充值失败：保存数据时出错。" << std::endl;
	}
}

void refund()
{
	std::vector<CardInfo> cards;
	loadCards(cards);

	std::string name;
	std::cout << "请输入要退费的卡号：";
	std::cin >> name;

	CardInfo *p = findCardByName(cards, name);
	if (!p || p->nDel == 1)
	{
		std::cout << "未找到有效的卡号或卡已删除：" << name << std::endl;
		return;
	}

	std::string pwd;
	std::cout << "请输入密码以确认退费：";
	std::cin >> pwd;
	if (!p->checkPwd(pwd))
	{
		std::cout << "密码错误，退费取消。" << std::endl;
		return;
	}

	double amount = 0.0;
	std::cout << "请输入退费金额（正数，不能超过当前余额）：";
	if (!(std::cin >> amount) || amount <= 0.0)
	{
		std::cin.clear();
		std::cin.ignore(10000, '\n');
		std::cout << "金额输入无效。" << std::endl;
		return;
	}

	if (static_cast<double>(p->fBalance) < amount)
	{
		std::cout << "退费失败：当前余额不足（当前余额：" << p->fBalance << "）。" << std::endl;
		return;
	}

	p->fBalance -= static_cast<float>(amount);
	p->setLastFromTimeT(std::time(nullptr));

	if (updateCardInStore(cards, *p))
	{
		std::cout << "退费成功。当前余额：" << p->fBalance
		          << "，最后操作时间：" << CardInfo::timeToString(p->tLast) << std::endl;
	}
	else
	{
		std::cout << "退费失败：保存数据时出错。" << std::endl;
	}
}