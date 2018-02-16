
#include "Socket.h"

#include <cstring>
#include <iostream>

#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

Socket::Socket(const std::string hostname, const int port)
{
	this->hostname = hostname;
	this->port = port;

	// Construct socket
	server = gethostbyname(this->hostname.c_str());
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);

    if (socketDescriptor < 0)
    {
    	std::cerr << "Error establishing socket..." << std::endl;
        return;
    }

    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_port = htons(this->port);

    if (!this->doConnect())
    {
    	std::cerr << "Error connecting to host..." << std::endl;
    }
}

Socket::Socket(const int socketDescriptor)
{
	this->socketDescriptor = socketDescriptor;
}

bool Socket::doConnect()
{
	if(connect(socketDescriptor,(struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
    	return false;
    }
    return true;
}

void Socket::closeSocket()
{
	close(socketDescriptor);
}

void Socket::sendMessage(const std::string message, int serverity) const
{
	send(socketDescriptor, message.c_str(), message.size(), serverity);
}

std::string Socket::receiveAmount(const int amountToReceive) const
{
	char data[amountToReceive];
	int counter = 0, flag = 0;
	std::string result = "";

	while (counter < amountToReceive)
	{
		bzero(&data, amountToReceive);
		flag = recv(socketDescriptor, data, (amountToReceive - counter), 0);
		if (flag == 0)
		{
			return std::string(result);
		}
		result += std::string(data);
		counter += flag;
	}
	return std::string(result);
}

std::string Socket::receiveToDelimiter(const char delimiter) const
{
	std::string data = "";
	char temp[2];
	int flag;

	do
	{
		bzero(&temp, sizeof(temp));
		flag = recv(socketDescriptor, temp, 1, 0);

		if (flag == 0)
		{
			return data;
		}
	} while (temp[0] != delimiter);

	return data;
}
