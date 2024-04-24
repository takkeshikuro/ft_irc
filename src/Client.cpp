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

Client::Client(std::string pw, std::string dt) : password(pw) 
{
	is_registred = false;	
	in_channel = 0;
	invite_access = 0;
	is_admin = false;
	is_irssi = false;
	quit_status = false;
	datetime = dt;
	set_current_channel("!no_chan!");
	
	green = "\e[1;32m";
	white = "\e[0;37m";
	red = "\e[1;31m";
	yellow = "\e[1;33m";
}

Client::~Client() {}


//--------->SETTER
void	Client::set_client_fd(int fd) { client_fd = fd; } 
void	Client::set_IpAdd(std::string ipadd) { IPadd = ipadd; }
void	Client::setNickname(std::string new_nick) { nickname = new_nick; }
void	Client::setUsername(std::string new_user) { username = new_user; }
void	Client::setRealname(std::string new_real) { realname = new_real; }
void	Client::set_current_channel(std::string new_chan) { current_channel = new_chan; }
void	Client::set_quit_status(int ok) { quit_status = ok; }
void	Client::set_invite_access() {
	if (this->invite_access == 0)
		this->invite_access = 1;
	else
		this->invite_access = 0;
}
void	Client::set_is_irssi() {
	if (this->is_irssi == false)
		this->is_irssi = true;
	else
		this->is_irssi = false;
}
void	Client::set_admin_perm() {
	this->is_admin = true;
	std::cout <<GREEN <<"client [" <<YELLOW <<nickname <<GREEN \
			  <<"] is now admin." << WHITE<< "\n";
}

//----------> GETTER
int				Client::get_client_fd() { return client_fd; }
int				Client::getSocket() const { return socket_usr; }
std::string&	Client::getIPAddress() { return ip_addr; }
std::string&	Client::getNickname() { return nickname; }
std::string&	Client::getUsername() { return username; }
std::string&	Client::getRealname() { return realname; }
std::string		Client::get_current_chan() const { return current_channel; }
int				Client::get_invite_access() { return this->invite_access; }
bool			Client::get_is_irssi() const { return is_irssi; }
bool			Client::get_quit_status() const { return quit_status; }


bool	Client::check_password(std::string pw_to_check)
{
	if (pw_to_check.length() == password.length()) {
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

int	Client::starting_point_data_set()
{
	std::string Account_welcome = yellow + "\n!Account creation!\n(you will can change information later)\nNew username : ";
	send(client_fd, Account_welcome.c_str(), Account_welcome.length(), 0);
	std::cout << "waiting for username...\n";
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
				std::cout << YELLOW << "[client_username given by user : " << client_username << "]\n" << RESET;
				this->username = client_username;
				break ;
			}
		}
		else if (bytes_lus == 0) {
			std::cerr << RED << "Connexion closed by user" << std::endl << RESET;
			return FAILURE;
		}
	}
	std::string new_nick = "\nNew nickname : ";
	send(client_fd, new_nick.c_str(), new_nick.length(), 0);
	std::cout << "waiting for nickname...\n";
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
				std::cout << YELLOW << "[client_nickname given by user : " << client_nickname << "]\n" << RESET;
				this->nickname = client_nickname;
				break ;
			}
		}
		else if (bytes_lus == 0) {
			std::cerr << RED << "Connexion closed by user" << std::endl << RESET;
			return FAILURE;
		}
	}
	std::string data_save = green + "\nNew data saved.\nWelcome!\n";
	std::string helper = "[press /help to get all the possible commands]\n" + white;
	send(client_fd, data_save.c_str(), data_save.length(), 0);
	send(client_fd, helper.c_str(), helper.length(), 0);
	return SUCCESS;
}

int	Client::client_starting_point() 
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
			std::cout << YELLOW << "[password given : " << client_pw << "] : " << RESET;
			if (check_password(client_pw) == true) 
			{
				std::cout << GRE << "[password match]\n" << RESET;
				std::string auth_ok = green + "Authentification ok.\n" + white ;
				send(client_fd, auth_ok.c_str(), auth_ok.length(), 0);
				break;
			}
			else {
				std::cout << RED << "[password mismatch]\n" << RESET;
				std::string auth_ko = red + "Authentification failed, pls try again.\n";
				send(client_fd, auth_ko.c_str(), auth_ko.length(), 0);
				std::string needpw = white + "\nPassword required for authentication : ";
				send(client_fd, needpw.c_str(), needpw.length(), 0);
			}			
		}
		else if (bytes_lus == 0) {
			std::cerr << RED << "Connexion closed by user" << std::endl << RESET;
			return FAILURE;
		}
	}
	if (starting_point_data_set() == SUCCESS) 
	{
		this->is_registred = true;
		std::cout << GRE << "Authentification ok : starting point over\n" << RESET;
		return SUCCESS;
	}
	return FAILURE;
}

