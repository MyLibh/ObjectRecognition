#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h> 
#include <string>     // std::string

#include "ObjectRecognition.hpp"


namespace Network
{
	constexpr unsigned short STD_PORT = 1234;

	class Server final
	{
		bool connectWithClient(size_t);
		int  compute();
		bool sendAnswer();

	public:
		explicit Server() noexcept;
		Server(const Server&) = delete;
		Server(Server&&)      = delete;
		~Server();

		Server& operator=(const Server&) = delete;
		Server& operator=(Server&&)      = delete;

		bool start();
		bool work();
		bool close();

	private:
		static constexpr unsigned short MAX_BUFFER_LENGTH = 1 << 10;

		SOCKET      server_;
		SOCKET      client_;
		sockaddr_in serverBind_;
		sockaddr_in sockaddr_;
		bool        started_;
		bool        closed_;
	};
};
