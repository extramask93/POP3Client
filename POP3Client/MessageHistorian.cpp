#include "pch.h"
#include "MessageHistorian.h"
#include <fstream>
#include <algorithm>
#include <sstream>


MessageHistorian::MessageHistorian(const std::string& historyFile) : fname(historyFile)
{
	std::fstream fhistory{ fname };
	std::stringstream buffer;
	buffer << fhistory.rdbuf();
	Split(buffer.str(), "\n");
}

void MessageHistorian::AddHistory(std::set<std::string>::iterator begin,
	std::set<std::string>::iterator end)
{
	history.insert(begin, end);
}

MessageHistorian::~MessageHistorian()
{
	std::ofstream fhistory{ fname };
	for (const auto& item : history)
	{
		fhistory << item << '\n';
	}
}

void MessageHistorian::Split(const std::string& s, const std::string& tok)
{
	size_t pos = s.find(tok);
	size_t initialPos = 0;

	while (pos != std::string::npos) {
		history.insert(s.substr(initialPos, pos - initialPos));
		initialPos = pos + tok.length();
		pos = s.find(tok, initialPos);
	}
	if (initialPos != s.length())
		history.insert(s.substr(initialPos, std::min(pos, s.length()) - initialPos + tok.length()));
}
