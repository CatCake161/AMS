#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include "header.h"
#include "card_info.h"
#include "card_storage.h"

void queryStatistics()
{
	std::vector<CardInfo> cards;
	loadCards(cards);

	if (cards.empty())
	{
		std::cout << "没有卡片记录。" << std::endl;
		return;
	}

	const std::time_t now = std::time(nullptr);

	int totalCards = static_cast<int>(cards.size());
	int deletedCount = 0;
	int statusCount[4] = { 0, 0, 0, 0 }; // index 对应 0,1,2,3
	int activeCount = 0;
	int expiredCount = 0;

	double totalBalance = 0.0;
	double totalRevenue = 0.0; // fTotalUse 累计消费
	long long totalUseCount = 0;

	for (const auto &c : cards)
	{
		if (c.nDel == 1) ++deletedCount;
		if (c.nStatus >= 0 && c.nStatus <= 3) ++statusCount[c.nStatus];
		if (c.isActive()) ++activeCount;
		if (c.nDel == 0 && c.tEnd > 0 && c.tEnd < static_cast<int64_t>(now)) ++expiredCount;

		totalBalance += static_cast<double>(c.fBalance);
		totalRevenue += static_cast<double>(c.fTotalUse);
		totalUseCount += c.nUseCount;
	}

	int nonDeleted = totalCards - deletedCount;
	double avgBalance = nonDeleted > 0 ? (totalBalance / nonDeleted) : 0.0;
	double avgUseCount = nonDeleted > 0 ? (static_cast<double>(totalUseCount) / nonDeleted) : 0.0;

	// 前 5 名余额最高的卡（不包含已删除）
	std::vector<const CardInfo*> byBalance;
	for (const auto &c : cards) if (c.nDel == 0) byBalance.push_back(&c);
	std::sort(byBalance.begin(), byBalance.end(), [](const CardInfo *a, const CardInfo *b) {
		return a->fBalance > b->fBalance;
	});

	// 输出统计
	std::cout << "===== 卡片统计 =====" << std::endl;
	std::cout << "总卡数: " << totalCards << std::endl;
	std::cout << "已删除: " << deletedCount << std::endl;
	std::cout << "状态分布: 未使用(0)=" << statusCount[0]
	          << " 在用(1)=" << statusCount[1]
	          << " 注销(2)=" << statusCount[2]
	          << " 失效(3)=" << statusCount[3] << std::endl;
	std::cout << "当前在用卡数: " << activeCount << std::endl;
	std::cout << "已过期但未删除卡数: " << expiredCount << std::endl;
	std::cout << std::fixed << std::setprecision(2);
	std::cout << "总余额: " << totalBalance << " 元" << std::endl;
	std::cout << "总累计消费: " << totalRevenue << " 元" << std::endl;
	std::cout << "平均余额(不含已删除): " << avgBalance << " 元" << std::endl;
	std::cout << "平均使用次数(不含已删除): " << avgUseCount << std::endl;
	std::cout << "总使用次数(所有卡): " << totalUseCount << std::endl;

	// 列出余额前 5 的卡
	std::cout << std::endl << "余额排名（前5，不含已删除）:" << std::endl;
	int limit = std::min<int>(5, static_cast<int>(byBalance.size()));
	for (int i = 0; i < limit; ++i)
	{
		const CardInfo *c = byBalance[i];
		std::cout << "#" << (i + 1) << " 卡号: " << c->aName
		          << " 余额: " << std::fixed << std::setprecision(2) << c->fBalance
		          << " 状态:" << c->nStatus
		          << " 累计消费:" << c->fTotalUse
		          << " 最后使用:" << CardInfo::timeToString(c->tLast)
		          << std::endl;
	}

	// 按需还可以打印更多统计（例如按日/月汇总），当前提供汇总和余额排行
}