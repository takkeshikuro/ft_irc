/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irssi.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 05:00:55 by tmorikaw          #+#    #+#             */
/*   Updated: 2024/04/09 00:03:28 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

void	Client::set_user_data(std::string &s_data)
{
	std::string nickname_i;
	std::size_t nick_start = (s_data.find(' ', 8) + 1);
	std::size_t nick_end = (s_data.find('\n', nick_start) - 1);
	nickname_i = s_data.substr(nick_start, (nick_end - nick_start));	
	this->setNickname(nickname_i);
	
	
	std::string username_i;
	std::size_t start = s_data.find("USER");
	std::size_t user_start = (s_data.find(' ', start) + 1);
	std::size_t user_end = s_data.find(' ', user_start);
	username_i = s_data.substr(user_start, (user_end - user_start));
	this->setUsername(username_i);

	std::string realname_i;
	start = s_data.find("127.0.0.1");
	std::size_t real_start = (s_data.find(' ', start) + 2);
	std::size_t real_end = s_data.find('\n', real_start);
	realname_i = s_data.substr(real_start, (real_end - real_start));
	this->setRealname(realname_i);
	return ;
}

void	Client::client_starting_point_irssi(std::string &irssi_base) 
{
	set_user_data(irssi_base);
	std::string s1 = yellow + "	 /$$$$$$$$ /$$$$$$$$     /$$$$$$ /$$$$$$$   /$$$$$$ \r\n" + white;
	std::string s2 = yellow + "| $$_____/|__  $$__/    |_  $$_/| $$__  $$ /$$__  $$\r\n" + white;
	std::string s3 = yellow + "| $$         | $$         | $$  | $$  \\ $$| $$  \\__/\r\n" + white;
	std::string s4 = yellow + "| $$$$$      | $$         | $$  | $$$$$$$/|$$      \r\n" + white;
	std::string s5 = yellow + "| $$__/      | $$         | $$  | $$__  $$|$$      \r\n";
	std::string s6 = yellow + "| $$         | $$         | $$  | $$  \\ $$| $$    $$\r\n" + white;
	std::string s7 = yellow + "| $$         | $$        /$$$$$$| $$  | $$|  $$$$$$/\r\n" + white;
	std::string s8 = yellow + "|__/         |__//$$$$$$|______/|__/  |__/ \\______/ \r\n" + white;
	std::string s9 = yellow + "                |______/                          \r\n" + white;
	std::string s10 = yellow + "Enjoy!\r\n" + white;
	
	send(client_fd, RPL_WELCOME(nickname, IPadd).c_str(), RPL_WELCOME(nickname, IPadd).size(), 0);
	send(client_fd, RPL_YOURHOST(nickname, "[pirate server]").c_str(), RPL_YOURHOST(nickname, "pirate server").size(), 0);
	send(client_fd, RPL_CREATED(nickname, datetime).c_str(), RPL_CREATED(nickname, datetime).size(), 0);
	send(client_fd, RPL_MYINFO(nickname, "[pirate server]").c_str(), RPL_MYINFO(nickname, "pirate server").size(), 0);
	send(client_fd, RPL_ISUPPORT(nickname).c_str(), RPL_ISUPPORT(nickname).size(), 0);
	send(client_fd, s1.c_str(), s1.size(), 0);
	send(client_fd, s2.c_str(), s2.size(), 0);
	send(client_fd, s3.c_str(), s3.size(), 0);
	send(client_fd, s4.c_str(), s4.size(), 0);
	send(client_fd, s5.c_str(), s5.size(), 0);
	send(client_fd, s6.c_str(), s6.size(), 0);
	send(client_fd, s7.c_str(), s7.size(), 0);
	send(client_fd, s8.c_str(), s8.size(), 0);
	send(client_fd, s9.c_str(), s9.size(), 0);

	send(client_fd, s10.c_str(), s10.size(), 0);

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

	
	std::cout <<"starting point irssi over\n";
}

int cmp(std::string s1) 
{
	std::string cap_ls = "CAP LS";

	for (int i = 0; i < 6; ++i) {
		if (s1[i] != cap_ls[i])
			return 1;
	}
	return 0;
}

int Server::check_irssi_entrance(int fd)
{
	char buff[1024];
	sleep(1);
	int byte = read(fd, buff, sizeof(buff));
	if (byte > 0) 
	{
		buff[byte] = '\0';
		std::string answer(buff);
		if (cmp(answer) == 0)
		{
			std::cerr << "!irssi connexion!\n";
			this->irssi_base = answer;
			return 1;
		}
	}
	else
		std::cerr << "check irssi entrance failed\n";
	return 0;
}