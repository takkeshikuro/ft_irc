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

bool	Client::check_double_usage(std::vector<Client> &client_vec, std::string nick)
{
	for (size_t i = 0; i < client_vec.size(); i++)
	{
		if (nick == client_vec[i].getNickname())
			return true;
	}
	return false;
}

void	Client::client_starting_point() 
{
	std::string message001 = "!Bienvenue sur le serveur IRC irc.server.com!\r\n";
	send(client_fd, message001.c_str(), message001.length(), 0);
	std::string s1 = "\n\e[0;35m/$$$$$$$$ /$$$$$$$$     /$$$$$$ /$$$$$$$   /$$$$$$ \r\n";
	std::string s2 = "| $$_____/|__  $$__/    |_  $$_/| $$__  $$ /$$__  $$\r\n";
	std::string s3 = "| $$         | $$         | $$  | $$  \\ $$| $$  \\__/\r\n";
	std::string s4 = "| $$$$$      | $$         | $$  | $$$$$$$/|$$      \r\n";
	std::string s5 = "| $$__/      | $$         | $$  | $$__  $$|$$      \r\n";
	std::string s6 = "| $$         | $$         | $$  | $$  \\ $$| $$    $$\r\n";
	std::string s7 = "| $$         | $$        /$$$$$$| $$  | $$|  $$$$$$/\r\n";
	std::string s8 = "|__/         |__//$$$$$$|______/|__/  |__/ \\______/ \r\n";
	std::string s9 = "                |______/                          \e[0m\r\n";
	
	send(client_fd, s1.c_str(), s1.size(), 0);
	send(client_fd, s2.c_str(), s2.size(), 0);
	send(client_fd, s3.c_str(), s3.size(), 0);
	send(client_fd, s4.c_str(), s4.size(), 0);
	send(client_fd, s5.c_str(), s5.size(), 0);
	send(client_fd, s6.c_str(), s6.size(), 0);
	send(client_fd, s7.c_str(), s7.size(), 0);
	send(client_fd, s8.c_str(), s8.size(), 0);
	send(client_fd, s9.c_str(), s9.size(), 0);
	std::string welcome = green + "\nWelcome!\n";
	std::string w1 = "\n\e[1;34mYou must use /login to access the server and create a new account :\r\n";
	std::string w2 = "please use : [/login <server_password> <username> <nickname>]\r\n";
	std::string w3 = "Your username and nickname must not contain invalid char [,/*/?/!/@/.] and have more than 15 characters\r\n";
	std::string w4 = "Your nickname is unique.\r\n" + white;

	send(client_fd, welcome.c_str(), welcome.length(), 0);
	send(client_fd, w1.c_str(), w1.length(), 0);
	send(client_fd, w2.c_str(), w2.length(), 0);
	send(client_fd, w3.c_str(), w3.length(), 0);
	send(client_fd, w4.c_str(), w4.length(), 0);

}

