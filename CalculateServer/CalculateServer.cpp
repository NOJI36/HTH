#include <iostream>
#include "connector.h"


int main()
{
	Connector* connect = Connector::GetInstance();
	if (!connect->readyToConnect()) {
		std::cout << "ready failed" << std::endl;
	}
	SOCKET client = connect->acceptClient();
	std::cout << "client 연결됨" << std::endl;
	char buffer[255];
	int recvLen = recv(client, buffer, 255, 0);
	buffer[recvLen] = '\0';
	std::cout << buffer << std::endl;
	send(client, buffer, recvLen, 0);
}
