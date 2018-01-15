// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Server.hpp"

using namespace Network;

int main()
{
	Server server;
	server.start();
	
	server.work();

	server.close();
	system("pause");
	return 0;
}