/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 02:23:18 by keshikuro         #+#    #+#             */
/*   Updated: 2024/03/08 05:40:07 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

Channel::Channel() {}

Channel::Channel(std::string name) {
	this->channel_name = name;
	this->creator_fd = 0;
	this->user_max = 10;
	//	
}

Channel::Channel(std::string name, int fd) : channel_name(name), creator_fd(fd) {
//    green = "\e[1;32m";
//	white = "\e[0;37m";
	red = "\e[1;31m";
//	yellow = "\e[1;33m";

// yaura dautre variable a set genre nb max d'user tt ca

	std::cout << "creation of new channel named " << name << std::endl;
	std::string bio_msg = "You just created a new channel. please give a small description of it : \n";
	send(creator_fd, bio_msg.c_str(), bio_msg.size(), 0);
	while (1) 
	{
		char buffer[1024];
		int bytes = read(creator_fd, buffer, sizeof(buffer));
		if (bytes > 0) {
			buffer[bytes] = '\0';
			std::string bio(buffer);
			rm_backslash_n(bio);
			if (bio.empty()) {
				std::string null_buffer = red + "cant be null.\e[0;33m\nNew username : ";
				send(creator_fd, null_buffer.c_str(), null_buffer.length(), 0);
			}
			else {
				std::cout << "[creator's bio : " << bio << "]\n";
				this->description = bio;
				break ;
			}
		}
		else if (bytes == 0) {
			std::cerr << "Connexion fermée par le client" << std::endl;
			break ;
		}
	}
}

Channel::~Channel() {}

std::string &Channel::get_name() {
	return channel_name;
}

std::string &Channel::get_description() {
	return description;
}

void	Channel::set_description(std::string &s) {
	this->description = s;
}

void    Channel::get_clients()
{
	std::cout << YEL << "Users in channel " << this->get_name() << "\n";
	for (size_t i = 0; i < client_list.size(); i++)
	{
		std::cout << YELLOW << client_list[i]->getNickname() << "\n";
	}
	return ;
}

void	Channel::rm_backslash_n(std::string &s) // mettre dans utils
{
	if (!s.empty() && s[s.length() - 1] == '\n') {
		s[s.length() - 1] = '\0';
		s.resize(s.length() - 1);
	}	
}

void    Channel::add_user(Client *to_add)
{
	client_list.push_back(to_add);
	std::cout << YEL << to_add->getNickname() << GRE 
	<< " has been added to channel #" << this->get_name() << "\n" << RESET; 
	return ;
}

void	Channel::rm_user(Client *to_rm)
{
	std::vector<Client>::iterator it;
	for (size_t i = 0; i < client_list.size(); ++i) 
	{
		
		if (client_list[i] == to_rm) {
			client_list.erase(client_list.begin() + i);
			std::cout << YEL << to_rm->getNickname() << GRE 
   			<< " leave channel #" << this->get_name() << "\n" << RESET; 
			break;
		}
	}

}
	
void    Channel::send_to_all(std::string buffer, Client c_client)
{
	std::string nick = c_client.getNickname() + ": ";
	std::string chan = "[#" + c_client.get_current_chan() + "] ";
	std::string to_send = chan + nick + buffer;
	for (size_t i = 0; i < client_list.size(); i++)
	{
		send(client_list[i]->get_client_fd(), to_send.c_str(), to_send.size(), 0);
	}
}