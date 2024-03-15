/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 01:41:42 by keshikuro         #+#    #+#             */
/*   Updated: 2024/03/15 15:28:49 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
//#include "../inc/Channel.hpp"

int Server::is_command(char *buffer, Client c_client)
{
	std::string buf = buffer;
	std::string cmd_arg;
	//command with arg
	std::stringstream   sbuf(buf);
	std::getline(sbuf, cmd_arg, ' ');
	std::transform(cmd_arg.begin(), cmd_arg.end(), cmd_arg.begin(), ::toupper);

	if (cmd_arg == "/PRIVMSG")
	{
		PRIVMSG(buf, c_client);
		return 1;
	}
	else if (cmd_arg == "/WHOIS")
	{
		WHOIS(buf, c_client);
		return 1;
	}
	else if (cmd_arg == "/CREATE")
	{
		CREATE(buf, c_client);
		return 1;
	}
	else if (cmd_arg == "/NICK")
	{
		NICK(buf, c_client);
		return 1;
	}
	else if (cmd_arg == "/JOIN")
	{
		JOIN(buf, c_client);
		return 1;
	}

    else if (cmd_arg == "/MODE")
    {
        MODE(buf, c_client);
        return 1;
    }
	else if (cmd_arg == "/TOPIC")
	{
		TOPIC(buf, c_client, 1);
		return 1;
	}
	//command without arg
	std::string cmd_noarg;
	std::stringstream   okbuf(buf);
	std::getline(okbuf, cmd_noarg, '\n');
	std::transform(cmd_noarg.begin(), cmd_noarg.end(), cmd_noarg.begin(), ::toupper);

	if (cmd_noarg == "/HELP")
	{
		HELP(buf, c_client);
		return 1;
	}
	else if (cmd_noarg == "/LIST_CH")
	{
		LIST_CH(buf, c_client);
		return 1;
	}
	else if (cmd_noarg == "/TOPIC")
	{
		TOPIC(buf, c_client, 0);
		return 1;
	}
	// else if (cmd == "/REMOVE")
	// {
	// 	REMOVE(buf, c_client);
	// 	return 1;
	// }

	else if (cmd_noarg == "/LEAVE")
	{
		LEAVE(buf, c_client);
		return 1;
	}
	else if (cmd_noarg == "/SECRET_ROOT")
	{
		SECRET_ROOT(buf, c_client);
		return 1;
	}
	return 0;
}

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

void    Server::LIST_CH(std::string buffer, Client c_client) 
{
	(void)buffer;
	std::string head = "=============================================\n List of all opened channels : \n";
	send(c_client.get_client_fd(), head.c_str(), head.size(), 0);
	
	for (size_t i = 0; i < channel_vec.size(); i++)
	{
		std::string ch_line = "-#" + channel_vec[i].get_name() + "\n";
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

void    Server::REMOVE(std::string buffer, Client c_client)
{
	(void)buffer;
	(void)c_client;
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

