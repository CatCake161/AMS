#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <ctime>
#include <cstdint>
#include <cstring>
#include <string>
#include <sstream>
#include <iomanip>

struct CardInfo
{
	char aName[18];
	char aPwd[8];
	int nStatus;
	// 使用明确的 64-bit 整数保存时间戳，避免不同平台 time_t 大小差异导致的问题
	int64_t tStart;
	int64_t tEnd;
	float fTotalUse;
	int64_t tLast;
	int nUseCount;
	float fBalance;
	int nDel;

	CardInfo()
	{
		std::memset(this, 0, sizeof(CardInfo));
		nStatus = 0;
		nDel = 0;
		fTotalUse = 0.0f;
		fBalance = 0.0f;
		nUseCount = 0;
		tStart = tEnd = tLast = 0;
	}

	void setName(const std::string &name)
	{
#if defined(_MSC_VER)
		strncpy_s(aName, sizeof(aName), name.c_str(), _TRUNCATE);
#else
		std::strncpy(aName, name.c_str(), sizeof(aName) - 1);
		aName[sizeof(aName) - 1] = '\0';
#endif
	}

	void setPwd(const std::string &pwd)
	{
#if defined(_MSC_VER)
		strncpy_s(aPwd, sizeof(aPwd), pwd.c_str(), _TRUNCATE);
#else
		std::strncpy(aPwd, pwd.c_str(), sizeof(aPwd) - 1);
		aPwd[sizeof(aPwd) - 1] = '\0';
#endif
	}

	bool checkPwd(const std::string &pwd) const
	{
		return std::strncmp(aPwd, pwd.c_str(), sizeof(aPwd)) == 0;
	}

	bool isActive() const
	{
		return (nStatus == 1) && (nDel == 0);
	}

	// 辅助：从 time_t 设置字段
	void setStartFromTimeT(std::time_t t) { tStart = static_cast<int64_t>(t); }
	void setEndFromTimeT(std::time_t t) { tEnd = static_cast<int64_t>(t); }
	void setLastFromTimeT(std::time_t t) { tLast = static_cast<int64_t>(t); }

	// 辅助：以 time_t 读取字段
	std::time_t getStartAsTimeT() const { return static_cast<std::time_t>(tStart); }
	std::time_t getEndAsTimeT() const { return static_cast<std::time_t>(tEnd); }
	std::time_t getLastAsTimeT() const { return static_cast<std::time_t>(tLast); }

	// 将时间戳格式化为可读字符串（本地时间）
	static std::string timeToString(int64_t t)
	{
		if (t == 0) return "0";
		std::time_t tt = static_cast<std::time_t>(t);
		std::tm tm{};
#if defined(_MSC_VER)
		localtime_s(&tm, &tt);
#else
		std::tm *ptm = std::localtime(&tt);
		if (ptm) tm = *ptm;
#endif
		char buf[64]{0};
		std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
		return std::string(buf);
	}

	std::string toCsv() const
	{
		std::ostringstream ss;
		ss << aName << ','
		   << aPwd << ','
		   << nStatus << ','
		   << static_cast<long long>(tStart) << ','
		   << static_cast<long long>(tEnd) << ','
		   << std::fixed << std::setprecision(2) << fTotalUse << ','
		   << static_cast<long long>(tLast) << ','
		   << nUseCount << ','
		   << std::fixed << std::setprecision(2) << fBalance << ','
		   << nDel;
		return ss.str();
	}

	static bool fromCsv(const std::string &line, CardInfo &out)
	{
		std::istringstream ss(line);
		std::string token;
		auto next = [&](std::string &dst) -> bool {
			if (!std::getline(ss, dst, ',')) return false;
			return true;
		};

		CardInfo tmp;
		if (!next(token)) return false; tmp.setName(token);
		if (!next(token)) return false; tmp.setPwd(token);
		if (!next(token)) return false; tmp.nStatus = std::stoi(token);
		if (!next(token)) return false; tmp.tStart = static_cast<int64_t>(std::stoll(token));
		if (!next(token)) return false; tmp.tEnd = static_cast<int64_t>(std::stoll(token));
		if (!next(token)) return false; tmp.fTotalUse = std::stof(token);
		if (!next(token)) return false; tmp.tLast = static_cast<int64_t>(std::stoll(token));
		if (!next(token)) return false; tmp.nUseCount = std::stoi(token);
		if (!next(token)) return false; tmp.fBalance = std::stof(token);
		if (!next(token)) return false; tmp.nDel = std::stoi(token);

		out = tmp;
		return true;
	}
};