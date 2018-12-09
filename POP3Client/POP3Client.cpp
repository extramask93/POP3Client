// POP3Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "SocketTCP.h"
#include "IPAddress.h"
#include <set>
#include <string>
#include <iostream>
#include <thread>
#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/classification.hpp"

#define PORT 110
#define IP "pop3.poczta.onet.pl"
class Message;
std::set<std::string> receieved;

int main()
{
	SocketTCP sock;
	std::string buffer;
	char cc[200];
	unsigned a = 0;
	IPAddress ip{ IP };
	sock.TCPConnect(ip, PORT);
	sock.TCPReceiveLine(buffer);
	std::cout << buffer<<'\n';
	sock.TCPSendString("user jd33790@onet.pl\r\n");
	sock.TCPReceiveLine(buffer);
	std::cout << buffer << '\n';
	sock.TCPSendString("pass SMTPPoP3\r\n");
	sock.TCPReceiveLine(buffer);
	std::cout << buffer << '\n';

	sock.TCPSendString("quit\r\n");
	sock.TCPReceiveLine(buffer);
	std::cout << buffer << '\n';


}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
