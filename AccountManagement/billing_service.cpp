#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cmath>
#include <iomanip>
#include "header.h"
#include "card_info.h"
#include "card_storage.h"

extern CardInfo* getAndVerifyCard(std::vector<CardInfo>& cards, const std::string& actionName);

// 上机：要求卡号 + 密码，校验通过且卡未被注销且未在用，设置为正在使用并记录登录开始时间（存于 tLast）
void login()
{
	std::vector<CardInfo> cards;
	loadCards(cards);

	CardInfo* p = getAndVerifyCard(cards, "上机");
	if (!p) return;

	if (p->nStatus == 1)
	{
		std::cout << "该卡已在使用中。" << std::endl;
		return;
	}

	// 成功上机：设置状态并记录登录开始时间到 tLast
	std::time_t now = std::time(nullptr);
	p->nStatus = 1;
	p->setLastFromTimeT(now); // 将 tLast 作为当前会话的开始时间
	if (p->tStart == 0) p->setStartFromTimeT(now);
	if (p->tEnd == 0) p->setEndFromTimeT(now + 365LL*24*60*60);

	if (updateCardInStore(cards, *p))
	{
		std::cout << "上机成功。开始时间：" << CardInfo::timeToString(p->tLast) << std::endl;
	}
	else
	{
		std::cout << "上机失败：保存数据失败。" << std::endl;
	}
}

// 下机：要求卡号 + 密码，校验通过且卡处于使用中，计算时长并扣费，更新余额/累计消费/使用次数/状态
void logout()
{
	std::vector<CardInfo> cards;
	loadCards(cards);

	CardInfo* p = getAndVerifyCard(cards, "下机");
	if (!p) return;

	if (p->nStatus != 1)
	{
		std::cout << "该卡当前不在使用中，不能下机。" << std::endl;
		return;
	}

	// 计算时长（秒）
	std::time_t now = std::time(nullptr);
	int64_t start = p->tLast; // 上机时将开始时间保存在 tLast
	if (start <= 0)
	{
		std::cout << "未找到上机开始时间，无法计算费用。" << std::endl;
		return;
	}
	int64_t seconds = static_cast<int64_t>(now) - start;
	if (seconds < 0) seconds = 0;

	// 计费规则：未满1小时 5 元；超过1小时，首小时 5 元，之后每小时 3 元（不足小时按整小时计）
	double charge = 0.0;
	const int64_t oneHour = 3600;
	if (seconds <= oneHour)
	{
		charge = 5.0;
	}
	else
	{
		int64_t extraSeconds = seconds - oneHour;
		double extraHours = std::ceil(static_cast<double>(extraSeconds) / oneHour);
		charge = 5.0 + 3.0 * extraHours;
	}

	// 检查余额（要求余额 >= charge）
	if (static_cast<double>(p->fBalance) < charge)
	{
		std::cout << "余额不足，当前余额：" << std::fixed << std::setprecision(2) << p->fBalance
		          << "，本次应扣：" << std::fixed << std::setprecision(2) << charge
		          << "。请先充值后再下机。" << std::endl;
		return;
	}

	// 扣费并更新记录
	p->fBalance -= static_cast<float>(charge);
	p->fTotalUse += static_cast<float>(charge);
	p->nUseCount += 1;
	p->nStatus = 0;
	p->setLastFromTimeT(now); // 将 tLast 更新为本次结束时间

	if (updateCardInStore(cards, *p))
	{
		// 输出使用时长和费用
		int64_t hours = seconds / 3600;
		int64_t mins = (seconds % 3600) / 60;
		int64_t secs = seconds % 60;
		std::cout << "下机成功。结束时间：" << CardInfo::timeToString(p->tLast)
		          << "，本次时长：" << hours << "时" << mins << "分" << secs << "秒"
		          << "，本次扣费：" << std::fixed << std::setprecision(2) << charge
		          << "，当前余额：" << std::fixed << std::setprecision(2) << p->fBalance
		          << "，累计使用次数：" << p->nUseCount << std::endl;
	}
	else
	{
		std::cout << "下机失败：保存数据失败。" << std::endl;
	}
}