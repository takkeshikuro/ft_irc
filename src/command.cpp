/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marecarrayan <marecarrayan@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 01:41:42 by keshikuro         #+#    #+#             */
/*   Updated: 2024/03/08 01:04:35 by marecarraya      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
//#include "../inc/Channel.hpp"

int Server::is_command(char *buffer, Client c_client)
{
	std::string buf = buffer;
	std::string cmd;
	//command with arg
	std::stringstream   sbuf(buf);
	std::getline(sbuf, cmd, ' ');
	std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
	
	if (cmd == "/PRIVMSG")
	{
		PRIVMSG(buf, c_client);
		return 1;
	}
	else if (cmd == "/WHOIS")
	{
		WHOIS(buf, c_client);
		return 1;
	}
	else if (cmd == "/CREATE")
	{
		CREATE(buf, c_client);
		return 1;
	}
	else if (cmd == "/NICK")
	{
		NICK(buf, c_client);
		return 1;
	}
	else if (cmd == "/JOIN")
	{
		JOIN(buf, c_client);
		return 1;
	}
	//command without arg
	std::stringstream   okbuf(buf);
	std::getline(okbuf, cmd, '\n');
	if (cmd == "/LIST_CH")
	{
		LIST_CH(buf, c_client);
		return 1;
	}
	// else if (cmd == "/REMOVE")
	// {
	// 	REMOVE(buf, c_client);
	// 	return 1;
	// }

	// else if (cmd == "/QUIT")
	// {
	// 	QUIT(buf, c_client);
	// 	return 1;
	// }
	return 0;
}

void    Server::LIST_CH(std::string buffer, Client c_client) 
{
	(void)buffer;
	std::string head = "=============================================\n List of all the open channels : \n";
	send(c_client.get_client_fd(), head.c_str(), head.size(), 0);
	
	for (size_t i = 0; i < channel_vec.size(); i++)
	{
		std::string ch_line = "-#" + channel_vec[i].get_name() + "\n";
		send(c_client.get_client_fd(), ch_line.c_str(), ch_line.size(), 0);
		for (size_t j = 0; j < channel_vec[i].client_list.size(); j++)
		{
			std::string members = "   - " + channel_vec[i].client_list[j]->getNickname() + "\n";
			send(c_client.get_client_fd(), members.c_str(), members.size(), 0);
		}
	}
	std::string bottom = "=============================================\n";
	send(c_client.get_client_fd(), bottom.c_str(), bottom.size(), 0);
}

void    Server::JOIN(std::string buffer, Client c_client)
{
	std::string channel_name;
	std::stringstream sbuf(buffer);
	std::string cmd;
	std::getline(sbuf, cmd, ' ');
	int			exist = 0;
	size_t		i;
	size_t		j;
	while (std::getline(sbuf, channel_name, '\n'))
	{
		if (!channel_name.empty())
			break ;
	}
	for (i = 0; i < channel_vec.size(); i++)
	{
		if (channel_vec[i].get_name() == channel_name)
		{
			exist = 1;
			break ;
		}
	}
	if (!exist)
		CREATE(buffer, c_client);
	for (j = 0; j < client_vec.size(); j++)
	{
		if (client_vec[j].getNickname() == c_client.getNickname())
			break ;
	}		
	channel_vec[i].add_user(&client_vec[j]);
	client_vec[j].in_channel = 1;
	client_vec[j].set_current_channel(channel_name);
}

void    Server::QUIT(std::string buffer, Client c_client)
{	(void)buffer;
	(void)c_client;
}
void    Server::REMOVE(std::string buffer, Client c_client)
{	(void)buffer;
	(void)c_client;
}

void    Server::CREATE(std::string buffer, Client c_client)
{
	//check arg()
	std::string channel_name;
	std::stringstream sbuf(buffer);
	std::string cmd;
	std::getline(sbuf, cmd, ' ');
	
	while (std::getline(sbuf, channel_name, '\n'))
	{
		if (!channel_name.empty())
			break ;
	}
	for (size_t i = 0; i < channel_vec.size(); i++)
	{
		if (channel_vec[i].get_name() == channel_name)
		{
			std::string cannot_create = "Channel creation impossible, name already exist.\n";
			send(c_client.get_client_fd(), cannot_create.c_str(), cannot_create.size(),0);
			return;
		}
	}
	Channel new_channel(channel_name, c_client.get_client_fd());
	channel_vec.push_back(new_channel);
	std::string creation_ok = "New channel " + channel_name + "created.\n";
	send(c_client.get_client_fd(), creation_ok.c_str(), creation_ok.size(),0);
	std::cout << "creation new chan over\n";
}

void    Server::NICK(std::string buffer, Client c_client)
{
	std::stringstream	sbuf(buffer);
	std::string			new_nick;
	std::string			cmd;
	std::getline(sbuf, cmd, ' ');
	
	while (std::getline(sbuf, new_nick, '\n')) //get the msg to send
	{
		if (!new_nick.empty())
			break ;
	}
	for (size_t i = 0; i < new_nick.size(); i++)
	{
		if (new_nick[i] == ' ')
		{
			std::string error = "Error: nickname can't contain spaces.\n";
			send(c_client.get_client_fd(), error.c_str(), error.size(), 0);
			return ;
		}	
	}
	size_t	i;
	for (i = 0; i < this->client_vec.size(); i++)
	{
		if (client_vec[i].getUsername() == c_client.getUsername())
			break ;
	}
	client_vec[i].setNickname(new_nick);
	std::string	success = "\e[1;32mYour nickname has been changed to " + new_nick + ".\n\e[0m";
	send(c_client.get_client_fd(), success.c_str(), success.size(), 0);
	return ;
}

void    Server::PRIVMSG(std::string buffer, Client c_client)
{
	std::string dest_name;
	std::string msg;
	std::stringstream sbuf(buffer);
	Client  dest_cl;
	std::string cmd;
	std::getline(sbuf, cmd, ' ');

	while (std::getline(sbuf, dest_name, ' ')) //get the dest client
	{
		if (!dest_name.empty())
			break ;
	}
	while (std::getline(sbuf, msg, '\n')) //get the msg to send
	{
		if (!msg.empty())
			break ;
	}
	for (size_t i = 0; i < client_vec.size(); i++)
	{
		if (client_vec[i].getNickname() == dest_name) //get the dest_client in vec_client
		{
			dest_cl = client_vec[i];
			std::string to_send = "\e[1;35m[PRIVMSG] " + c_client.getNickname() + ": \e[0;37m" + msg + "\n";
			send(dest_cl.get_client_fd(), to_send.c_str(), to_send.size(),0);
			return ;
		}
	}

	std::string invalid_dest = "Error: no user nicknamed " + dest_name + "\n";
	send(c_client.get_client_fd(), invalid_dest.c_str(),invalid_dest.size(), 0);
}

void    Server::WHOIS(std::string buffer, Client c_client)
{
	std::string target_name;
	std::stringstream sbuf(buffer);
	Client  target_cl;
	std::string cmd;
	std::getline(sbuf, cmd, ' ');

	while (std::getline(sbuf, target_name, '\n'))
	{
		if (!target_name.empty())
			break ;
	}
	for (size_t i = 0; i < client_vec.size(); i++)
	{
		if (client_vec[i].getNickname() == target_name)
		{
			target_cl = client_vec[i];
			std::stringstream ss;
			ss << target_cl.get_client_fd();
			std::string fd = ss.str();
			std::string answer = "User " + target_name + " is :\n-username = " + target_cl.getUsername() \
			+ "\n-nickname = " + target_cl.getNickname() + "\n-user number = " + fd + "\n";
			send(c_client.get_client_fd(), answer.c_str(), answer.size(),0);
			return ;
		}
	}
	std::string invalid_target = "Error: no user nicknamed " + target_name + "\n";
	send(c_client.get_client_fd(), invalid_target.c_str(), invalid_target.size(), 0);
}
