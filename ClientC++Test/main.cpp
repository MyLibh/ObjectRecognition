// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "ClientC++Test.hpp"

int main()
{
	Client client;
	if (client.connect2Server())
	{
		client.sendTask("Test.png");

		client.getResult();
	}

	system("pause");
	return 0;
}