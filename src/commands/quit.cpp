#include "../../inc/Server.hpp"

void	send_to_all(Client c_client, Channel &chan, std::string reason, std::string id);

/**
 * @brief The QUIT command is used to terminate a client’s connection to the server. 
 *  The server acknowledges this by replying with an ERROR message and closing 
 *  the connection to the client.
 * 
 * 	When connections are terminated by a client-sent QUIT command, 
 *  servers SHOULD prepend <reason> with the ASCII string "Quit: " when sending 
 *  QUIT messages to other clients, to represent that this user terminated the 
 *  connection themselves. This applies even if <reason> is empty, in which case 
 *  the reason sent to other clients SHOULD be just this "Quit: " string. 
 * 	
 *  Parameters: [<reason>]
 * 
 *  Example : [CLIENT] QUIT :Gone to have lunch   
 * 			  [SERV] ;Client exiting from the network
 * 
 */

void    Server::quit(std::string buffer, Client c_client)
{
	
	std::string client_nickname = c_client.getNickname();
    std::string reason;

	if (buffer[buffer.length() - 1] == '\n')
		buffer.erase(buffer.length() - 1);	
	std::size_t pos = buffer.find(" ");
	if (pos != std::string::npos)
		reason = buffer.substr(pos + 1);
	else
		reason.clear();

	std::string id = ":" + client_nickname + "!" + c_client.getUsername() + "@localhost";
	std::string quit_msg = red + "[info] You will be disconnected. Goodbye.\n" + white;
	if (c_client.get_is_irssi() == true)
		send(c_client.get_client_fd(), RPL_QUIT(id, reason).c_str(), RPL_QUIT(id, reason).size(), 0);
	else
		send(c_client.get_client_fd(), quit_msg.c_str(), quit_msg.size(), 0);	
	for (size_t i = 0; i < this->channel_vec.size(); ++i)
	{
		for (size_t j = 0; j < channel_vec[i].client_list.size(); ++j)
		{
			if (client_nickname == channel_vec[i].client_list[j].getNickname())
			{
				
				int index = index_client_vec(client_nickname, this->client_vec);
				channel_vec[i].rm_user(client_vec[index]);
				if (index_operator_nick(client_nickname, channel_vec[i]) != -1)
					channel_vec[i].rm_operator(client_vec[index]);
				send_to_all(c_client, channel_vec[i], reason, id);
				break ;
			}
		}
	}
	
	size_t i;
	for (i = 0; i < client_vec.size(); ++i) {
		if (client_nickname == client_vec[i].getNickname())
			break;
	}
	client_vec[i].set_quit_status(true);
	handlePolloutEvent(c_client.get_client_fd());
}


void	send_to_all(Client c_client, Channel &chan, std::string reason, std::string id)
{
	std::string nick = c_client.getNickname();
	std::string user = c_client.getUsername();
	std::string y = "\e[1;33m";
	std::string w = "\e[0;37m";	
	std::string share_quit;
	if (reason.empty())
		share_quit = y + "[info] " + nick + " [" + user + "@localhost] has quit the server [leaving]\n" + w;
	else
		share_quit = y + "[info] " + nick + " [" + user + "@localhost] has quit the server [" + reason + "]\n" + w;

	for (size_t i = 0; i < chan.client_list.size(); i++) 
	{
		if (chan.client_list[i].get_is_irssi() == true)
			send(chan.client_list[i].get_client_fd(), RPL_QUIT(id, reason).c_str(), RPL_QUIT(id, reason).size(), 0);
		else
			send(chan.client_list[i].get_client_fd(), share_quit.c_str(), share_quit.size(), 0);

	}
}
