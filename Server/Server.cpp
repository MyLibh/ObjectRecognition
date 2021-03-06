// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <iostream>  // std::cout, std::cerr, std::endl
#include <fstream>  // std::ofstream

#include "Server.hpp"

#pragma comment(lib, "Ws2_32.lib")

namespace Network
{
	//====================================================================================================================================
	//=========================================================METHOD_DEFINITION==========================================================
	//====================================================================================================================================

#pragma region METHOD_DEFINITION

	Server::Server() noexcept :
		server_(),
		client_(),
		serverBind_(),
		sockaddr_(),
		started_(FALSE),
		closed_(FALSE)
	{
		WSADATA wsaData = { };
		if (FAILED(WSAStartup(MAKEWORD(2, 2), &wsaData)))
		{
			std::cout << "Cannot start up the WSA\n";

			PostQuitMessage(WSAGetLastError());
		}
	}

	Server::~Server()
	{
		if (!closed_)
			close();
	}

	bool Server::connectWithClient(size_t i)
	{
		ZeroMemory(&sockaddr_, sizeof(sockaddr_));
		int newLen = sizeof(sockaddr_);
		if (FAILED(client_ = accept(server_, (sockaddr*)&sockaddr_, &newLen)))
		{
			std::cerr << "Failed to connect " << i << ", WSAError: " << WSAGetLastError() << std::endl;

			return false;
		}
		
		std::cout << "Client(" << i << ") IP: " << inet_ntoa(static_cast<in_addr>(sockaddr_.sin_addr)) << ", port: " << ntohs(sockaddr_.sin_port) << "\n\n";
		
		return true;
	}

	int Server::compute()
	{
		std::string file("Task.png");

		std::ofstream output(file, std::ios::binary);
		if (!output.is_open())
		{
			std::cerr << "Cannot open/create result file\n";

			return -1;
		}

		while (true)
		{
			char buf[MAX_BUFFER_LENGTH] = {};
			if (FAILED(recv(client_, buf, MAX_BUFFER_LENGTH, 0)))
			{
				std::cerr << "WSAError: " << WSAGetLastError() << std::endl;

				return -1;
			}
			
			if (!std::string_view(buf).compare("TASKEND"))
			{
				std::cout << "Task received\n\n";

				output.close();

				break;
			}

			output.write(buf, MAX_BUFFER_LENGTH);
		}
		
		return FindCircles(file);
	}

	bool Server::sendAnswer()
	{
		char num[8] = {};
		_itoa_s(compute(), num, 10);
		send(client_, num, 8, 0);

		std::ifstream input("Task.png", std::ios::binary);
		if (!input.is_open())
		{
			std::cerr << "Cannot open file: Task.png\n";

			return false;
		}

		while (!input.eof())
		{
			char buf[MAX_BUFFER_LENGTH] = {};
			input.read(buf, MAX_BUFFER_LENGTH);

			send(client_, buf, MAX_BUFFER_LENGTH, NULL);
		}
		send(client_, "RESULTEND", static_cast<int>(std::strlen("RESULTEND")), 0);

		input.close();

		return true;
	}

	bool Server::start()
	{
		if (started_)
		{
			std::cerr << "Server has already started. Do not start it again\n";

			return false;
		}

		if (closed_)
			closed_ = !closed_;

		std::cout << "Starting server...\n";

		if ((server_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		{
			std::cerr << "WSAError: " << WSAGetLastError() << std::endl;
			
			return false;
		}

		ZeroMemory(&serverBind_, sizeof(serverBind_));
		serverBind_.sin_family = PF_INET;
		serverBind_.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		serverBind_.sin_port = htons(STD_PORT);

		if (bind(server_, reinterpret_cast<sockaddr*>(&serverBind_), sizeof(serverBind_)) == SOCKET_ERROR)
		{
			std::cerr << "WSAError: " << WSAGetLastError() << std::endl;

			return false;
		}

		if (FAILED(listen(server_, SOMAXCONN)))
		{
			std::cerr << "WSAError: " << WSAGetLastError() << std::endl;

			return false;
		}

		started_ = true;
		std::cout << "Server started\n\n";

		return started_;
	}

	bool Server::work()
	{
		if (!started_ || closed_)
		{
			std::cerr << "You did not start the server\n";

			return false;
		}

		for (size_t i = 0; !GetAsyncKeyState(VK_ESCAPE); ++i)
		{
			if (!connectWithClient(i) || !sendAnswer())
				return false;
		}

		return false;
	}

	bool Server::close()
	{
		if (!started_)
		{
			std::cerr << "Do not close server if you did not start it!\n";
			
			return false;
		}

		if (closed_)
		{
			std::cerr << "Server has already closed. Do not close it again\n";
			
			return false;
		}

		std::cout << "Closing server...\n";

		closesocket(server_);

		closed_  = true;
		started_ = false;

		std::cout << "Server closed\n\n";

		return true;
	}

#pragma endregion

} // namespace Network