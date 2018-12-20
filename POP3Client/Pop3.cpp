#include "pch.h"
#include "Pop3.h"
#include "SocketTCP.h"
#include <map>
#include <utility>
#include "IPAddress.h"
#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/classification.hpp"


void Pop3::Connect(std::string address, uint32_t port)
{
	IPAddress adr{ address };
	sock.TCPConnect(adr, port);
	if(const auto response = GetServerResponse(); response.state == State::ERR)
	{
		throw std::runtime_error("Connection refused" + response.message);
	}
}

void Pop3::Login(std::string login, std::string password) const
{
	SendCommand("user " + login);
	if (const auto response = GetServerResponse(); response.state == State::ERR)
	{
		throw std::runtime_error("Authorization failed" + response.message);
	}
	SendCommand("pass " + password);
	if (const auto response = GetServerResponse(); response.state == State::ERR)
	{
		throw std::runtime_error("Authorization failed" + response.message);
	}
}

std::set<std::string> Pop3::GetAllUIDL() const
{
	std::set<std::string> uidl{};
	std::vector<std::string> buffer{};
	SendCommand("uidl");
	const auto response = GetMultilineResponse();
	if (response.state == State::ERR)
	{
		throw std::runtime_error("Reading UIDL's failed" + response.message);
	}
	for (auto line : response.data) {
		boost::split(buffer, line, boost::is_any_of(" "));
		uidl.insert(buffer.at(1));
	}
	return uidl;
}

Message Pop3::GetMessageByUIDL(std::string uidl)
{
	std::map<std::string, std::string> uidlmap;
	std::vector<std::string> buffer{};
	SendCommand("uidl");
	const auto response = GetMultilineResponse();
	if (response.state == State::ERR)
	{
		throw std::runtime_error("Reading UIDL's failed" + response.message);
	}
	for (auto line : response.data) {
		boost::split(buffer, line, boost::is_any_of(" "));
		auto ms = std::make_pair(buffer[1],buffer[0]);
		uidlmap.insert(ms);
	}
	auto id = uidlmap[uidl];
	return GetMessageByID(id);
}

Message Pop3::GetMessageByID(std::string id)
{
	SendCommand("retr " +id);
	const auto response = GetMultilineResponse();
	if (response.state == State::ERR)
	{
		throw std::runtime_error("Reading UIDL's failed" + response.message);
	}
	return Message { response.data };
}


void Pop3::Close()
{
	SendCommand("quit");
	GetMultilineResponse();
}

Pop3::~Pop3()
{
	Close();
}

Response Pop3::GetServerResponse() const
{
	Response response{};
	std::string resp;
	sock.TCPReceiveLine(resp);
	if(resp[0] == '+')
	{
		response.state = State::OK;
		resp.erase(0, 4);
	}
	else
	{
		response.state = State::ERR;
		resp.erase(0, 5);
	}
	response.message = resp;
	response.data.clear();
	return response;
}

Response Pop3::GetMultilineResponse() const
{
	std::string temp;
	Response response = GetServerResponse();
	size_t bytesRead = 0;
	while(true)
	{
		temp.clear();
		bytesRead = sock.TCPReceiveLine(temp);
		if(bytesRead == 0 || temp == ".")
		{
			break;
		}
		if(temp[0] =='.')
		{
			temp.erase(0, 1);
		}
		response.data.push_back(temp);
	}
	return response;
}

void Pop3::SendCommand(const std::string & command) const
{
	sock.TCPSendString(command + "\r\n");
}
