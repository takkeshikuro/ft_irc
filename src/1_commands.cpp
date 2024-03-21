/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_commands.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marecarrayan <marecarrayan@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 02:13:54 by tmorikaw          #+#    #+#             */
/*   Updated: 2024/03/21 01:05:37 by marecarraya      ###   ########.fr       */
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

void    Server::KICK(std::string buffer, Client c_client)
{
            //      /KICK      #channel      nickname
            //      args[0]      args[1]      args[2]
    
    std::string args[3];
	std::stringstream sbuf(buffer);
	std::getline(sbuf, args[0], ' ');

    while (std::getline(sbuf, args[1], ' '))
    {
        if (!args[1].empty())
            break ;
    }
    while (std::getline(sbuf, args[2], '\n'))
    {
        if (!args[2].empty())
            break ;
    }

    size_t i;
    for (i = 0; i < channel_vec.size(); i++)
    {
        if (channel_vec[i].get_name() == args[1])
            break ;
    }
    if (i == channel_vec.size()) //NOT A CHANNEL
    {
        std::string to_send = "\e[1;31mError: " + args[1] + " is not a channel.\n" + RESET;
        send(c_client.get_client_fd(), to_send.c_str(), to_send.size(), 0);
        return ;
    }
    int index;
    index = index_operator(c_client, channel_vec[i]);
    if (index == -1) //NOT AN OPERATOR
    {
        std::string to_send = "\e[1;31mYou do not have permission to use /KICK in #" + args[1] + ".\n" + RESET;
        send(c_client.get_client_fd(), to_send.c_str(), to_send.size(), 0);
        return ;
    }
    index = index_channel_nick(args[2], channel_vec[i]);
    if (index == -1) //target not in channel
    {
        std::string to_send = "\e[1;31mError: no user named " + args[2] + " in #" + args[1] + ".\n" + RESET;
        send(c_client.get_client_fd(), to_send.c_str(), to_send.size(), 0);
        return ;
    }
    std::string bye = "\e[1;31mYou've been kicked from \e[1;34m#" + args[1] + RESET + ".\n";
    send(channel_vec[i].client_list[index].get_client_fd(), bye.c_str(), bye.size(), 0);
    channel_vec[i].client_list.erase(channel_vec[i].client_list.begin() + index);
    std::string to_send = YEL + c_client.getNickname() 
                + "\e[1;31m has kicked " + YEL + args[2] 
                + "\e[1;31m from \e[1;34m#" + args[2] + RESET + ".\n";
    channel_vec[i].send_string(to_send);
}