/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 03:17:35 by keshikuro         #+#    #+#             */
/*   Updated: 2024/03/02 11:25:27 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client::Client() : in_channel(0) {}

Client::Client(std::string pw) : password(pw) {
	in_channel = 0;
	is_admin = false;
	green = "\e[1;32m";
	white = "\e[0;37m";
	red = "\e[1;31m";
	yellow = "\e[1;33m";
}

Client::~Client() {
	//std::cout << "Client " << nickname << " is gone\n";
}

bool	Client::check_password(std::string pw_to_check)
{
	if (pw_to_check.length() == password.length())
	{
		if (this->password == pw_to_check)
			return true;
	}
	return false;
}

void	rm_backslash_n(std::string &s)
{
	if (!s.empty() && s[s.length() - 1] == '\n') {
		s[s.length() - 1] = '\0';
		s.resize(s.length() - 1);
	}	
}

void	Client::starting_point_data_set()
{
	std::string Account_welcome = yellow + "\n!Account creation!\n(you will can change information later)\nNew username : ";
	send(client_fd, Account_welcome.c_str(), Account_welcome.length(), 0);
	while (1) 
	{
		char buffer[1024];
		int bytes_lus = read(client_fd, buffer, sizeof(buffer));
		if (bytes_lus > 0) {
			buffer[bytes_lus] = '\0';
			std::string client_username(buffer);
			rm_backslash_n(client_username);
			if (client_username.empty()) {
				std::string null_buffer = red + "cant be null.\e[0;33m\nNew username : ";
				send(client_fd, null_buffer.c_str(), null_buffer.length(), 0);
			}
			else {
				std::cout << "[client_username given by user : " << client_username << "]\n";
				this->username = client_username;
				break ;
			}
		}
		else if (bytes_lus == 0) {
			std::cerr << "Connexion fermée par le client" << std::endl;
			break ;
		}
	}
	std::string new_nick = "\nNew nickname : ";
	send(client_fd, new_nick.c_str(), new_nick.length(), 0);
	while (1)
	{
		char buffer[1024];
		int bytes_lus = read(client_fd, buffer, sizeof(buffer));
		if (bytes_lus > 0) {
			buffer[bytes_lus] = '\0';
			std::string client_nickname(buffer);
			rm_backslash_n(client_nickname);
			if (client_nickname.empty()) {
				std::string null_buffer = red + "cant be null.\e[0;33m\nNew nickname : ";
				send(client_fd, null_buffer.c_str(), null_buffer.length(), 0);
			}
			else {
				std::cout << "[client_nickname given by user : " << client_nickname << "]\n";
				this->nickname = client_nickname;
				break ;
			}
		}
		else if (bytes_lus == 0) {
			std::cerr << "Connexion fermée par le client" << std::endl;
			break ;
		}
	}
	std::string data_save = green + "\nNew data saved.\nWelcome!\n";
	std::string helper = "[press /help to get all the possible commands]\n" + white;
	send(client_fd, data_save.c_str(), data_save.length(), 0);
	send(client_fd, helper.c_str(), helper.length(), 0);

}

void	Client::client_starting_point() 
{
	std::string message001 = "!Bienvenue sur le serveur IRC irc.server.com!\r\n";
	send(client_fd, message001.c_str(), message001.length(), 0);
	
	std::string needpw = "\nPassword required for authentication : ";
	send(client_fd, needpw.c_str(), needpw.length(), 0);
	while (1) 
	{
		char buffer[1024];
		int bytes_lus = read(client_fd, buffer, sizeof(buffer));
		if (bytes_lus > 0) 
		{
			buffer[bytes_lus] = '\0';
			std::string client_pw(buffer);
			rm_backslash_n(client_pw);
			std::cout << "[password given by user : " << client_pw << "]\n";
			if (check_password(client_pw) == true) {
				std::string auth_ok = green + "Authentification ok.\n" + white ;
				send(client_fd, auth_ok.c_str(), auth_ok.length(), 0);
				break;
			}
			else {
				std::cout << "[user authentification failed]\n";
				std::string auth_ko = red + "Authentification failed, pls try again.\n";
				send(client_fd, auth_ko.c_str(), auth_ko.length(), 0);
				std::string needpw = white + "\nPassword required for authentication : ";
				send(client_fd, needpw.c_str(), needpw.length(), 0);
			}			
		}
		else if (bytes_lus == 0) {
			std::cerr << "Connexion fermée par le client" << std::endl;
			break ;
		}
	}
	starting_point_data_set();
	std::cout << "starting point over\n";
}

void	Client::set_admin_perm()
{
	this->is_admin = true;
	std::cout <<GREEN <<"client [" <<YELLOW <<nickname <<GREEN \
			  <<"] is now admin." << WHITE<< "\n";
	return ;
}

int Client::get_client_fd() {
	return client_fd;
}

void Client::set_client_fd(int fd) {
	client_fd = fd;
} 

void Client::set_IpAdd(std::string ipadd) {
	IPadd = ipadd;
}
		
int Client::getSocket() const {
	return socket_usr;
}

std::string& Client::getUsername() {
	return username;
}

std::string& Client::getNickname() {
	return nickname;
}

std::string& Client::getIPAddress() {
	return ip_addr;
}

void    Client::setNickname(std::string new_nick) {
	nickname = new_nick;
}

void    Client::set_current_channel(std::string new_chan) {
	current_channel = new_chan;
}

std::string    Client::get_current_chan() const {
	return current_channel;
}