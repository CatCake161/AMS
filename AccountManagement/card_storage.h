#pragma once

#include "card_info.h"
#include <vector>
#include <string>

bool loadCards(std::vector<CardInfo> &cards, const std::string &filename = "cards.csv");
bool saveCards(const std::vector<CardInfo> &cards, const std::string &filename = "cards.csv");

bool addCardToStore(std::vector<CardInfo> &cards, const CardInfo &card, const std::string &filename = "cards.csv");
CardInfo *findCardByName(std::vector<CardInfo> &cards, const std::string &name);
bool markCardDeleted(std::vector<CardInfo> &cards, const std::string &name, const std::string &filename = "cards.csv");
bool updateCardInStore(std::vector<CardInfo> &cards, const CardInfo &card, const std::string &filename = "cards.csv");