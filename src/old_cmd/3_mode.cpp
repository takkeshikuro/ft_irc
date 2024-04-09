/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_mode.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorikaw <tmorikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 21:41:39 by marecarraya       #+#    #+#             */
/*   Updated: 2024/03/19 06:13:57 by tmorikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"


// 1 - +o/-o
// 2 - -l
// 3 - +k/-k
// 4 - checking mode

void    Server::MODE_oprt(Channel &chan, std::string args[3], Client c_client)
{
	size_t i;
	size_t j;
	(void)c_client;
	for (j = 0; j < client_vec.size(); j++)
	{
		if (client_vec[j].getUsername() == args[2])
			break ;
	}
	for (i = 0; i < chan.op_clients.size(); i++)
	{
		if (chan.client_list[i].getNickname() == args[2])
			break ;
	}
	if (i == chan.op_clients.size())
	{
		std::string err = "Error: " + args[2] + " is not in channel " + args[0];
	}
	if (args[1] == "+o")
		chan.add_operator(client_vec[j]);
	if (args[1] == "-o")
		chan.rm_operator(client_vec[j]);
}

void    Server::MODE_limit(Channel &chan, std::string args[3], Client c_client)
{
	// /MODE #channel +l value
	for (size_t i = 0; i < args[2].size(); i++)
	{
		if (!isdigit(args[2][i]) && args[2][i] != '\n' && args[2][i] != ' ')
		{
			std::string invalid = "\e[1;31mError: Invalid limit value (" + args[2] + ")\n" + RESET;
			send(c_client.get_client_fd(), invalid.c_str(), invalid.size(), 0);
			return ;
		}
	}

	if (args[1] == "-l" || args[1] == "-l\n")
	{
		chan.set_limit(-1);
		std::string tosend = YEL + c_client.getNickname() 
			+ GRE + " has unset users limit in " + BLU + chan.get_name() + RESET + "\n";
		chan.send_string_all(tosend);
		std::cout << tosend;
	}
	else
	{
		if (chan.get_size() > std::atoi(args[2].c_str()))
		{
			std::string errr = BLU + chan.get_name() + RED + " currently has more than " 
				+  args[2] + " users. Can't set limit to this value.\n" + RESET;
			send(c_client.get_client_fd(), errr.c_str(), errr.size(), 0);
		}
		else
		{
			chan.set_limit(std::atoi(args[2].c_str()));
			std::string success = GRE + chan.get_name() 
				+ " channel now have a limit of " + args[2] + " users\n" + RESET;
			chan.send_string_all(success);
		}
	}
}

void	Server::MODE_keypass_add(Channel &chan, Client c_client)
{
	std::string y_n = "[y/n] : ";
	if (chan.get_key_set() == true)
	{
		std::string key_set = "Keypass is already set (" + chan.get_keypass() + ")\n";
		send(c_client.get_client_fd(), key_set.c_str(), key_set.size(), 0);
		std::string ask = "Do you want to modify it ? [y/n] : ";
		send(c_client.get_client_fd(), ask.c_str(), ask.size(), 0);
		while (1) {
			char buff[1024];
			int byte = read(c_client.get_client_fd(), buff, sizeof(buff));
			if (byte > 0) {
				buff[byte] = '\0';
				std::string answer(buff);
				if (answer.size() == 2 && answer[0] == 'y')
					return (chan.set_keypass(c_client));
				else if (answer.size() == 2 && answer[0] == 'n')
					return ;
				else
					send(c_client.get_client_fd(), y_n.c_str(), y_n.size(), 0);
			}
			else if (byte == 0) {
				std::cerr << "Connexion fermée par le client" << std::endl;
				break ;
			}
		}
	}
	else
	{
		std::string key_set = yellow + "This channel has no keypass yet.\n";
		send(c_client.get_client_fd(), key_set.c_str(), key_set.size(), 0);
		std::string ask = "Do you want to set it ? [y/n] : " + white;
		send(c_client.get_client_fd(), ask.c_str(), ask.size(), 0);
		while (1) {
			char buff[1024];
			int byte = read(c_client.get_client_fd(), buff, sizeof(buff));
			if (byte > 0) {
				buff[byte] = '\0';
				std::string answer(buff);
				if (answer.size() == 2 && answer[0] == 'y') 
					return (chan.set_keypass(c_client));
				else if (answer.size() == 2 && answer[0] == 'n')
					return ;
				else
					send(c_client.get_client_fd(), y_n.c_str(), y_n.size(), 0);
			}
			else if (byte == 0) {
				std::cerr << "Connexion fermée par le client" << std::endl;
				break ;
			}
		}
	}
}

void	Server::MODE_keypass_rm(Channel &chan, Client c_client)
{
	if (chan.get_key_set() == true)
	{
		std::string remove = "You just remove channel's keypass.\n";
		send(c_client.get_client_fd(), remove.c_str(), remove.size(), 0);
		for (size_t i = 0; i < this->channel_vec.size(); i++) {
			if (chan.get_name() == channel_vec[i].get_name())
				channel_vec[i].set_key_set();
		}
		return ;
	}
	std::string no_kp = "This channel has no keypass, press +k to add one.\n";
	send(c_client.get_client_fd(), no_kp.c_str(), no_kp.size(), 0);
}

void	Server::MODE_invite(Channel &chan, Client c_client)
{
	if (chan.get_invite_set() == false)
	{
		std::string invite = yellow + "This channel is know in invite mode only.\n" + white;
		send(c_client.get_client_fd(), invite.c_str(), invite.size(), 0);
		chan.set_invite_set();
	}
	else
	{
		std::string invite = yellow + "This channel is know in normal access mode.\n" + white;
		send(c_client.get_client_fd(), invite.c_str(), invite.size(), 0);
		chan.set_invite_set();
	}
}

void Server::check_MODE_args(std::string args[3], Client c_client)
{
	size_t  i;
	size_t  j;
	std::string ops;
	
	for (i = 0; i < channel_vec.size(); i++) {                  //check if channel exists
		if (args[0] == channel_vec[i].get_name() 
			|| args[0] == (channel_vec[i].get_name() + "\n"))
			break ;
	}
	if (i == channel_vec.size()) {
		std::string not_in_chan = red + "Error: channel #" + args[0] + " does not exists\n" + white;
		send(c_client.get_client_fd(), not_in_chan.c_str(), not_in_chan.size(), 0);
		return ;
	}
	for (j = 0; j < channel_vec[i].op_clients.size(); j++) {
		if (c_client.getUsername() == channel_vec[i].op_clients[j].getUsername())
			break ;
	}
	if (j == channel_vec[i].op_clients.size())    //check if user is an operator in the active channel
	{
		std::string str = red + "You do not have the permissions to use /MODE in #"
						+ channel_vec[i].get_name() + "\n" + white;
		send(c_client.get_client_fd(), str.c_str(), str.size(), 0);
		return ;
	}
	if (args[1] == "-o" || args[1] == "+o")
		MODE_oprt(channel_vec[i], args, c_client);
	else if (args[1] == "+k" || args[1] == "+k\n") 
		MODE_keypass_add(channel_vec[i], c_client);
	else if (args[1] == "-k" || args[1] == "-k\n") 
		MODE_keypass_rm(channel_vec[i], c_client);
	else if (args[1] == "-l" || args[1] == "+l" || args[1] == "-l\n")
		MODE_limit(channel_vec[i], args, c_client);
	else if (args[1] == "+i" || args[1] == "+i\n" || args[1] == "-i" || args[1] == "-i\n") 
		MODE_invite(channel_vec[i], c_client);
	else if (args[1].empty())
	{
		for (size_t k = 0; k < channel_vec[i].op_clients.size(); k++)
			ops += channel_vec[i].op_clients[k].getNickname() + "\n";
		std::string ops_list = "Operators in #" + channel_vec[i].get_name() + ":\n" + ops;
		send(c_client.get_client_fd(), ops_list.c_str(), ops_list.size(), 0);
	}
}

void    Server::MODE(std::string buffer, Client c_client)
{
	(void) c_client;
	std::stringstream   sbuf(buffer);
	std::string cmd;
	std::string args[3];
	std::getline(sbuf, cmd, ' ');
/*  parsing of /MODE cmd
	par exemple : /MODE      channel        -o        username
				   cmd       args[0]      args[1]      args[2]
*/
	while (std::getline(sbuf, args[0], ' ')) {
		if (!args[0].empty())
			break;
	}
	while (std::getline(sbuf, args[1], ' ')) {
		if (!args[1].empty())
			break;
	}
	while (std::getline(sbuf, args[2], '\n')) {
		if (!args[2].empty())
			break;
	}
	check_MODE_args(args, c_client);
}