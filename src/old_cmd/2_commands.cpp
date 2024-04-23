/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_commands.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:17:25 by keshikuro         #+#    #+#             */
/*   Updated: 2024/04/23 16:55:55 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

// 1 - create
// 2 - join
// 3 - leave

void		send_infos(Channel &channel, std::string channel_name, Client &client);

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

int     check_limit(Channel &chan, Client c_client)
{
    if (chan.get_limit() == -1)
        return 0;
    if (chan.get_limit() <= chan.get_size())
    {
        std::cout << chan.get_limit() << "\n";
        std::string full = "\e[1;31mCannot join this channel because it is full.\n\e[0m";
        send(c_client.get_client_fd(), full.c_str(), full.size(), 0);
        return 1;
    }
    return 0;
}

// bool	Server::check_channel(Client &c_client)
// {
// 	size_t i;
// 	for (i = 0; i < channel_vec.size(); i++)
// 	{
// 		if (channel_vec[i].get_name() == c_client.get_current_chan())
// 			return true;
// 	}
// 	std::string invite_failed = red + "You are not in a channel.\n" + white;
// 	send(c_client.get_client_fd(), invite_failed.c_str(), invite_failed.size(), 0);
// 	return false;
// }

// bool Server::invite_target(Client &c_client, Client &target, std::string chan)
// {
// 	std::string y_n = "[y/n] : ";
// 	std::string invite = yellow + c_client.getNickname() + 
// 		" sent to you an invitation to join #" + chan + " channel.\n Do you want to join ? [y/n] : " + white;
// 	std::string invite_wait = yellow + "Invitation was sent successfuly, please wait.\n";
// 	send(target.get_client_fd(), invite.c_str(), invite.size(), 0);
// 	send(c_client.get_client_fd(), invite_wait.c_str(), invite_wait.size(), 0);
// 	while (1)
// 	{
// 		char buff[1024];
// 		int byte = read(target.get_client_fd(), buff, sizeof(buff));
// 		if (byte > 0) 
// 		{
// 			buff[byte] = '\0';
// 			std::string answer(buff);
// 			if (answer.size() == 2 && answer[0] == 'y') 
// 			{
// 				std::string invite_accepted = green + target.getNickname() + " accepted your invitation.\n" + white;
// 				send(c_client.get_client_fd(), invite_accepted.c_str(), invite_accepted.size(), 0);
// 				return true;
// 			}
// 			else if (answer.size() == 2 && answer[0] == 'n')
// 				return false;
// 			else
// 				send(target.get_client_fd(), y_n.c_str(), y_n.size(), 0);
// 		}
// 		else if (byte == 0) {
// 			std::cerr << "Connexion fermée par le client" << std::endl;
// 			break ;
// 		}
// 	}
// 	return false;
// }

// void	Server::INVITE(std::string buffer, Client c_client)
// {
// 	if (check_channel(c_client) == false)
// 		return ;
// 	std::string client_target;
// 	std::stringstream sbuf(buffer);
// 	std::string cmd;
// 	std::getline(sbuf, cmd, ' ');
	
// 	int			exist = 0;
// 	size_t		i;
// 	while (std::getline(sbuf, client_target, '\n')) {
// 		if (!client_target.empty())
// 			break ;
// 	}
// 	for (i = 0; i < client_vec.size(); i++) {
// 		if (client_vec[i].getNickname() == client_target) {
// 			exist = 1;
// 			break ;
// 		}
// 	}
// 	if (exist)
// 	{
// 		size_t j;
// 		for (j = 0; j < channel_vec.size(); j++)
// 		{
// 			if (channel_vec[j].get_name() == client_vec[i].get_current_chan()) 
// 			{
// 				if (channel_vec[j].get_name() == c_client.get_current_chan())
// 				{
// 					std::string into_chan = red + client_target + " is already into this channel.\n" + white;
// 					send(c_client.get_client_fd(), into_chan.c_str(), into_chan.size(), 0);
// 					return ;				
// 				}
// 				if (invite_target(c_client, client_vec[i], c_client.get_current_chan()) == true)
// 				{
// 					client_vec[i].set_invite_access();
// 				//	LEAVE("null", client_vec[i]);
// 				//	JOIN("/join " + c_client.get_current_chan(), client_vec[i]);
// 					return ;
// 				}
// 				else
// 				{
// 					std::string declined = yellow + client_target + " declined your invitation.\n" + white;
// 					send(c_client.get_client_fd(), declined.c_str(), declined.size(), 0);
// 					return ;
// 				}
// 			}
// 		}
// 		if (invite_target(c_client, client_vec[i], c_client.get_current_chan()) == true)
// 		{
// 			client_vec[i].set_invite_access();
// 		//	JOIN("/join " + c_client.get_current_chan(), client_vec[i]);
// 		}
// 		else
// 		{
// 			std::string declined = yellow + client_target + " declined your invitation.\n" + white;
// 			send(c_client.get_client_fd(), declined.c_str(), declined.size(), 0);
// 			return ;
// 		}
// 	}
// 	else {
// 		std::string no_target = red + client_target + " nickname unknow.\n" + white;
// 		send(c_client.get_client_fd(), no_target.c_str(), no_target.size(), 0);
// 	}
// }
