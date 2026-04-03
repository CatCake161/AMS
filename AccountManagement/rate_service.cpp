#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include "header.h"
#include "card_info.h"
#include "card_storage.h"

// 借用已封装好的验证函数（建议后续可以把他移到 card_storage 中统一），这里暂在同文件内声明。
extern CardInfo* getAndVerifyCard(std::vector<CardInfo>& cards, const std::string& actionName);

void recharge()
{
	std::vector<CardInfo> cards;
	loadCards(cards);

	CardInfo* p = getAndVerifyCard(cards, "充值");
	if (!p) return;

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

	CardInfo* p = getAndVerifyCard(cards, "退费");
	if (!p) return;

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