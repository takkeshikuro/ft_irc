#include "../../inc/Server.hpp"

std::vector<std::string> ft_split(const std::string& str, const std::string& delimiters);

void	Server::msg0(std::string buffer, Client c_client)
{
	std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(buffer);
    while (std::getline(tokenStream, token, '\n')) {
        tokens.push_back(token);
    }
	if (tokens.size() == 0)
		return ;
    for (size_t i = 0; i < tokens.size(); i++)
		msg(tokens[i] + "\n", c_client);
}

void    Server::msg(std::string buffer, Client c_client)
{
	// std::cout << "in msg() buffer is :\n" << buffer[i];
	std::stringstream   sbuf(buffer);
    std::string         args[3];
	std::getline(sbuf, args[0], ' ');
	std::vector<std::string>	dests;

    //get dest client nickname or channel name
    while (std::getline(sbuf, args[1], ' ')) {
		if (!args[1].empty())
			break;
	}
	if (args[1].empty())
	{
		std::string noparam = "Error: Missing parameters.\n";
		send(c_client.get_client_fd(), noparam.c_str(), noparam.size(), 0);
		return ;
	}
	// get msg to send
	while (std::getline(sbuf, args[2], '\n')) {
		if (!args[2].empty())
			break;
	}
	if (args[2].empty())
	{
		std::string noparam = "Error: Missing parameters.\n";
		send(c_client.get_client_fd(), noparam.c_str(), noparam.size(), 0);
		return ;
	}
	for (size_t i = 0; i < args[2].size(); i++)
	{
		if (args[2][i] == '\r')
			args[2][i] = '\n';
	}
	if (c_client.get_is_irssi() == false)
	{
		dests = ft_split(args[1], ",");
		if (dests.size() == 0)
			return ;
		for (size_t k = 0; k < dests.size(); k++)
		{
			if (dests[k][0] == '#')
			{
				args[1] = dests[k];
				msg_channel(args, c_client);
			}
			else
				msg_user(dests[k], args[2], c_client);
		}
	}
	else
	{
		if (args[1][0] == '#')
			msg_channel(args, c_client);
		else
			msg_user(args[1], args[2], c_client);
	}
}

void	Server::msg_user(std::string target, std::string msg, Client c_client)
{
	int	index = index_client(target);
	if (index == -1)
	{
		send(c_client.get_client_fd(), ERR_NOSUCHNICK(target).c_str(), ERR_NOSUCHNICK(target).size(), 0);
		return ;
	}

    std::string to_send = YEL + c_client.getNickname() + " (PRIVMSG): \e[0m" + msg + "\n";
	if (client_vec[index].get_is_irssi() == true)
	{
		std::string nick = c_client.getNickname();
		std::string userna = c_client.getUsername();
		std::string target = client_vec[index].getNickname();
    	to_send = RPL_PRIVMSG(nick, userna, target, msg);
	}

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
	if (i == channel_vec.size() || channel_vec.size() == 0)
	{
		send(c_client.get_client_fd(), ERR_NOSUCHCHANNEL(args[1]).c_str(), ERR_NOSUCHCHANNEL(args[1]).size(), 0);
		return ;
	}
	if (index_channel_nick(c_client.getNickname(), channel_vec[i]) == -1) {
		std::string err = ERR_NOTONCHANNEL(c_client.getNickname(), channel_vec[i].get_name());
		send(c_client.get_client_fd(),err.c_str(), err.size(), 0);	
		return ;
	}
	std::ostringstream oss;
	oss << "[" << channel_vec[i].get_name() << "]" << c_client.getNickname() << " " << args[2]; 
	index = index_channel(c_client, channel_vec[i]); // check if cclient is in the channel
	if (index == -1)
		return ;
	std::string nick = c_client.getNickname();
	std::string nick0 = c_client.getNickname();
	if (index_operator(c_client, channel_vec[i]) != -1)
		nick0 = nick + "(+)";
	std::string userna = c_client.getUsername();
	std::string target = channel_vec[i].get_name();
	std::string msg = args[2];
	channel_vec[i].send_string(RPL_PRIVMSG(nick0, userna, target, msg), nick, target, msg);
}