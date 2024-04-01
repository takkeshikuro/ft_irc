/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 02:23:18 by keshikuro         #+#    #+#             */
/*   Updated: 2024/03/28 09:24:59 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

Channel::Channel() {}

Channel::Channel(std::string name) {
	if (name[0] == '#')
		this->channel_name = name;
	else
		this->channel_name = "#" + name;
	this->creator_fd = 0;
	this->user_max = 10;
	this->limit = -1;
	keypass_set = false;
	invite_mode = false;
	//	
}

Channel::Channel(std::string name, int fd) : creator_fd(fd), limit(-1)
{
	green = "\e[1;32m";
	white = "\e[0;37m";
	red = "\e[1;31m";
	yellow = "\e[1;33m";
	
// yaura dautre variable a set genre nb max d'user tt ca
	if (name[0] == '#')
		channel_name = name;
	else
		channel_name = "#" + name;
	keypass_set = false;
// +ask for max user

	std::cout << "creation of new channel named " << name << std::endl;
	std::string bio_msg = "You just created a new channel. please give a small description of it : \n";
	send(creator_fd, bio_msg.c_str(), bio_msg.size(), 0);
	
	// while (1) 
	// {
	// 	char buffer[1024];
	// 	int bytes = read(creator_fd, buffer, sizeof(buffer));
	// 	if (bytes > 0) {
	// 		buffer[bytes] = '\0';
	// 		std::string bio(buffer);
	// 		rm_backslash_n(bio);
	// 		if (bio.empty()) {
	// 			std::string null_buffer = red + "cant be null.\e[0;33m\nNew username : ";
	// 			send(creator_fd, null_buffer.c_str(), null_buffer.length(), 0);
	// 		}
	// 		else {
	// 			std::cout << "[creator's bio : " << bio << "]\n";
	// 			this->description = bio;
	// 			break ;
	// 		}
	// 	}
	// 	else if (bytes == 0) {
	// 		std::cerr << "Connexion fermée par le client" << std::endl;
	// 		break ;
	// 	}
	// }
}

Channel::~Channel() {}


//------> GETTER
std::string&	Channel::get_name() { return channel_name; }
std::string&	Channel::get_description() { return description; }
std::string		Channel::get_keypass() { return this->channel_keypass; }
int				Channel::get_limit() { return limit; }
int				Channel::get_size() { return client_list.size(); }
bool			Channel::get_invite_set() { return this->invite_mode; }
size_t				Channel::get_user_max() { return this->user_max; }

//-------> SETTER
void	Channel::set_description(std::string &s) { this->description = s; }
void	Channel::set_limit(int lim) { limit = lim; }
bool	Channel::get_key_set() {
	if (keypass_set == true)
		return true;
	else
		return false;
}

void	Channel::set_key_set() {
	if (this->keypass_set == true)
		this->keypass_set = false;
	else
		this->keypass_set = true;
}

void	Channel::set_invite_set() {
	if (this->invite_mode == false)
		this->invite_mode = true;
	else
		this->invite_mode = false;
}

void    Channel::send_string(std::string to_send, std::string nick)
{
	for (size_t i = 0; i < client_list.size(); i++)
	{
		if (client_list[i].getNickname() != nick)
			send(client_list[i].get_client_fd(), to_send.c_str(), to_send.size(), 0);
	}
}

void		Channel::set_keypass(Client c_client) 
{
	for (size_t i = 0; i < op_clients.size(); i++) {
		if (c_client.get_client_fd() == op_clients[i].get_client_fd())
			break;
		else if ((i + 1) == op_clients.size())
		{
			std::string no_ope = "Sorry only operators can set keypass.\n";
			send(c_client.get_client_fd(), no_ope.c_str(), no_ope.length(), 0);
			return ;
		}
	}
	std::string key = yellow + "please enter the new keypass : ";
	send(c_client.get_client_fd(), key.c_str(), key.size(), 0);
	while (1)
	{
		char buffer[1024];
		int bytes = read(c_client.get_client_fd(), buffer, sizeof(buffer));
		if (bytes > 0) {
			buffer[bytes] = '\0';
			std::string keypass(buffer);
			rm_backslash_n(keypass);
			if (keypass.empty()) {
				std::string null_buffer = red + "cant be null.\e[0;33m\nNew keypass : ";
				send(c_client.get_client_fd(), null_buffer.c_str(), null_buffer.length(), 0);
			}
			else {
				std::string set_ok = green + "[New keypass set]\n" + white;
				send(c_client.get_client_fd(), set_ok.c_str(), set_ok.size(), 0);
				this->channel_keypass = keypass;
				this->keypass_set = true;
				break ;
			}
		}
		else if (bytes == 0) {
			std::cerr << "Connexion fermée par le client" << std::endl;
			break ;
		}
	}
}

bool	Channel::check_kp(std::string kp_to_check)
{
	if (kp_to_check.length() == channel_keypass.length())
	{
		if (this->channel_keypass == kp_to_check)
			return true;
	}
	return false;
}

bool	Channel::check_keypass(Client c_client)
{
	if (this->keypass_set == true)
	{
		std::string ask = "this channel requires a keypass/password to access.\nKeypass : ";
		send(c_client.get_client_fd(), ask.c_str(), ask.size(), 0);
		while (1)
		{
			char buffer[1024];
			int bytes = read(c_client.get_client_fd(), buffer, sizeof(buffer));
			if (bytes > 0) 
			{
				buffer[bytes] = '\0';
				std::string keypass(buffer);
				rm_backslash_n(keypass);
				if (check_kp(keypass) == true) {
					std::string ok = green + "keypass ok.\n" + white;
					send(c_client.get_client_fd(), ok.c_str(), ok.size(), 0);
					return true;
				}
				else {
					std::string ko = red + "keypass not ok.\n" + white;
					send(c_client.get_client_fd(), ko.c_str(), ko.size(), 0);
					return false;
				}
			}
			else if (bytes == 0) {
				std::cerr << "Connexion fermée par le client" << std::endl;
				break ;
			}
		}
	}
	return true; //no keypass set
}

void    Channel::get_clients()
{
	std::cout << YEL << "Users in channel " << this->get_name() << "\n";
	for (size_t i = 0; i < client_list.size(); i++)
	{
		std::cout << YELLOW << client_list[i].getNickname() << "\n";
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

void    Channel::add_user(Client to_add)
{
	client_list.push_back(to_add);
	std::cout << YEL << to_add.getNickname() << GRE << " has been added to channel " << get_name() << "\n" << RESET; 
	return ;
}

void	Channel::rm_user(Client to_rm)
{
	for (size_t i = 0; i < client_list.size(); ++i)  {		
		if (client_list[i].get_client_fd() == to_rm.get_client_fd()) {
			client_list.erase(client_list.begin() + i);
			std::cout << YEL << to_rm.getNickname() << GRE << " left the channel " << get_name() << "\n" << RESET; 
			break;
		}
	}
}
	
void    Channel::send_to_all(std::string buffer, Client c_client)
{
	std::string nick = "\e[1;33m" + c_client.getNickname() + ": " + "\e[0m";
	std::string chan = "\e[1;34m[" + c_client.get_current_chan() + "] ";
	std::string to_send = chan + nick + buffer;
	std::string sender = chan + "\e[1;35m" +  c_client.getNickname() + "(you): " + "\e[0m" + buffer;
	
	for (size_t i = 0; i < client_list.size(); i++) {
		if (client_list[i].getUsername() == c_client.getUsername())
			send(client_list[i].get_client_fd(), sender.c_str(), sender.size(), 0);
		else
			send(client_list[i].get_client_fd(), to_send.c_str(), to_send.size(), 0);
	}
}

void    Channel::add_operator(Client to_add)
{
	for (size_t i = 0; i < op_clients.size(); i++) {
		if (op_clients[i].getUsername() == to_add.getUsername()) {
			std::cout << YEL << to_add.getNickname() << GRE << " is already an operator in " << get_name() << "\n" << RESET; 
			return ;
		}
	}
	std::cout << YEL << to_add.getNickname() << GRE << " is now an operator in " << get_name() << "\n" << RESET; 
	op_clients.push_back(to_add);
}

void    Channel::rm_operator(Client to_rm)
{
	for (size_t i = 0; i < op_clients.size(); i++) {
		if (op_clients[i].getUsername() == to_rm.getUsername()) {
			op_clients.erase(op_clients.begin() + i);
			std::cout << YEL << to_rm.getNickname() << RED << " was removed from operators in " << get_name() << "\n" << RESET; 
			break;
		}
	}
}


