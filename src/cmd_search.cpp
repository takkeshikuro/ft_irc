/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_search.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorikaw <tmorikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 02:11:04 by tmorikaw          #+#    #+#             */
/*   Updated: 2024/03/19 06:03:14 by tmorikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

bool space(std::string& chaine) 
{
	std::string::const_iterator it;
	for (it = chaine.begin(); it != chaine.end(); ++it)
	{
		if (*it == ' ')
			return true;
	}
	return false;
}

int	search_cmd(std::string &cmd, int arg)
{
	if (arg)
	{
		
		std::string commands[8] = {"/PRIVMSG", "/WHOIS", "/CREATE", "/NICK", "/JOIN", "/MODE", "/TOPIC", "/INVITE"};
		for (int i = 0; i < 8; i++)
		{
			if (cmd == commands[i])
				return (i);
		}
	}
	else if (!arg)
	{
		std::string commands[6] = {"/HELP", "/LIST_CH", "/LIST_CL", "/TOPIC", "/LEAVE", "/SECRET_ROOT"};
		for (int i = 0; i < 6; i++)
		{
			if (cmd == commands[i])
				return (i);
		}
	}
	return -1;
}

int Server::is_command(char *buffer, Client c_client)
{
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
				case 0 :
					PRIVMSG(buf, c_client);
					break;
				case 1 :
					WHOIS(buf, c_client);
					break;
				case 2 :
					CREATE(buf, c_client);
					break; 
				case 3 :
					NICK(buf, c_client);
					break;
				case 4 :
					JOIN(buf, c_client);
					break;
				case 5 :
					MODE(buf, c_client);
					break; 
				case 6 :
					TOPIC(buf, c_client, 1);
					break ;
				case 7 :
					INVITE(buf, c_client);
					break ;
				case -1 :
					command_unknow(c_client);
					break ;		
			}
		}
		else		//command without argument
		{
			switch (search_cmd(cmd_string, 0))
			{
				case 0 :
					HELP(buf, c_client);
					break;
				case 1 :
					LIST_CH(buf, c_client);
					break;
				case 2 :
					LIST_CL(buf, c_client);
					break; 
				case 3 :
					TOPIC(buf, c_client, 0);
					break;
				case 4 :
					LEAVE(buf, c_client);
					break;
				case 5 :
					SECRET_ROOT(buf, c_client);
					break; 
				case -1 :
					command_unknow(c_client);
					break ;		
			}
		}
	}
	return 0;
}

int	Server::command_unknow(Client c_client)
{
	std::string cmd_unk = red + "Command unknow. (press /help).\n" + white;
	send(c_client.get_client_fd(), cmd_unk.c_str(), cmd_unk.size(), 0);
	return 1;
}