#pragma once
#include <string>
#include <set>

class MessageHistorian
{
public:
	MessageHistorian(const std::string& historyFile);
	const std::set<std::string>& GetHistory() const
	{
		return history;
	}
	void AddHistory(std::set<std::string>::iterator begin,
		std::set<std::string>::iterator end);
	auto begin() { return history.begin(); }
	auto end() { return history.end(); }
	~MessageHistorian();
private:
	std::set<std::string> history{};
	std::string fname = "";
	void Split(const std::string& s, const std::string &tok);
};

