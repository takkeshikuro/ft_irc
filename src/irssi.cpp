/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irssi.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorikaw <tmorikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 05:00:55 by tmorikaw          #+#    #+#             */
/*   Updated: 2024/03/22 05:01:01 by tmorikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

void	Client::client_starting_point_irssi(std::string &irssi_base) 
{
	std::cerr << "|||" << irssi_base << "|||\n";
	
	
	std::string welcome = ":server 001 tmorikaw :Welcome\n";
    send(client_fd, welcome.c_str(), welcome.size(), 0);
    char mode[1024];
	int byte1 = read(client_fd, mode, sizeof(mode));
	if (byte1 > 0) 
	{
		mode[byte1] = '\0';
		std::string s1(mode);
		std::cerr << ";irssi_s1; = " << s1 << "|" << std::endl;
		std::string ss1 = s1 + "after s1\n";
    	send(client_fd, ss1.c_str(), ss1.size(), 0);
	}

	
    char ping[1024];
	int byte2 = read(client_fd, ping, sizeof(ping));
	if (byte2 > 0) 
	{
		ping[byte2] = '\0';
		std::string s2(ping);
		std::cerr << ";irssi_s2; = " << s2 << "|" << std::endl;
		std::string ss2 = s2 + "after s2\n";
		send(client_fd, ss2.c_str(), ss2.size(), 0);
	}
	
    

    char quit[1024];
	int byte3 = read(client_fd, quit, sizeof(quit));
	if (byte3 > 0) 
	{
		quit[byte3] = '\0';
		std::string s3(quit);
		std::cerr << ";irssi_s3; = " << s3 << "|" << std::endl;
		std::string ss3 = s3 + "after s3\n";
    	send(client_fd, ss3.c_str(), ss3.size(), 0);
	}


	
	// std::string needpw = "\nPassword required for authentication : ";
	// send(client_fd, needpw.c_str(), needpw.length(), 0);
	// while (1) 
	// {
	// 	char buffer[1024];
	// 	int bytes_lus = read(client_fd, buffer, sizeof(buffer));
	// 	if (bytes_lus > 0) 
	// 	{
	// 		buffer[bytes_lus] = '\0';
	// 		std::string client_pw(buffer);
	// 		//rm_backslash_n(client_pw);
	// 		std::cout << "[password given by user : " << client_pw << "]\n";
	// 		if (check_password(client_pw) == true) {
	// 			std::string auth_ok = green + "Authentification ok.\n" + white ;
	// 			send(client_fd, auth_ok.c_str(), auth_ok.length(), 0);
	// 			break;
	// 		}
	// 		else {
	// 			std::cout << "[user authentification failed]\n";
	// 			std::string auth_ko = red + "Authentification failed, pls try again.\n";
	// 			send(client_fd, auth_ko.c_str(), auth_ko.length(), 0);
	// 			std::string needpw = white + "\nPassword required for authentication : ";
	// 			send(client_fd, needpw.c_str(), needpw.length(), 0);
	// 		}			
	// 	}
	// 	else if (bytes_lus == 0) {
	// 		std::cerr << "Connexion fermée par le client" << std::endl;
	// 		break ;
	// 	}
	// }
//	starting_point_data_set();
	std::cout << "starting point irssi over\n";
}

int cmp(std::string s1) 
{
	std::string cap_ls = "CAP LS";

    for (int i = 0; i < 6; ++i) 
	{
        if (s1[i] != cap_ls[i])
            return 1;
    }
    return 0;
}


int Server::check_irssi_entrance(int fd)
{
	char buff[1024];
	int byte = read(fd, buff, sizeof(buff));
	if (byte > 0) 
	{
		buff[byte] = '\0';
		std::string answer(buff);
		if (cmp(answer) == 0)
		{
			std::cerr << ";irssi connexion;\n";
			this->irssi_base = answer;
            return 1;
		}
	}
    return 0;
}