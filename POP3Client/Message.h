#pragma once
#include <string>
#include <vector>

class Message
{
public:
	explicit Message(std::vector<std::string> content);
	//std::string GetSubject();
	//std::string GetSender();
	std::vector<std::string> content;
	~Message();
};

