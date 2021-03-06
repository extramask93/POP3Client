// POP3Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <set>
#include <string>
#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <chrono>
#include <atomic>
#include <fstream>
#include "MessageHistorian.h"
#include "boost/algorithm/string/classification.hpp"
#include "Pop3.h"
#include "boost/program_options.hpp" 


namespace po = boost::program_options;
using namespace std::chrono_literals;
class Message;

bool exitFlag = false;
std::atomic<unsigned int> NrOfReceievedMessages = 0;
std::mutex mymutex;
std::condition_variable mycond;
po::variables_map ini;
std::set<std::string> receieved;

po::variables_map TryParseCommandLineArguments(int, char**);

void ReceptionThread()
{
	MessageHistorian historian{ "history.txt" };
	std::cout << "Mail check running, with refresh rate of " << ini["delay"].as<unsigned>()
	<< " sec...\nPress 'q' to quit\n";
	while(!exitFlag)
	{
		std::set<std::string> diff{};
		std::set<std::string> uidls;
		Pop3 pop3{};
		try {
			pop3.Connect(ini["ip"].as<std::string>(), ini["port"].as<uint16_t>());
			pop3.Login(ini["login"].as<std::string>(), ini["pass"].as<std::string>());
			uidls = pop3.GetAllUIDL();
			pop3.Close();
		}
		catch(std::exception &e)
		{
			std::cout << e.what()<<"\n Retrying..."<<std::endl;
		}
		std::set_difference(uidls.begin(), uidls.end(),
			historian.begin(), historian.end(), std::inserter(diff, diff.begin()));
		NrOfReceievedMessages += diff.size();
		for(auto it = diff.begin();it != diff.end();++it)
		{
			std::cout <<"New message arrived, id: "<< *it << '\n';
		}
		historian.AddHistory(diff.begin(), diff.end());
		std::unique_lock<std::mutex> lock(mymutex);
		mycond.wait_for(lock,
			std::chrono::seconds(ini["delay"].as<unsigned>()),
			[]() { return exitFlag; });
	}
}
int main(int argc, char *argv[])
{
	ini = TryParseCommandLineArguments(argc, argv);
	std::thread worker{ ReceptionThread };
	while (!exitFlag) {
		if (GetKeyState('Q') & 0x8000)
		{
			std::cout << "Exit Requested, about to shutdown...\n";
			std::cout << "During this session receieved " << NrOfReceievedMessages<<" new messages\n";
			std::lock_guard<std::mutex> lock(mymutex);
			exitFlag = true;
			mycond.notify_one();
			break;
		}
		std::this_thread::sleep_for(100ms);
	}
	worker.join();
	return 0;
}
void options_required(const po::variables_map& vm, const std::vector<std::string>& required_options)
{
	for (const auto& item : required_options) {
		if (vm.count(item) == 0)
			throw std::logic_error(std::string("Option '") + item
				+ "' is required.");
	}
}
po::variables_map TryParseCommandLineArguments(int argc, char **argv)
{
	std::ifstream configFile{ "config.ini",std::ifstream::in };
	po::options_description configFileOptions("Allowed options");
	configFileOptions.add_options()
		("ip", po::value<std::string>(), "POP3 server address")
		("port", po::value<uint16_t>(), "POP3 server port")
		("login,l", po::value<std::string>(), "Email login")
		("pass,p", po::value<std::string>(), "Email password")
		("delay", po::value<unsigned>(), "Delay in seconds between subsequent email checks")
		("help,h", "Produce help message")
		("version,v", "Prints version number")
		;
	po::variables_map vm;
	auto a = *argv;
	try {
		po::store(po::parse_command_line(argc, argv, configFileOptions), vm);
		po::store(po::parse_config_file(configFile, configFileOptions), vm);
		options_required(vm, { "ip","port","login","pass","delay", });
	}
	catch (const std::logic_error &ex)
	{
		std::cerr << ex.what() << '\n';
		std::cout << configFileOptions << "\n";
		exit(1);
	}
	catch (std::exception)
	{
		std::cerr << "Error: Unrecognized Option!\n";
		exit(2);
	}
	po::notify(vm);
	if (vm.count("help")) {
		std::cout << configFileOptions << "\n";
		exit(0);
	}
	if (vm.count("version"))
	{
		std::cout << "v.1.0.0\n";
		exit(0);
	}
	return vm;
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
