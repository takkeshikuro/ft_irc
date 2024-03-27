#include "../inc/Server.hpp"

void	Server::PING(std::string buffer, Client c_client)
{
	std::string	nickname = c_client.getNickname();

	if (buffer[0] == ' ')
		buffer.erase(0, 1);
	buffer.insert(0, ": ");
    if (buffer[buffer.length() - 1] == '\n')
        buffer.erase(buffer.length() - 2);
    std::string pong_back = "[" + RPL_PONG(nickname, buffer + ":(127.0.0.1)]");
    send(c_client.get_client_fd(), pong_back.c_str(), pong_back.size(), 0);    
}