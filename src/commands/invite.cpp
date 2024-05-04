#include "../../inc/Server.hpp"

// INVITE nick #channel
//verify nick exists
//verify channel exists

std::vector<std::string> ft_split(const std::string& str, const std::string& delimiters);

bool is_invited(int fd, Channel &chan)
{
	if (chan.get_invite_set() == false)
		return true ;
	for (size_t i = 0; i < chan.invited_clients.size(); i++)
	{
		if (chan.invited_clients[i] == fd)
			return true;
	}
	return false;
}

void    Server::invite(std::string buffer, Client c_client)
{
	int                 chan_idx;
	int                 user_idx;
	std::vector<std::string> args = ft_split(buffer, " \r\n");

	if (args.size() < 3)
	{
		size_t size = ERR_NEEDMOREPARAMS(c_client.getNickname(), "/invite").size();
		send(c_client.get_client_fd(), ERR_NEEDMOREPARAMS(c_client.getNickname(), "/invite").c_str(), size, 0);
		return ;
	}
	chan_idx = index_channel_name(args[2], channel_vec); //verify if chan exists
	if (chan_idx == -1)
	{
		std::string to_send = ERR_NOSUCHCHANNEL(args[2]);
		send(c_client.get_client_fd(), to_send.c_str(), to_send.size(), 0);
		return ;
	}
	if (index_channel_nick(c_client.getNickname(), channel_vec[chan_idx]) == -1)
	{
		std::string to_send = ERR_NOTONCHANNEL(c_client.getNickname(), args[2]);
		send(c_client.get_client_fd(), to_send.c_str(), to_send.size(), 0);
		return ;
	}
	user_idx = index_client_vec(args[1], client_vec); //verify if user exists
	if (user_idx == -1)
	{
		std::string to_send = ERR_NOSUCHNICK(c_client.getNickname(), args[1]);
		send(c_client.get_client_fd(), to_send.c_str(), to_send.size(), 0);
		return ;
	}
	channel_vec[chan_idx].invited_clients.push_back(client_vec[user_idx].get_client_fd());
	if (c_client.get_is_irssi() == true)
	{
		std::string to_send = RPL_INVITING(args[2], args[1]);
		send(c_client.get_client_fd(), to_send.c_str(), to_send.size(), 0);
	}
	std::string dest = "You've been invited to join " + args[2] + " by " + c_client.getNickname() + ".\n";
	send(client_vec[user_idx].get_client_fd(), dest.c_str(), dest.size(), 0);
} 