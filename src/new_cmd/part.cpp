#include "../../inc/Server.hpp"

void		send_to_all(Client c_client, Channel &chan, std::string reason);
bool		is_alpha_in_part(std::string str);
std::string get_channel_name_part(const std::string& arg);
bool		is_inside(Channel &channel, std::string nickname);
std::vector<std::string> ft_split(const std::string& str, const std::string& delimiters);

/**
 * @brief The PART command removes the client from the given channel(s).
 * 
 * 	Syntax: PART <channel>{,<channel>} [<reason>]
 * 
 *  On sending a successful PART command, the user will receive a PART message 
 *  from the server for each channel they have been removed from. 
 *  <reason> is the reason that the client has left the channel(s).
 * 
 *  Numeric Replies:
 *   ERR_NEEDMOREPARAMS (461)
 *   ERR_NOSUCHCHANNEL (403)
 *   ERR_NOTONCHANNEL (442)
 * 	
 * 	Example:
 * 	[IRSSI] /PART #test,#hey :Dining
 * 	[SERVER] leaves both channels #test and #hey with the reason "Dining"
 * 	[SERVER to CLIENT]"@user_id PART #channel Dining" (for EACH channel they leave)
 */

void    Server::part(std::string buffer, Client c_client)
{
	std::string client_nickname = c_client.getNickname();
	std::string	channel_name;
	std::string	total_arg;
	std::string reason;
	reason.clear();

	if (c_client.get_is_irssi() == false)
	{
		std::vector<std::string> arg = ft_split(buffer, " ");
		if (arg.size() == 1)
		{
			std::string param = "Error : need more param (/part <channel>)\n";
			send(c_client.get_client_fd(), param.c_str(), param.size(), 0);
			return ;
		}
		if (arg.size() == 2)
			buffer = buffer + "\r\n";
		else if (arg.size() > 2)
		{
			std::string p1 = "/PART " + arg[1];
			std::string p2 = buffer.substr(p1.length());
			buffer = p1 + " :" + p2 + "\n";
		}
	}
	
	if (buffer[buffer.length() - 1] == '\n')
		buffer.erase(buffer.length() - 2);
	std::size_t pos = buffer.find(' ');
	if (pos != std::string::npos)
		total_arg = buffer.substr(pos + 1);
	
	if (is_alpha_in_part(total_arg) == false) 
	{
		size_t size = ERR_NEEDMOREPARAMS(client_nickname, "/part").size();
		send(c_client.get_client_fd(), ERR_NEEDMOREPARAMS(client_nickname, "/part").c_str(), size, 0);
	}
	std::size_t pos_reason = total_arg.find(':');
	if (pos_reason != std::string::npos)
		reason = total_arg.substr(pos_reason + 1);
	while (is_alpha_in_part(total_arg) == true)
	{
		channel_name.clear();
		channel_name = get_channel_name_part(total_arg);
		total_arg.erase(total_arg.find(channel_name), channel_name.length()); 
		if (channel_name[0] != '#')
			channel_name = "#" + channel_name;
		std::cout<<"user "<<client_nickname<<" try to leave channel "<<channel_name<< " because :{"<< reason<< "}\n";
		int check;
		for (size_t i = 0; i < channel_vec.size(); i++) 
		{
			check = 0;
			if (channel_vec[i].get_name() == channel_name) 
			{
				check = 1;
				if (index_channel_nick(client_nickname, channel_vec[i]) != -1)
				{
					int j = index_client_vec(client_nickname, this->client_vec);
					channel_vec[i].rm_user(client_vec[j]);	
					if (index_operator_nick(client_nickname, channel_vec[i]) != -1)
						channel_vec[i].rm_operator(client_vec[j]);
					client_vec[j].in_channel -= 1;
					size_t size = RPL_PART(client_nickname, c_client.getUsername(), channel_name, reason).size();
					send(c_client.get_client_fd(), RPL_PART(client_nickname, c_client.getUsername(), channel_name, reason).c_str(), size, 0);
					std::string info = yellow + "[info] You just left channel " + channel_name + white + "\r\n";
					send(c_client.get_client_fd(), info.c_str(), info.size(), 0);
					send_to_all(c_client, channel_vec[i], reason);
				}
				else {
					size_t size = ERR_NOTONCHANNEL(client_nickname, channel_name).size();
					send(c_client.get_client_fd(), ERR_NOTONCHANNEL(client_nickname, channel_name).c_str(), size, 0);
					continue ;
				}
			}
		}
		if (!check) {
			size_t size = ERR_NOSUCHCHANNEL(channel_name).size();
			send(c_client.get_client_fd(), ERR_NOSUCHCHANNEL(channel_name).c_str(), size, 0);
			continue ;
		}
	}
}

void	send_to_all(Client c_client, Channel &chan, std::string reason)
{
	std::string nick = c_client.getNickname();
	std::string user = c_client.getUsername();

	for (size_t i = 0; i < chan.client_list.size(); i++) 
	{
		size_t size = RPL_PART(nick, user, chan.get_name(), reason).size();
		send(chan.client_list[i].get_client_fd(), RPL_PART(nick, user, chan.get_name(), reason).c_str(), size, 0);
	}
}

bool		is_alpha_in_part(std::string str)
{
	if (str[0] == ' ')
		str.erase(0, 1);
	if (str.find(" ") != str.npos)
		str = str.substr(0, str.find(" "));
	for (size_t i = 0; i < str.size(); i++) {
		if (isalpha(str[i]))
			return true;
	}
	return false;
}

std::string get_channel_name_part(const std::string& arg) 
{
	std::string channel_name;
	bool name_find = false;

	for (std::size_t i = 0; i < arg.length(); ++i) 
	{
		char c = arg[i];
		if (!name_find) 
		{
			if (!std::isspace(c) && c != ' ' && c != '\a' && c != ',' && c != '#') {
				channel_name += c;
				name_find = true;
			}
		} 
		else 
		{
			if (c == '\a' || c == ' ' || c == ',' || c == ':' || !std::isprint(c))
				break;
			else
				channel_name += c;
		}
	}
	return channel_name;
}

bool	is_inside(Channel &channel, std::string nickname)
{
	int i = index_channel_nick(nickname, channel);
	if (i == -1)
		return false;
	else
		return true; 
}