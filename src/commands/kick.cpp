#include "../../inc/Server.hpp"

std::vector<std::string> ft_split(const std::string& str, const std::string& delimiters);
std::string get_data(std::string buffer, int hook);

/**
 * @brief The KICK command can be used to request the forced removal of a user 
 *  from a channel. It causes the <user> to be removed from the <channel> by force. 
 *  If no comment is given, the server SHOULD use a default message instead.
 * 
 *  Parameters: <channel> <user> *( "," <user> ) [<comment>]
 * 
 * Numeric Replies:
 * 
 * ERR_NEEDMOREPARAMS (461)
 * ERR_NOSUCHCHANNEL (403)
 * ERR_CHANOPRIVSNEEDED (482)
 * ERR_USERNOTINCHANNEL (441)
 * ERR_NOTONCHANNEL (442)
 * 
 * Example:
 * Client's request : KICK #Finnish John :Speaking English
 * 
 * Server's Response: " Command to kick John from #Finnish using "Speaking English" 
 * 						as the reason (comment)."
 *
 * @param server
 * @param cmd_infos Structure w/ prefix, command name and message
 */


void    Server::kick(std::string buffer, Client c_client)
{
	std::string	client_nickname	= c_client.getNickname();
	if (c_client.get_is_irssi() == false)
	{
		std::vector<std::string> arg = ft_split(buffer, " ");
		if (arg.size() == 1)
		{
			std::string param = "Error : need more param (/kick <channel> <human>)\n";
			send(c_client.get_client_fd(), param.c_str(), param.size(), 0);
			return ;
		}
		else if (arg.size() == 2)
		{
			size_t size = ERR_NEEDMOREPARAMS(client_nickname, "/kick").size();
			send(c_client.get_client_fd(), ERR_NEEDMOREPARAMS(client_nickname, "/kick").c_str(), size, 0);
			return ;
		}
		else if (arg.size() > 3)
		{
			std::string p1 = "/KICK " + arg[1] + " " + arg[2];
			std::string p2 = buffer.substr(p1.length());
			buffer = p1 + " :" + p2;
		}
	}
	if (c_client.get_is_irssi() == false)
		buffer.erase(buffer.length() - 1);
	std::string channel_name = get_data(buffer, 0);
	std::string kicked_name = get_data(buffer, 1);
	std::string comment = get_data(buffer, 2);
	comment = (comment.empty()) ? "Kicked by the channel's operator" : comment;
	std::cout << "user "<< client_nickname << " try to kick user " << kicked_name << " bc :" << comment<< std::endl;
	if (channel_name.empty() || kicked_name.empty() || channel_name[0] != '#') 
	{
		size_t size = ERR_NEEDMOREPARAMS(client_nickname, "/kick").size();
		send(c_client.get_client_fd(), ERR_NEEDMOREPARAMS(client_nickname, "/kick").c_str(), size, 0);
		return ;
	}
	int i = index_channel_name(channel_name, this->channel_vec);
	if (i == -1) 
	{
		if (c_client.get_is_irssi() == true) {
			size_t size = ERR_NOSUCHCHANNEL(channel_name).size();
			send(c_client.get_client_fd(), ERR_NOSUCHCHANNEL(channel_name).c_str(), size, 0);
			return ;
		}
		std::string to_send = "\e[1;31mError: " + channel_name + " is not a channel.\n" + RESET;
        send(c_client.get_client_fd(), to_send.c_str(), to_send.size(), 0);
		return ;
	}
	if (index_channel_nick(client_nickname, channel_vec[i]) == -1) 
	{
		if (c_client.get_is_irssi() == true) {
			size_t size = ERR_NOTONCHANNEL(client_nickname, channel_name).size();
			send(c_client.get_client_fd(), ERR_NOTONCHANNEL(client_nickname, channel_name).c_str(), size, 0);
			return ;
		}
		std::string to_send = "\e[1;31mError: You are not in the channel " + channel_name + ".\n" + RESET;
        send(c_client.get_client_fd(), to_send.c_str(), to_send.size(), 0);
		return ;
	}
	int j = index_channel_nick(kicked_name, channel_vec[i]);
	if (j == -1) 
	{
		if (c_client.get_is_irssi() == true) {
			size_t size = ERR_USERNOTINCHANNEL(client_nickname, kicked_name, channel_name).size();
			send(c_client.get_client_fd(), ERR_USERNOTINCHANNEL(client_nickname, kicked_name, channel_name).c_str(), size, 0);
			return ;	
		}
		std::string to_send = "\e[1;31mError: no user named " + kicked_name + " in " + channel_name + ".\n" + RESET;
		send(c_client.get_client_fd(), to_send.c_str(), to_send.size(), 0);
		return ;
	}
	if (index_operator_nick(client_nickname, channel_vec[i]) == -1) 
	{
		if (c_client.get_is_irssi() == true) {
			size_t size = ERR_CHANOPRIVSNEEDED(client_nickname, channel_name).size();
			send(c_client.get_client_fd(), ERR_CHANOPRIVSNEEDED(client_nickname, channel_name).c_str(), size, 0);
			return ;
		}
		std::string to_send = "\e[1;31mYou do not have permission to use /KICK in #" + channel_name + ".\n" + RESET;
		send(c_client.get_client_fd(), to_send.c_str(), to_send.size(), 0);
		return ;
	}// kick un operator ??
	else 
	{
		int j = index_client(kicked_name);
		channel_vec[i].send_string_all(RPL_KICK(client_nickname, c_client.getUsername(), channel_name, kicked_name, comment));
		channel_vec[i].rm_user(this->client_vec[j]);
		std::string kicked = red + "[info] You have been kicked from " + channel_name + " by " + client_nickname + " because : " + comment + white + "\r\n";
		send(client_vec[j].get_client_fd(), kicked.c_str(), kicked.size(), 0);
	}
}

std::string get_data(std::string buffer, int hook)
{
	std::string channel_name;
	std::string kicked_name;
	std::string comment;
	std::vector<std::string> vec_arg;
	std::vector<std::string> vec_arg_bis;

	comment.clear();
	
	if (buffer[buffer.length() - 1] == '\n')
		buffer.erase(buffer.length() - 2);
	vec_arg = ft_split(buffer, " ");
	channel_name = vec_arg[1];
	vec_arg_bis = ft_split(channel_name, ",");
	if (vec_arg_bis.size() == 2) {
		channel_name = vec_arg_bis[0];
		kicked_name = vec_arg_bis[1];
	}
	else
		kicked_name = vec_arg[2];
	size_t pos_comment = buffer.find(':');
	if (pos_comment != std::string::npos)
		comment = buffer.substr(pos_comment + 1);
	if (hook == 0)
		return channel_name;
	else if (hook == 1)
		return kicked_name;
	else
		return comment;
	
}
