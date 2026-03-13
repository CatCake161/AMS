#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include "header.h"
#include "card_info.h"
#include "card_storage.h"

static std::string formatTimeInt64(int64_t t)
{
	return CardInfo::timeToString(t);
}

void addCard()
{
	std::vector<CardInfo> cards;
	loadCards(cards);

	std::string name;
	std::cout << "请输入卡号 (最多17字符)：";
	std::cin >> name;
	if (name.empty() || name.size() >= sizeof(((CardInfo*)0)->aName)) {
		std::cout << "卡号长度不正确。" << std::endl;
		return;
	}
	if (findCardByName(cards, name)) {
		std::cout << "卡号已存在。" << std::endl;
		return;
	}

	std::string pwd;
	std::cout << "请输入密码 (最多7字符)：";
	std::cin >> pwd;
	if (pwd.empty() || pwd.size() >= sizeof(((CardInfo*)0)->aPwd)) {
		std::cout << "密码长度不正确。" << std::endl;
		return;
	}

	double initBalance = 0.0;
	std::cout << "请输入初始余额 (数字)：";
	if (!(std::cin >> initBalance)) {
		std::cin.clear();
		std::cin.ignore(10000, '\n');
		std::cout << "金额输入无效。" << std::endl;
		return;
	}

	CardInfo c;
	c.setName(name);
	c.setPwd(pwd);
	c.nStatus = 0;
	c.setStartFromTimeT(std::time(nullptr));
	c.setEndFromTimeT(std::time(nullptr) + 365LL*24*60*60); // 默认有效期1年
	c.fBalance = static_cast<float>(initBalance);
	c.fTotalUse = 0.0f;
	c.setLastFromTimeT(0);
	c.nUseCount = 0;
	c.nDel = 0;

	if (addCardToStore(cards, c)) {
		std::cout << "添加卡成功。" << std::endl;
	} else {
		std::cout << "添加卡失败：可能卡号已存在或文件写入失败。" << std::endl;
	}
}

void queryCard()
{
	std::vector<CardInfo> cards;
	loadCards(cards);

	std::string name;
	std::cout << "请输入要查询的卡号：";
	std::cin >> name;
	CardInfo *p = findCardByName(cards, name);
	if (!p) {
		std::cout << "未找到卡号：" << name << std::endl;
		return;
	}

	std::string pwd;
	std::cout << "请输入密码以查看卡信息：";
	std::cin >> pwd;
	if (!p->checkPwd(pwd)) {
		std::cout << "密码错误，无法查看卡信息。" << std::endl;
		return;
	}

	std::cout << "卡号: " << p->aName << std::endl;
	std::cout << "状态: " << p->nStatus << " (0-未用,1-使用,2-注销,3-失效)" << std::endl;
	std::cout << "删除标识: " << p->nDel << std::endl;
	std::cout << "开卡时间: " << formatTimeInt64(p->tStart) << std::endl;
	std::cout << "截止时间: " << formatTimeInt64(p->tEnd) << std::endl;
	std::cout << "累计消费: " << p->fTotalUse << std::endl;
	std::cout << "最后使用: " << formatTimeInt64(p->tLast) << std::endl;
	std::cout << "使用次数: " << p->nUseCount << std::endl;
	std::cout << "余额: " << p->fBalance << std::endl;
}

void logoutCard()
{
	std::vector<CardInfo> cards;
	loadCards(cards);

	std::string name;
	std::cout << "请输入要注销的卡号：";
	std::cin >> name;
	CardInfo *p = findCardByName(cards, name);
	if (!p) {
		std::cout << "未找到卡号：" << name << std::endl;
		return;
	}

	std::string pwd;
	std::cout << "请输入密码以确认注销：";
	std::cin >> pwd;
	if (!p->checkPwd(pwd)) {
		std::cout << "密码错误，无法注销卡。" << std::endl;
		return;
	}

	if (markCardDeleted(cards, name)) {
		std::cout << "卡已注销。" << std::endl;
	} else {
		std::cout << "注销失败：未找到卡或写入失败。" << std::endl;
	}
}