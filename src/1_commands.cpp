/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_commands.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorikaw <tmorikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 02:13:54 by tmorikaw          #+#    #+#             */
/*   Updated: 2024/03/19 02:21:23 by tmorikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

// 1 - topic
// 2 - priv_msg
// 3 - whois


void    Server::TOPIC(std::string buffer, Client c_client, int arg)
{
	if (!arg) 
	{
		std::string bio; 
		if (c_client.in_channel) // faut chercher si il est dans le boon chan
		{
			size_t i;
			for (i = 0; i < channel_vec.size(); i++) {
				if (channel_vec[i].get_name() == c_client.get_current_chan()) 
				{
					bio = yellow + "[" + channel_vec[i].get_description() + "]\n" + white;
					break;
				}
				bio = red + "! no topic found.\n" + white;
			}	
		}
		else {
			std::string out_of_ch = red + "try /topic into a channel.\n" + white;
			send(c_client.get_client_fd(), out_of_ch.c_str(), out_of_ch.size(), 0);
			return ;
		}
		send(c_client.get_client_fd(), bio.c_str(), bio.size(), 0);
		return ;
	}
	std::stringstream	sbuf(buffer);
	std::string			new_bio;
	std::string			cmd;
	std::getline(sbuf, cmd, ' ');
	
	while (std::getline(sbuf, new_bio, '\n')) {
		if (!new_bio.empty())
			break ;
	}
	std::string bio; 
	if (c_client.in_channel) // need add check_permission()
	{
		size_t i;
		for (i = 0; i < channel_vec.size(); i++) {
			if (channel_vec[i].get_name() == c_client.get_current_chan()) 
			{
				channel_vec[i].set_description(new_bio);
				std::string save = green + "change saved.\n" + white;
				send(c_client.get_client_fd(), save.c_str(), save.size(), 0);
				return ;
			}
		}
	}
	std::string cannot = red + "you don't have permission\n" + white;
	send(c_client.get_client_fd(), cannot.c_str(), cannot.size(), 0);	
} 

void    Server::PRIVMSG(std::string buffer, Client c_client)
{
	std::string dest_name;
	std::string msg;
	std::stringstream sbuf(buffer);
	Client  dest_cl;
	std::string cmd;
	std::getline(sbuf, cmd, ' ');

	while (std::getline(sbuf, dest_name, ' ')) {
		if (!dest_name.empty())
			break ;
	}
	while (std::getline(sbuf, msg, '\n')) {
		if (!msg.empty())
			break ;
	}
	for (size_t i = 0; i < client_vec.size(); i++) {
		if (client_vec[i].getNickname() == dest_name) {
			dest_cl = client_vec[i];
			std::string to_send = "\e[1;35m[PRIVMSG] " + c_client.getNickname() + ": \e[0;37m" + msg + "\n";
			send(dest_cl.get_client_fd(), to_send.c_str(), to_send.size(),0);
			return ;
		}
	}

	std::string invalid_dest = red + "Error: no user nicknamed " + dest_name + "\n" + white;
	send(c_client.get_client_fd(), invalid_dest.c_str(),invalid_dest.size(), 0);
}

void    Server::WHOIS(std::string buffer, Client c_client)
{
	std::string target_name;
	std::stringstream sbuf(buffer);
	Client  target_cl;
	std::string cmd;
	std::getline(sbuf, cmd, ' ');

	while (std::getline(sbuf, target_name, '\n')) {
		if (!target_name.empty())
			break ;
	}
	for (size_t i = 0; i < client_vec.size(); i++) {
		if (client_vec[i].getNickname() == target_name) {
			target_cl = client_vec[i];
			std::stringstream ss;
			ss << target_cl.get_client_fd();
			std::string fd = ss.str();
			std::string answer = yellow + "User " + target_name + " is :\n-username = " + target_cl.getUsername() \
			+ "\n-nickname = " + target_cl.getNickname() + "\n-user number = " + fd + "\n" + white;
			send(c_client.get_client_fd(), answer.c_str(), answer.size(),0);
			return ;
		}
	}
	std::string invalid_target = red + "Error: no user nicknamed " + target_name + "\n" + white;
	send(c_client.get_client_fd(), invalid_target.c_str(), invalid_target.size(), 0);
}
