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
#include "Pop3.h"

#define PORT 110
#define IP "pop3.poczta.onet.pl"
class Message;
std::set<std::string> receieved;

int main()
{
	Pop3 pop3{};
	pop3.Connect(IP, PORT);
	pop3.Login("jd33790@onet.pl", "SMTPPoP3");
	auto uidls = pop3.GetAllUIDL();
	for(auto uidl : uidls)
	{
		std::cout << uidl<<"\n";
	}
	auto message =pop3.GetMessageByUIDL(uidls.at(0));
	pop3.Close();

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
