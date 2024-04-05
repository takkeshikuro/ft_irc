/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_search.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 02:11:04 by tmorikaw          #+#    #+#             */
/*   Updated: 2024/04/04 06:28:29 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

bool space(std::string& chaine)  {
	std::string::const_iterator it;
	for (it = chaine.begin(); it != chaine.end(); ++it) {
		if (*it == ' ')
			return true;
	}
	return false;
}

int	search_cmd(std::string &cmd, int arg) 
{
	if (arg) {
		std::string commands[9] = {"/PRIVMSG", "/WHOIS", "/CREATE", "/NICK", "/JOIN", "/MODE", "/TOPIC", "/INVITE", "/KICK"};
		for (int i = 0; i < 9; i++) {
			if (cmd == commands[i])
				return (i);
		}
	}
	else if (!arg) {
		std::string commands[7] = {"/HELP", "/LIST_CH", "/LIST_CL", "/TOPIC", "/LEAVE", "/SECRET_ROOT", "/HELP_OPERATOR"};
		for (int i = 0; i < 7; i++) {
			if (cmd == commands[i])
				return (i);
		}
	}
	return -1;
}

int	search_irssi_cmd(std::string &cmd, int arg)
{
	if (!arg) {
		std::string commands[1] = {"LIST"};
		for (int i = 0; i < 1; i++) {
			if (cmd == commands[i])
				return (i);
		}
	}
	else {
		std::string commands[7] = {"PING", "NICK", "LIST", "JOIN", "PRIVMSG", "TOPIC", "USER"};
		for (int i = 0; i < 7; i++) {
			if (cmd == commands[i])
				return (i);
		}
	}
	return -1;
}

int Server::is_irssi_command(char *buffer, Client c_client)
{
	std::string			buf = buffer;
	std::string			cmd_string;
	std::stringstream	sbuf(buf);
	
	std::getline(sbuf, cmd_string, '\n');
	std::transform(cmd_string.begin(), cmd_string.end(), cmd_string.begin(), ::toupper);
	if (space(cmd_string) == true) // command with arguments
	{
		size_t index_space = cmd_string.find(' ');
		std::string cmd_arg = cmd_string.substr(0, index_space);
		switch (search_irssi_cmd(cmd_arg, 1))
		{
			case 0 : PING(buffer, c_client); break;
			case 1 : nick(buffer, c_client); break;
			case 2 : list(buffer, c_client); break;
			case 3 : join(buffer, c_client); break;
			case 4 : msg(buffer, c_client); break;
			case 5 : topic(buffer, c_client); break;
			case 6 : user(buffer, c_client); break;
			case -1 : command_unknow(c_client, cmd_arg); break ;		
		}
	}
	else		//command without argument
	{
		switch (search_irssi_cmd(cmd_string, 0))
		{
			case 0 : list(buffer, c_client); break;
			case -1 : command_unknow(c_client, cmd_string); break ;		
		}
	}
	return 0;
}

int Server::is_command(char *buffer, Client c_client)
{
	if (c_client.get_is_irssi() == true)
		return 0;
	std::string			buf = buffer;
	std::string			cmd_string;
	std::stringstream	sbuf(buf);
	
	std::getline(sbuf, cmd_string, '\n');
	std::transform(cmd_string.begin(), cmd_string.end(), cmd_string.begin(), ::toupper);
	if (cmd_string[0] == '/')
	{
		if (space(cmd_string) == true) // command with arguments
		{
			size_t index_space = cmd_string.find(' ');
			std::string cmd_arg = cmd_string.substr(0, index_space);
			switch (search_cmd(cmd_arg, 1))
			{
				case 0 : PRIVMSG(buf, c_client);  return 1;
				case 1 : WHOIS(buf, c_client);  return 1;
				case 2 : CREATE(buf, c_client);  return 1; 
				case 3 : NICK(buf, c_client);  return 1;
				case 4 : join(buf, c_client);  return 1;
				case 5 : MODE(buf, c_client);  return 1; 
				case 6 : TOPIC(buf, c_client, 1);  return 1 ;
				case 7 : INVITE(buf, c_client);  return 1 ;
                case 8 : KICK(buf, c_client);  return 1 ;
				case -1 : command_unknow(c_client, cmd_arg); return 0 ;
			}
		}
		else		//command without argument
		{
			switch (search_cmd(cmd_string, 0))
			{
				case 0 : HELP(buf, c_client);  return 1;
				case 1 : LIST_CH(buf, c_client);  return 1;
				case 2 : LIST_CL(buf, c_client);  return 1; 
				case 3 : TOPIC(buf, c_client, 0);  return 1;
				case 4 : LEAVE(buf, c_client);  return 1;
				case 5 : SECRET_ROOT(buf, c_client);  return 1;
				case 6 : HELP_OPERATOR(buf, c_client);  return 1; 
				case -1 : command_unknow(c_client, cmd_string); return 1;		
			}
		}
	}
	return 0;
}

void	Server::command_unknow(Client c_client, std::string cmd)
{
	std::cout << "[unk command]\n";
	std::string cmd_unk = red + ERR_UNKNOWNCOMMAND(c_client.getNickname(), cmd) + white;
	send(c_client.get_client_fd(), cmd_unk.c_str(), cmd_unk.size(), 0);
}