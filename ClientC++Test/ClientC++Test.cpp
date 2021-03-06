// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <iostream> // std::cerr, std::endl
#include <fstream>  // std::ifstream
#include <string>   // std::string, std::to_string

#include "ClientC++Test.hpp"

#pragma comment(lib, "Ws2_32.lib")

//====================================================================================================================================
//=========================================================METHOD_DEFINITION==========================================================
//====================================================================================================================================

#pragma region METHOD_DEFINITION

Client::Client() noexcept :
	server_(),
	client_(),
	serverAddr_()
{
	WSADATA wsData = {};

	if (FAILED(WSAStartup(MAKEWORD(2, 2), &wsData)))
		PostQuitMessage(WSAGetLastError());
}

Client::~Client()
{
	closesocket(server_);
	closesocket(client_);
}

bool Client::initServerConnection(std::string_view ip)
{
	if ((server_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		std::cerr << "WSAError: " << WSAGetLastError() << std::endl;
		
		return false;
	}

	ZeroMemory(&serverAddr_, sizeof(serverAddr_));
	serverAddr_.sin_family           = AF_INET;
	serverAddr_.sin_addr.S_un.S_addr = inet_addr(ip.data());
	serverAddr_.sin_port             = htons(1234);

	return true;
}

bool Client::connect2Server()
{
	if (!initServerConnection())
		return false;

	if (connect(server_, reinterpret_cast<sockaddr*>(&serverAddr_), sizeof(serverAddr_)) == SOCKET_ERROR)
	{
		std::cerr << "Cannot connect to server, WSAError: " << WSAGetLastError() << std::endl;
		
		return false;
	}

	return true;
}

bool Client::sendTask(std::string_view filename)
{
	std::ifstream input(filename.data(), std::ios::binary);
	if (!input.is_open())
	{
		std::cerr << "Cannot open file: " << filename << std::endl;

		return false;
	}

	while (!input.eof())
	{
		char buf[MAX_BUFFER_LENGTH] = { };
		input.read(buf, MAX_BUFFER_LENGTH);

		send(server_, buf, MAX_BUFFER_LENGTH, NULL);
	}
	send(server_, "TASKEND", static_cast<int>(std::strlen("TASKEND")), 0);

	input.close();

	return true;
}

bool Client::getResult()
{
	char num[8] = { };
	if (FAILED(recv(server_, num, 8, 0)))
	{
		std::cerr << "WSAError: " << WSAGetLastError() << std::endl;

		return false;
	}

	std::ofstream output(std::string("Result") + num + ".png", std::ios::binary);
	if (!output.is_open())
	{
		std::cerr << "Cannot open/create result file\n";

		return false;
	}

	while (true)
	{
		char buf[MAX_BUFFER_LENGTH] = {};
		if (FAILED(recv(server_, buf, MAX_BUFFER_LENGTH, 0)))
		{
			std::cerr << "WSAError: " << WSAGetLastError() << std::endl;

			return false;
		}

		if (!std::string_view(buf).compare("RESULTEND"))
		{
			std::cout << "Result received\n";

			output.close();

			break;
		}

		output.write(buf, MAX_BUFFER_LENGTH);
	}

	return true;
}

#pragma endregion