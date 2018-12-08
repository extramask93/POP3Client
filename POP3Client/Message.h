#pragma once
#include <string>
class Message
{
public:
	explicit Message(std::string content);
	std::string GetSubject();
	std::string GetSender();
	~Message();
};

