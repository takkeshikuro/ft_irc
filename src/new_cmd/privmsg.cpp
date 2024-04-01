#include "../../inc/Server.hpp"

void    Server::msg(std::string buffer, Client c_client)
{
	std::stringstream   sbuf(buffer);
    std::string         args[3];
	std::getline(sbuf, args[0], ' ');
    //get dest client nickname or channel name
    while (std::getline(sbuf, args[1], ' ')) {
		if (!args[1].empty())
			break;
	}
	// get msg to send
	while (std::getline(sbuf, args[2], '\n')) {
		if (!args[2].empty())
			break;
	}
	for (size_t i = 0; i < args[2].size(); i++)
	{
		if (args[2][i] == '\r')
			args[2][i] = '\n';
	}
	if (args[1][0] == '#')
	{
		msg_channel(args, c_client);
		return ;
	}

	//find dest client fd
	int	index = index_client(args[1]);
	if (index == -1)
	{
		send(c_client.get_client_fd(), ERR_NOSUCHNICK(args[1]).c_str(), ERR_NOSUCHNICK(args[1]).size(), 0);
		return ;
	}
    std::string to_send = YEL + c_client.getNickname() + " (PRIVMSG)\e[0m" + args[2];
	send(client_vec[index].get_client_fd(), to_send.c_str(), to_send.size(), 0);
}

void	Server::msg_channel(std::string args[3], Client c_client)
{
	size_t	i;
	int		index;
	for (i = 0; i < channel_vec.size(); i++) // check if channel exists
	{
		if (channel_vec[i].get_name() == args[1])
			break ;
	}
	if (i == channel_vec.size())
	{
		send(c_client.get_client_fd(), ERR_NOSUCHCHANNEL(args[1]).c_str(), ERR_NOSUCHCHANNEL(args[1]).size(), 0);
		return ;
	}
	std::ostringstream oss;
	// oss << ":" << c_client.getNickname() << "!" << c_client.getUsername() << "@localhost ";
	// oss << "PRIVMSG ";
	// oss << args[1] << args[2];
	oss << "[" << channel_vec[i].get_name() << "]" << c_client.getNickname() << " " << args[2]; 
	index = index_channel(c_client, channel_vec[i]); // check if cclient is in the channel
	
	if (index == -1)
		return ;
	// channel_vec[i].send_string(oss.str());
	std::string nick = c_client.getNickname();
	std::string userna = c_client.getUsername();
	std::string target = channel_vec[i].get_name();
	std::string msg = args[2];
	std::string tosend = "[" + target + "] " + nick + ": " + msg;
	// std::cout <<RPL_PRIVMSG(nick, userna, target, msg);
	channel_vec[i].send_string(RPL_PRIVMSG(nick, userna, target, msg), nick);
}