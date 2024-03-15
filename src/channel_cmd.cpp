/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_cmd.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:17:25 by keshikuro         #+#    #+#             */
/*   Updated: 2024/03/15 17:39:09 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"


////////////////////////////////////////////////////////////////////////////////
////////////////////////// CREATE/JOIN/LEAVE ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void    Server::CREATE(std::string buffer, Client c_client)
{
	//check arg()
	std::string channel_name;
	std::stringstream sbuf(buffer);
	std::string cmd;
	std::getline(sbuf, cmd, ' ');
	
	while (std::getline(sbuf, channel_name, '\n')) {
		if (!channel_name.empty())
			break ;
	}
	for (size_t i = 0; i < channel_vec.size(); i++) {
		if (channel_vec[i].get_name() == channel_name)
		{
			std::string cannot_create = red + "Channel creation impossible, name already exist.\n" + white;
			send(c_client.get_client_fd(), cannot_create.c_str(), cannot_create.size(),0);
			return;
		}
	}
	std::cout << "-creation new chan start\n";
	Channel new_channel(channel_name, c_client.get_client_fd());
	new_channel.add_operator(c_client);
	channel_vec.push_back(new_channel);
	std::string creation_ok = green + "New channel " + channel_name + " created.\n\n" + white;
	send(c_client.get_client_fd(), creation_ok.c_str(), creation_ok.size(),0);
	std::cout << "-creation new chan over\n";
}

int	Server::asking_to_create(std::string buffer, Client c_client)
{
	std::string y_n = "[y/n] : ";
	std::string ask = "This channel don't exist yet. Do you want to create it ? [y/n] : ";
	send(c_client.get_client_fd(), ask.c_str(), ask.size(), 0);
	while (1)
	{
		char buff[1024];
		int byte = read(c_client.get_client_fd(), buff, sizeof(buff));
		if (byte > 0) 
		{
			buff[byte] = '\0';
			std::string answer(buff);
			if (answer.size() == 2 && answer[0] == 'y') {
				CREATE(buffer, c_client);
				return 1;
			}
			else if (answer.size() == 2 && answer[0] == 'n')
				return 0;
			else
				send(c_client.get_client_fd(), y_n.c_str(), y_n.size(), 0);
		}
		else if (byte == 0) {
			std::cerr << "Connexion fermée par le client" << std::endl;
			break ;
		}
	}
	return 0;
}

int is_in_channel(Client c_client, Channel chan)
{
	for (size_t i = 0; i < chan.client_list.size(); i++)
	{
		if (chan.client_list[i].getUsername() == c_client.getUsername())
			return 1;
	}
	return 0;
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
	while (std::getline(sbuf, channel_name, '\n')) {
		if (!channel_name.empty())
			break ;
	}
	for (i = 0; i < channel_vec.size(); i++) {
		if (channel_vec[i].get_name() == channel_name) {
			exist = 1;
			break ;
		}
	}
	if (!exist && (asking_to_create(buffer, c_client) == 0)) 
		return ;
	for (j = 0; j < client_vec.size(); j++) {
		if (client_vec[j].getNickname() == c_client.getNickname())
			break ;
	}
	if (!is_in_channel(c_client, channel_vec[i]))
	{
		if (channel_vec[i].check_keypass(c_client) == true)
		{
			channel_vec[i].add_user(client_vec[j]);
			client_vec[j].in_channel += 1;
			
			
			std::string line = "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
			send(c_client.get_client_fd(), line.c_str(), line.size(), 0);
			send(c_client.get_client_fd(), line.c_str(), line.size(), 0);
			std::string welcome_channel = "\n# Welcome to " + channel_name + " channel !\n";
			send(c_client.get_client_fd(), welcome_channel.c_str(), welcome_channel.size(), 0);

		}
		else
			return ;
	}
	else
	{
		std::string active_channel = "# You are currently in " + channel_name + " channel.\n";
		send(c_client.get_client_fd(), active_channel.c_str(), active_channel.size(), 0);
	}
	client_vec[j].set_current_channel(channel_name);
}


void    Server::LEAVE(std::string buffer, Client c_client)
{	
	(void)buffer;
	size_t i;
	if (c_client.in_channel)
	{	
		for (i = 0; i < channel_vec.size(); i++)
		{
			if (channel_vec[i].get_name() == c_client.get_current_chan()) 
			{
				size_t j;
				for (j = 0; j < client_vec.size(); j++) 
				{
					if (client_vec[j].get_client_fd() == c_client.get_client_fd()) 
					{
						channel_vec[i].rm_user(client_vec[j]);
						client_vec[j].in_channel -= 1;
						client_vec[j].set_current_channel("default");
						std::string quit = yellow + "You have left the channel #" + channel_vec[i].get_name() + "\n" + white;
						send(c_client.get_client_fd(), quit.c_str(), quit.size(), 0);
						if (channel_vec[i].client_list.size() == 0 
							&& channel_vec[i].get_name() != "random" && channel_vec[i].get_name() != "announcements")
						{
							std::cout << RED << "Channel #" << channel_vec[i].get_name() << " has been deleted.\n";
							channel_vec.erase(channel_vec.begin() + i);
						}
						return ;
					}
				}
			}
		}
	}
	std::string quit_failed = red + "You are not in a channel\n" + white;
	send(c_client.get_client_fd(), quit_failed.c_str(), quit_failed.size(), 0);
}