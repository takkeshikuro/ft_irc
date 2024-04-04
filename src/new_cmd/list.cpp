#include "../../inc/Server.hpp"

static std::string	findAnyChannel(std::string buffer);
static std::string	get_rpl_list(std::string client_nick, Channel &chan);

/**
 * @brief If the exact name of a channel is given, the only information about 
 * 		this channel is requested; otherwise, a list of all channels will be 
 * 		displayed.
 * 
 * 	Syntax : 
 * 	[IRSSI] LIST [-yes] [<channel>]
 * 	
 * 	Numeric Replies:
 * 		RPL_LISTSTART (321) : marks the start of a channel list. 
 * 		RPL_LIST (322) : sends information about a channel to the client.
 * 		RPL_LISTEND (323) : indicates the end of a LIST response.
 * 
 * 	Examples:
 * 		/LIST
 * 		/LIST -yes => "LIST" when received by server
 * 		/LIST #ubuntu
 */

void	Server::list(std::string buffer, Client c_client)
{
	std::string channel_to_display	= findAnyChannel(buffer);
	std::string client_nick 		= c_client.getNickname();
	std::string	RPL_LISTSTART		= "321 " + client_nick + " Channels: \r\n";
	std::string	RPL_LIST;
	std::string	RPL_LISTEND 		= "323 " + client_nick + " :End of /LIST\r\n";

	if (!channel_to_display.empty())
	{
		for (size_t i = 0; i < channel_vec.size(); i++) {		
			if (channel_vec[i].get_name() == channel_to_display) 
			{
				RPL_LIST = get_rpl_list(client_nick, channel_vec[i]);
				send(c_client.get_client_fd(), RPL_LIST.c_str(), RPL_LIST.size(), 0);
				return ;
			}
		}
	}
 // if no chan match to channel_vec or just "/LIST" => list all channels
	if (this->channel_vec.empty())
		send(c_client.get_client_fd(), RPL_LISTEND.c_str(), RPL_LISTEND.size(), 0);
	else 
	{
		send(c_client.get_client_fd(), RPL_LISTSTART.c_str(), RPL_LISTSTART.size(), 0);
		for (size_t i = 0; i < channel_vec.size(); i++) 
		{
			RPL_LIST.clear();
			RPL_LIST = get_rpl_list(client_nick, channel_vec[i]);
			send(c_client.get_client_fd(), RPL_LIST.c_str(), RPL_LIST.size(), 0);
		}
		send(c_client.get_client_fd(), RPL_LISTEND.c_str(), RPL_LISTEND.size(), 0);
	}
	return ;
}

static std::string	findAnyChannel(std::string buffer)
{
	std::string	channel;
	channel.clear();

	if (buffer.empty() || buffer.find("#") == buffer.npos) // Si pas d'arg ou pas de chan (#)
		return (channel);
	else
	{
		if (buffer[buffer.length() - 1] == '\n')
			buffer.erase(buffer.length() - 2);
		std::size_t pos = buffer.find(' ');
		if (pos != std::string::npos)
			channel = buffer.substr(pos + 1);
		return (channel);
	}
}

static std::string	get_rpl_list(std::string client_nick, Channel &chan)
{
	std::stringstream concat;
	concat << "322 " << client_nick << " #" << chan.get_name() << " [nb of users = " << chan.get_size() << "] "\
	<< (chan.get_description()) << "\r\n";
	return concat.str();			
}