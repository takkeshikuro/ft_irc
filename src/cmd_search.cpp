/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_search.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 02:11:04 by tmorikaw          #+#    #+#             */
/*   Updated: 2024/04/23 18:43:26 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

bool space(std::string& chaine);

/////////////////////// IRSSI COMMANDS ////////////////////////////////////////////

int	search_irssi_cmd(std::string &cmd)
{
	std::string commands[11] = {"PING", "NICK", "LIST", "JOIN", \
								"PRIVMSG", "TOPIC", "USER", "MODE", \
								"PART", "KICK", "INVITE"};
	for (int i = 0; i < 11; i++) {
		if (cmd == commands[i])
			return (i);
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
	
	if (space(cmd_string) == true) {
		size_t index_space = cmd_string.find(' ');
		cmd_string = cmd_string.substr(0, index_space);
	}
	switch (search_irssi_cmd(cmd_string))
	{
		case 0 : ping(buffer, c_client); break;
		case 1 : nick(buffer, c_client); break;
		case 2 : list(buffer, c_client); break;
		case 3 : join(buffer, c_client); break;
		case 4 : msg(buffer, c_client); break;
		case 5 : topic(buffer, c_client); break;
		case 6 : user(buffer, c_client); break;
		case 7 : mode(buffer, c_client); break;
		case 8 : part(buffer, c_client); break;
		case 9 : kick(buffer, c_client); break;
		case 10 : invite(buffer, c_client); break;
		case -1 : command_unknow(c_client, cmd_string); break ;		
	}
	return 0;
}


/////////////////////// NETCAT COMMANDS /////////////////////////////////////////////

int	search_cmd(std::string &cmd) 
{
	std::string commands[13] = {"/PRIVMSG", "/NICK", "/JOIN", "/MODE", \
								"/TOPIC", "/INVITE", "/KICK", "/PART", \
								"/LIST_CH", "/LIST_CL", "/HELP" \
								"/HELP_OPERATOR", "/CREATE"};
	for (int i = 0; i < 13; i++) {
		if (cmd == commands[i])
			return (i);
	}

	return -1;
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
		if (space(cmd_string) == true) { // check arg
			size_t index_space = cmd_string.find(' ');
			cmd_string = cmd_string.substr(0, index_space);
		}
		switch (search_cmd(cmd_string))
		{
			case 0 : msg(buf, c_client);  return 1;
			case 1 : nick(buf, c_client);  return 1;
			case 2 : join(buf, c_client);  return 1;
			case 3 : mode(buf, c_client);  return 1; 
			case 4 : topic(buf, c_client);  return 1 ;
			case 5 : invite(buf, c_client);  return 1 ;
			case 6 : KICK(buf, c_client);  return 1 ;		//kick nc
			case 7 : part(buffer, c_client); break;
			case 8 : LIST_CH(buf, c_client);  return 1;
			case 9 : LIST_CL(buf, c_client);  return 1;
			case 10 : HELP(buf, c_client);  return 1;
			case 11 : HELP_OPERATOR(buf, c_client);  return 1;
			case 12 : CREATE(buf, c_client);  return 1;
			case -1 : command_unknow(c_client, cmd_string); return 0 ;
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

bool space(std::string& chaine)  {
	std::string::const_iterator it;
	for (it = chaine.begin(); it != chaine.end(); ++it) {
		if (*it == ' ')
			return true;
	}
	return false;
}