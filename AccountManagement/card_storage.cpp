#include "card_storage.h"
#include <fstream>
#include <sstream>
#include <algorithm>

static const char SEP = ',';

bool loadCards(std::vector<CardInfo> &cards, const std::string &filename)
{
	cards.clear();
	std::ifstream ifs(filename);
	if (!ifs) return true; // 文件不存在视为空集合

	std::string line;
	while (std::getline(ifs, line))
	{
		if (line.empty()) continue;
		CardInfo c;
		if (CardInfo::fromCsv(line, c))
		{
			cards.push_back(c);
		}
	}
	return true;
}

bool saveCards(const std::vector<CardInfo> &cards, const std::string &filename)
{
	std::ofstream ofs(filename, std::ofstream::trunc);
	if (!ofs) return false;
	for (const auto &c : cards)
	{
		ofs << c.toCsv() << '\n';
	}
	return true;
}

bool addCardToStore(std::vector<CardInfo> &cards, const CardInfo &card, const std::string &filename)
{
	// 检查重复卡号
	auto it = std::find_if(cards.begin(), cards.end(), [&](const CardInfo &x) {
		return std::strncmp(x.aName, card.aName, sizeof(x.aName)) == 0;
	});
	if (it != cards.end()) return false;
	cards.push_back(card);
	return saveCards(cards, filename);
}

CardInfo *findCardByName(std::vector<CardInfo> &cards, const std::string &name)
{
	auto it = std::find_if(cards.begin(), cards.end(), [&](CardInfo &x) {
		return std::strncmp(x.aName, name.c_str(), sizeof(x.aName)) == 0;
	});
	if (it == cards.end()) return nullptr;
	return &(*it);
}

bool markCardDeleted(std::vector<CardInfo> &cards, const std::string &name, const std::string &filename)
{
	CardInfo *p = findCardByName(cards, name);
	if (!p) return false;
	p->nDel = 1;
	p->nStatus = 2; // 注销状态
	return saveCards(cards, filename);
}

bool updateCardInStore(std::vector<CardInfo> &cards, const CardInfo &card, const std::string &filename)
{
	CardInfo *p = findCardByName(cards, std::string(card.aName));
	if (!p) return false;
	*p = card;
	return saveCards(cards, filename);
}