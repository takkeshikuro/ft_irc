#include "../../inc/Server.hpp"

void		display_topic(std::string channel_name, Client c_client, std::vector<Channel> vec);
std::string change_topic(std::string channel_name, Client c_client, std::vector<Channel> vec, std::string new_topic);
std::vector<std::string> ft_split(const std::string& str, const std::string& delimiters);

/**
 * @brief Command : TOPIC <channel> [<topic>]
 * 			
 * 	The TOPIC command is used to change or view the topic of the given channel. 
 * 	If <topic> is not given, either RPL_TOPIC or RPL_NOTOPIC is returned 
 * 	specifying the current channel topic or lack of one. 
 * 	If <topic> is an empty string, the topic for the channel will be cleared.
 * 
 * 	Numeric Replies:
 * 	
 * 	ERR_NEEDMOREPARAMS (461)
 * 	ERR_NOSUCHCHANNEL (403)
 * 	ERR_NOTONCHANNEL (442)
 * 	RPL_NOTOPIC (331)
 * 	RPL_TOPIC (332)
 * 	
 * 	Examples:
 * 	[CLIENT] TOPIC #test :New topic
 *  [SERVER] ; Setting the topic on "#test" to "New topic".
 * 
 * 	[CLIENT] TOPIC #test :
 * 	[SERVER] ; Clearing the topic on "#test"
 * 
 * 	[CLIENT] TOPIC #test
 * 	[SERVER] ; Checking the topic for "#test"
 * 
 */

void    Server::topic(std::string buffer, Client c_client)
{
	std::string total_arg;
	std::string channel_name;
	std::string next_arg;
	std::string client_nickname = c_client.getNickname();

	if (c_client.get_is_irssi() == false)
	{
		std::vector<std::string> arg = ft_split(buffer, " ");
		if (arg.size() == 1)
		{
			std::string param = "Error : need more param (/topic #<channel>)\n";
			send(c_client.get_client_fd(), param.c_str(), param.size(), 0);
			return ;
		}
		else if (arg.size() == 2)
			buffer = buffer + "\n";
		else if (arg.size() > 2)
		{
			std::string p1 = "/TOPIC " + arg[1];
			std::string p2 = buffer.substr(p1.length());
			if (p2[p2.length() - 2] == ':')
				buffer = p1 + " :";
			else
				buffer = p1 + " :" + p2 + "\n";
		}
	}

	if (buffer[buffer.length() - 1] == '\n')
		buffer.erase(buffer.length() - 2);	
	std::size_t pos = buffer.find('#');
	if (pos != std::string::npos)
	{
		total_arg = buffer.substr(pos);
		pos = total_arg.find(' ');
		if (pos != std::string::npos) // remplace topic
		{
			channel_name = total_arg.substr(0, pos);
			pos = total_arg.find(':');
			next_arg = total_arg.substr(pos);
			int index_chan = index_channel_name(channel_name, this->channel_vec);
			
			if (index_chan != -1) 
			{
				std::string new_topic = change_topic(channel_name, c_client, this->channel_vec, next_arg);
				if (new_topic == "err")
					return ;
				channel_vec[index_chan].set_description(new_topic);
				size_t size = RPL_TOPIC(client_nickname, channel_name, new_topic).size();
				for (size_t i = 0; i < channel_vec[index_chan].client_list.size(); ++i)
					send(channel_vec[index_chan].client_list[i].get_client_fd(), RPL_TOPIC(client_nickname, channel_name, new_topic).c_str(), size, 0);	
			}
			else {
				size_t size = ERR_NOSUCHCHANNEL(channel_name).size();
				send(c_client.get_client_fd(), ERR_NOSUCHCHANNEL(channel_name).c_str(), size, 0);
			}
		}
		else  // just display topic
		{
			channel_name = total_arg;
			if (index_channel_name(channel_name, this->channel_vec) != -1)
				display_topic(channel_name, c_client, this->channel_vec);
			else {
				size_t size = ERR_NOSUCHCHANNEL(channel_name).size();
				send(c_client.get_client_fd(), ERR_NOSUCHCHANNEL(channel_name).c_str(), size, 0);
			}
		}
	}
	else {
		size_t size = ERR_NEEDMOREPARAMS(client_nickname, "/topic").size();
		send(c_client.get_client_fd(), ERR_NEEDMOREPARAMS(client_nickname, "/topic").c_str(), size, 0);
	}
}

void	display_topic(std::string channel_name, Client c_client, std::vector<Channel> vec)
{
	int i = index_channel_name(channel_name, vec);
	if (!vec[i].get_description().empty()) 
	{
		std::string actual_topic = vec[i].get_description();
		if (c_client.get_is_irssi() == true) {
			size_t size = RPL_TOPIC(c_client.getNickname(), channel_name, actual_topic).size();
			send(c_client.get_client_fd(), RPL_TOPIC(c_client.getNickname(), channel_name, actual_topic).c_str(), size, 0);	
		}
		else {
			std::string s_topic = "-!- Topic for " + channel_name + ": " + actual_topic + "\r\n";
			send(c_client.get_client_fd(), s_topic.c_str(), s_topic.size(), 0);
		}
	}
	else 
	{
		if (c_client.get_is_irssi() == true) {
			size_t size = RPL_NOTOPIC(c_client.getNickname(), channel_name).size();
			send(c_client.get_client_fd(), RPL_NOTOPIC(c_client.getNickname(), channel_name).c_str(), size, 0);
		}
		else {
			std::string s_notopic = "-!- No topic set for " + channel_name + "\r\n";
			send(c_client.get_client_fd(), s_notopic.c_str(), s_notopic.size(), 0);
		}
	}
}

std::string change_topic(std::string channel_name, Client c_client, std::vector<Channel> vec, std::string new_topic)
{
	std::string err = "err";
	int i = index_channel_name(channel_name, vec);
	std::string client_nickname = c_client.getNickname();
	std::string new_topic_bis;

	if (index_channel_nick(client_nickname, vec[i]) == -1) {
		size_t size = ERR_NOTONCHANNEL(client_nickname, channel_name).size();
		send(c_client.get_client_fd(), ERR_NOTONCHANNEL(client_nickname, channel_name).c_str(), size, 0);	
		return err;
	}
	else if (index_operator_nick(client_nickname, vec[i]) == -1 && vec[i].get_topic_opr() == true) {
		size_t size = ERR_CHANOPRIVSNEEDED(client_nickname, channel_name).size();
		send(c_client.get_client_fd(), ERR_CHANOPRIVSNEEDED(client_nickname, channel_name).c_str(), size, 0);		
		return err;
	}
	if (new_topic == ":") {
		new_topic.clear();
		return new_topic;
	}
	else {
		new_topic_bis = new_topic.substr(1);
		return new_topic_bis;
	}
	return err;
}