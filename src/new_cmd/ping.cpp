#include "../../inc/Server.hpp"

/**
 * @brief The PING command is sent to check the other side of the connection is still
 * 		connected, and/or to check for connection latency.
 * 
 * 		When receiving a PING message, clients or servers must reply to it with a
 * 		PONG message with the same <token> value. 
 * 
 * 		Numeric replies:
 * 		461 - ERR_NEEDMOREPARAMS -  "<client> <command> :Not enough parameters" > DEALT BY IRSSI
 * 		409 - ERR_NOORIGIN - "<client> :No origin specified" (when <token> is empty) > DEALT BY IRSSI
 * 
 * 		IRSSI :
 * 		With this reference client, sending a PING to a server sends a CTCP request
 * 		to a nickname or a channel. Using the char '*' pings every user in a channel.
 * 		Syntax : PING [<nick> | <channel> | *]
 */

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