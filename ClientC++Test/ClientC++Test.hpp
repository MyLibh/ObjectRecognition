#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h> 

#include <string_view> // std::string_view

#pragma region CONSTANTS

constexpr std::string_view LOCAL_IP = std::string_view("127.0.0.1", 9);

constexpr size_t MAX_BUFFER_LENGTH = 1 << 10;

#pragma endregion

class Client 
{
	bool initServerConnection(std::string_view = LOCAL_IP);

public:
	explicit Client() noexcept;
	Client(const Client&) = delete;
	Client(Client&&)      = delete;
	~Client();

	Client &operator=(const Client&) = delete;
	Client &operator=(Client&&)      = delete;

	bool connect2Server();
	bool sendTask(std::string_view);
	bool getResult();
	
private:
	SOCKET      server_;
	SOCKET      client_;
	sockaddr_in serverAddr_;
};

