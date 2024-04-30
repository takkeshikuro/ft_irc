/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorikaw <tmorikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 02:23:18 by keshikuro         #+#    #+#             */
/*   Updated: 2024/04/30 06:12:16 by tmorikaw         ###   ########.fr       */
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
	this->limit = 10;
	keypass_set = false;
	invite_mode = false;
	topic_opr = true;
	description.clear();
}

Channel::Channel(std::string name, int fd) : creator_fd(fd), limit(10)
{
	user_max = 10;
	green = "\e[1;32m";
	white = "\e[0;37m";
	red = "\e[1;31m";
	yellow = "\e[1;33m";
	
	if (name[0] == '#')
		channel_name = name;
	else
		channel_name = "#" + name;
	keypass_set = false;
	topic_opr = true;
	invite_mode = false;
	std::cout <<yellow << "creation of new channel named " << name << std::endl << white;
}

Channel::~Channel() {}

//-------> SETTER
void	Channel::set_description(std::string &s) { this->description = s; }
void	Channel::set_keypass_k(std::string kp) { channel_keypass = kp; }
void	Channel::set_limit(int lim) { limit = lim; }
void	Channel::set_topic_opr(bool to_set) { topic_opr = to_set; }
void	Channel::set_user_max(size_t max) {
	user_max = max;
	limit = max;
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

//------> GETTER
std::string&	Channel::get_name() { return channel_name; }
std::string		Channel::get_description() { return description; }
size_t			Channel::get_user_max() { return this->user_max; }
int				Channel::get_size() { return client_list.size(); }
std::string		Channel::get_keypass() { return this->channel_keypass; }
int				Channel::get_limit() { return limit; }
bool			Channel::get_invite_set() { return this->invite_mode; }
bool			Channel::get_topic_opr() { return topic_opr; }
void			Channel::get_clients()
{
	std::cout << YEL << "Users in channel " << this->get_name() << "\n";
	for (size_t i = 0; i < client_list.size(); i++) {
		std::cout << YELLOW << client_list[i].getNickname() << "\n";
	}
}
bool			Channel::get_key_set() {
	if (keypass_set == true)
		return true;
	else
		return false;
}


void	Channel::add_user(Client to_add)
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
			std::cout << YEL << to_rm.getNickname() << RED << " was removed from channel client_list " << get_name() << "\n" << RESET; 
			break;
		}
	}
}

void	Channel::add_operator(Client to_add)
{
	for (size_t i = 0; i < op_clients.size(); i++) {
		if (op_clients[i].getNickname() == to_add.getNickname()) {
			std::cout << YEL << to_add.getNickname() << GRE << " is already an operator in " << get_name() << "\n" << RESET; 
			return ;
		}
	}
	std::cout << YEL << to_add.getNickname() << GRE << " is now an operator in " << get_name() << "\n" << RESET; 
	op_clients.push_back(to_add);
}

void	Channel::rm_operator(Client to_rm)
{
	for (size_t i = 0; i < op_clients.size(); i++) {
		if (op_clients[i].getNickname() == to_rm.getNickname()) {
			op_clients.erase(op_clients.begin() + i);
			std::cout << YEL << to_rm.getNickname() << RED << " was removed from operators in " << get_name() << "\n" << RESET; 
			break;
		}
	}
}

void	Channel::send_to_all(std::string buffer, Client c_client)
{
	std::string nick = "\e[1;33m" + c_client.getNickname() + ": " + "\e[0m";
	std::string chan = "\e[1;34m[" + c_client.get_current_chan() + "] ";
	std::string to_send = chan + nick + buffer;
	std::string sender = chan + "\e[1;35m" +  c_client.getNickname() + "(you): " + "\e[0m" + buffer;
	
	for (size_t i = 0; i < client_list.size(); i++) {
		if (client_list[i].getNickname() == c_client.getNickname())
			send(client_list[i].get_client_fd(), sender.c_str(), sender.size(), 0);
		else
			send(client_list[i].get_client_fd(), to_send.c_str(), to_send.size(), 0);
	}
}

void    Channel::send_string(std::string to_send, std::string nick, std::string target, std::string msg)
{
	std::string to_send_nc = "\e[1;34m[" + target + "] " + nick + " \e[0m" + msg;

	for (size_t i = 0; i < client_list.size(); i++)
	{
		if (client_list[i].getNickname() != nick)
		{
			if (client_list[i].get_is_irssi() == true)
				send(client_list[i].get_client_fd(), to_send.c_str(), to_send.size(), 0);
			else
				send(client_list[i].get_client_fd(), to_send_nc.c_str(), to_send_nc.size(), 0);
		}
	}
}

void    Channel::send_string_all(std::string to_send)
{
	for (size_t i = 0; i < client_list.size(); i++)
	{
		send(client_list[i].get_client_fd(), to_send.c_str(), to_send.size(), 0);
	}
}

void	Channel::rm_backslash_n(std::string &s) // mettre dans utils
{
	if (!s.empty() && s[s.length() - 1] == '\n') {
		s[s.length() - 1] = '\0';
		s.resize(s.length() - 1);
	}	
}
