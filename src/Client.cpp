/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 03:17:35 by keshikuro         #+#    #+#             */
/*   Updated: 2024/03/02 10:05:15 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client::Client() {}


Client::Client(std::string pw) : password(pw) {}

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

void	Client::client_starting_point() 
{
	std::string message001 = "!Bienvenue sur le serveur IRC irc.server.com!\r\n";
	send(client_fd, message001.c_str(), message001.length(), 0);
	
	std::string needpw = "Please write the server password : ";
	send(client_fd, needpw.c_str(), needpw.length(), 0);
	
	while (1) 
	{
		char buffer[1024];
    	int bytes_lus = read(client_fd, buffer, sizeof(buffer));
		if (bytes_lus > 0) 
		{
			buffer[bytes_lus] = '\0';
			std::string client_pw(buffer);
			std::cout << "Mot de passe saisi par l'user : " << client_pw <<std::endl;
			
			if (!client_pw.empty() && client_pw[client_pw.length() - 1] == '\n')
			{
				client_pw[client_pw.length() - 1] = '\0';
				client_pw.resize(client_pw.length() - 1);
			}
			if (check_password(client_pw) == true)
			{
				std::string auth_ok = "Authentification ok.\n";
				send(client_fd, auth_ok.c_str(), auth_ok.length(), 0);
				break;
			}
			else
			{
				std::string auth_ko = "Authentification failed, pls try again.\n";
				send(client_fd, auth_ko.c_str(), auth_ko.length(), 0);
			}			
		}
		else if (bytes_lus == 0) {
			std::cerr << "Connexion fermée par le client" << std::endl;
			break ;
		}
			
	//	else
	}
	std::cout << "starting point over\n";
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

std::string& Client::getNickname() {
	return nickname;
}

std::string& Client::getIPAddress() {
	return ip_addr;
}