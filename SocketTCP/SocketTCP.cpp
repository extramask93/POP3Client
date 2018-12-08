#include "stdafx.h"
#include "SocektException.h"
#include "SocketTCP.h"
#include "IPAddress.h"

SocketTCP::SocketTCP()
{
	WSAStartup(MAKEWORD(2, 2), &this->wsa);
}

SocketTCP::State SocketTCP::TCPConnect(const IPAddress & remoteAddress, unsigned short remotePort, int timeout)
{
	struct hostent *hent = NULL;
	struct in_addr in_addr;
	struct protoent *pent;
	struct sockaddr_in addr;
	in_addr.S_un.S_addr = htonl(remoteAddress.toInteger());
	pent = getprotobyname("tcp");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(remotePort);
	addr.sin_addr = in_addr;
	memset(addr.sin_zero, 0, 8);
	sock = socket(AF_INET, SOCK_STREAM, pent->p_proto);
	int res = connect(sock,(struct sockaddr *)&addr, sizeof(struct sockaddr));
	if (res < 0)
		throw std::runtime_error("cant connect");
	state = State::Done;
	return state;
}

SocketTCP::State SocketTCP::TCPSend(const void * data, std::size_t size) const
{
	if(send(sock, (const char*)data, size, 0)<0)
	{
		throw SocektException("Error while sending data to the remote host");
	}
	return State::Done;
}

SocketTCP::State SocketTCP::TCPSendString(const std::string s) const
{

	if(send(sock, (const char*)s.c_str(), s.length(), 0)<0)
	{
		throw SocektException("Error while sending data to the remote host");
	}
	return State::Done;
}

SocketTCP::State SocketTCP::TCPReceive(void * data, std::size_t size, std::size_t & received) const
{
	received = recv(sock, (char*)data, size, 0);
	return State::Done;
}
SocketTCP::State SocketTCP::TCPReceiveString(std::string &s) const
{
	char temp;
	while(TCPReceiveChar(&temp) == State::Done)
	{
		s.push_back(temp);
	}
	return State::Done;
}

SocketTCP::State SocketTCP::TCPReceiveChar(char* c) const
{
	const auto status = recv(sock, c, 1, 0);
	if(status>0)
	{
		return State::Done;
	}
	return State::NotReady;
}

size_t SocketTCP::TCPReceiveLine(std::string& line) const
{
	char buffer[2] = "";
	size_t bytesRead = 0;
	line.clear();
	if(TCPReceiveChar(buffer+1) == State::Done)
	{
		do
		{
			line.push_back(buffer[0]);
			buffer[0] = buffer[1];
			++bytesRead;

		} while (TCPReceiveChar(buffer + 1) == State::Done &&
			buffer[0] != '\r' && buffer[1] != '\n');
		line.erase(0, 1);
	}
	return bytesRead;
}

SocketTCP::~SocketTCP()
{
	closesocket(sock);
	state = State::Disconnected;
}
