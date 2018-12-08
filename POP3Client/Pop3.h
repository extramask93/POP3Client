#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "Message.h"
#include "SocketTCP.h"

enum State
{
	OK,ERR
};
struct Response
{
	State state;
	std::string message;
	std::vector<std::string> data;
};
class Pop3
{
public:
	Pop3() = default;
	void Connect(std::string address, uint32_t port = 110);
	void Login(std::string login, std::string password) const;
	std::vector<std::string> GetAllUIDL() const;
	Message GetMessageByUIDL(const std::string uidl);
	void Close();
	~Pop3();
private:
	Response GetServerResponse() const;
	Response GetMultilineResponse() const;
	void SendCommand(const std::string& command) const;
	SocketTCP sock;
};

