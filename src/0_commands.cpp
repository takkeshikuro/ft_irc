/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_commands.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorikaw <tmorikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 01:41:42 by keshikuro         #+#    #+#             */
/*   Updated: 2024/03/19 02:21:59 by tmorikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

// 1 - help
// 2 - list_ch
// 3 - list_cl
// 4 - nick
// 5 - secret_root

void    Server::HELP(std::string buffer, Client c_client)
{
	(void)buffer;
	std::string helper = "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"; 
	std::string helper1 = "| -/list_ch #to see all the channels                          |\n";
	std::string helper2 = "| -/create + <channel_name> #to create a new channels         |\n";
	std::string helper3 = "| -/join + <channel_name> #to join a new channel              |\n";
	std::string helper4 = "| -/nick + <new_name> #to rename (nickname)                   |\n";
	std::string helper5 = "| -/whois + <user_nickname> #to get info about an user        |\n";
	std::string helper6 = "| -/privmsg + <user_nickname> #to dm an user (private)        |\n";
	std::string helper7 = "| -/leave #to leave a channel                                 |\n";
	std::string helper8 = "| -/topic #to display channel's topic                         |\n";
	std::string helper9 = "| -/topic + <new topic> #to update channel's topic            |\n";
	std::string helper10 = "| -/mode + <channel> + option #for operators option           |\n";

	send(c_client.get_client_fd(), helper.c_str(), helper.size(), 0);
	send(c_client.get_client_fd(), helper1.c_str(), helper1.size(), 0);
	send(c_client.get_client_fd(), helper2.c_str(), helper2.size(), 0);
	send(c_client.get_client_fd(), helper3.c_str(), helper3.size(), 0);
	send(c_client.get_client_fd(), helper4.c_str(), helper4.size(), 0);
	send(c_client.get_client_fd(), helper5.c_str(), helper5.size(), 0);
	send(c_client.get_client_fd(), helper6.c_str(), helper6.size(), 0);
	send(c_client.get_client_fd(), helper7.c_str(), helper7.size(), 0);
	send(c_client.get_client_fd(), helper8.c_str(), helper8.size(), 0);
	send(c_client.get_client_fd(), helper9.c_str(), helper9.size(), 0);
	send(c_client.get_client_fd(), helper10.c_str(), helper10.size(), 0);
	send(c_client.get_client_fd(), helper.c_str(), helper.size(), 0);
	return;
}

void    Server::LIST_CH(std::string buffer, Client c_client) 
{
	(void)buffer;
	std::string head = "=============================================\n List of all opened channels : \n";
	send(c_client.get_client_fd(), head.c_str(), head.size(), 0);
	
	for (size_t i = 0; i < channel_vec.size(); i++)
	{
		std::string ch_line;
		if (channel_vec[i].get_key_set() == true)
			ch_line = "-#" + channel_vec[i].get_name() + " (keypass lock)\n";
		else
			ch_line = "-#" + channel_vec[i].get_name() + "\n";
		send(c_client.get_client_fd(), ch_line.c_str(), ch_line.size(), 0);
		for (size_t j = 0; j < channel_vec[i].client_list.size(); j++)
		{
			std::string members = "   - " + channel_vec[i].client_list[j].getNickname() + "\n";
			send(c_client.get_client_fd(), members.c_str(), members.size(), 0);
		}
	}
	std::string bottom = "=============================================\n";
	send(c_client.get_client_fd(), bottom.c_str(), bottom.size(), 0);
}

void    Server::LIST_CL(std::string buffer, Client c_client) 
{
	(void)buffer;
	std::string head = "=============================================\n List of all clients in server : \n";
	send(c_client.get_client_fd(), head.c_str(), head.size(), 0);
	
	for (size_t i = 0; i < client_vec.size(); i++)
	{
		std::string cl_line = "-#" + client_vec[i].getNickname() + "\n";
		send(c_client.get_client_fd(), cl_line.c_str(), cl_line.size(), 0);
	}
	std::string bottom = "=============================================\n";
	send(c_client.get_client_fd(), bottom.c_str(), bottom.size(), 0);
}

void    Server::NICK(std::string buffer, Client c_client)
{
	std::stringstream	sbuf(buffer);
	std::string			new_nick;
	std::string			cmd;
	std::getline(sbuf, cmd, ' ');
	
	while (std::getline(sbuf, new_nick, '\n')) {
		if (!new_nick.empty())
			break ;
	}
	if (new_nick.empty()) {
		std::string err = red + "Error: empty new nickname.\n" + white;
		send(c_client.get_client_fd(), err.c_str(), err.size(), 0);
		return ;
	}
	for (size_t i = 0; i < new_nick.size(); i++) {
		if (new_nick[i] == ' ') {
			std::string error = red + "Error: nickname can't contain spaces.\n" + white;
			send(c_client.get_client_fd(), error.c_str(), error.size(), 0);
			return ;
		}	
	}
	size_t	i;
	for (i = 0; i < this->client_vec.size(); i++) {
		if (client_vec[i].getUsername() == c_client.getUsername())
			break ;
	}
	if (c_client.in_channel)
	{
		for (size_t j = 0; j < channel_vec.size(); j++) {
			if (c_client.get_current_chan() == channel_vec[j].get_name()) {
				for (size_t k = 0; k < channel_vec[j].client_list.size(); k++) {
					if (channel_vec[j].client_list[k].getUsername() == c_client.getUsername())
						channel_vec[j].client_list[k].setNickname(new_nick);
				}
			}
		}
	}
	client_vec[i].setNickname(new_nick);
	std::string	success = "\e[1;32mYour nickname has been changed to " + new_nick + ".\n\e[0m";
	send(c_client.get_client_fd(), success.c_str(), success.size(), 0);
	return ;
}

void	Server::SECRET_ROOT(std::string buffer, Client c_client)
{
	(void)buffer;
	std::string root_password = "doumpied\n";
	std::string root = "please give root password to have root's permission : ";
	send(c_client.get_client_fd(), root.c_str(), root.size(),0);
	while (1) 
	{
		char buff[1024];
		int byte = read(c_client.get_client_fd(), buff, sizeof(buff));
		if (byte > 0) 
		{
			buff[byte] = '\0';
			std::string pw(buff);
			if (pw == root_password) 
			{	
				for (size_t i = 0; i < client_vec.size(); i++)  {
					if (client_vec[i].getNickname() == c_client.getNickname())
						client_vec[i].set_admin_perm();
					std::string good = yellow + "You are know admin." + white + "\n";
					send(c_client.get_client_fd(), good.c_str(), good.size(), 0);				
					return;
				}	
			}
			else {
				std::string bad = red + "bad password.\n" + white;
				send(c_client.get_client_fd(), bad.c_str(), bad.size(), 0);
				return ;
			}
		}
		else if (byte == 0) {
			std::cerr << "Connexion fermée par le client" << std::endl;
			break ;
		}
	}
}

