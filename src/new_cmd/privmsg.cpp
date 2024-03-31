#include "../../inc/Server.hpp"

void    Server::msg(std::string buffer, Client c_client)
{
	std::stringstream   sbuf(buffer);
    std::string         args[3];
	std::getline(sbuf, args[0], ' ');
    //get dest client nickname
    while (std::getline(sbuf, args[1], ' ')) {
		if (!args[1].empty())
			break;
	}
	// get msg to send
	while (std::getline(sbuf, args[2], '\n')) {
		if (!args[2].empty())
			break;
	}

	//find dest client fd
	int	index = index_client(args[1]);
	if (index == -1)
	{
		send(c_client.get_client_fd(), ERR_NOSUCHNICK(args[1]).c_str(), ERR_NOSUCHNICK(args[1]).size(), 0);
		return ;
	}
	for (size_t i = 0; i < args[2].size(); i++)
	{
		if (args[2][i] == '\r')
			args[2][i] = '\n';
	}

    std::string to_send = YEL + c_client.getNickname() + " (PRIVMSG)\e[0m" + args[2];
	send(client_vec[index].get_client_fd(), to_send.c_str(), to_send.size(), 0);	
}