/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irssi.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarecar <rmarecar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 05:00:55 by tmorikaw          #+#    #+#             */
/*   Updated: 2024/04/13 13:16:23 by rmarecar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

int	Client::set_user_data(std::string &s_data, Server& server)
{
	if (server.pass(s_data, *this) == FAILURE)
		return FAILURE;
	else
	{
		std::string nickname_i;
		std::size_t pos_nick = s_data.find("NICK");
		std::size_t nick_start = (s_data.find(' ', pos_nick) + 1);
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
		std::cout << GRE << "[authentification ok]\n"<< RESET;
		return SUCCESS;
	}
}

int	Client::client_starting_point_irssi(std::string &irssi_base, Server& server) 
{
	if (set_user_data(irssi_base, server) == FAILURE)
		return FAILURE;
	std::cout << YELLOW << "[checking nickname = "<< getNickname() << "]\n";
	std::cout << "[checking username = "<< getUsername() << "]\n";
	std::cout << " checking realname = "<< getRealname() << "[\n";
	std::cout << RESET;


	std::string s1 = yellow + "	 /$$$$$$$$ /$$$$$$$$     /$$$$$$ /$$$$$$$   /$$$$$$ \r\n" + white;
	std::string s2 = yellow + "| $$_____/|__  $$__/    |_  $$_/| $$__  $$ /$$__  $$\r\n" + white;
	std::string s3 = yellow + "| $$         | $$         | $$  | $$  \\ $$| $$  \\__/\r\n" + white;
	std::string s4 = yellow + "| $$$$$      | $$         | $$  | $$$$$$$/|$$      \r\n" + white;
	std::string s5 = yellow + "| $$__/      | $$         | $$  | $$__  $$|$$      \r\n";
	std::string s6 = yellow + "| $$         | $$         | $$  | $$  \\ $$| $$    $$\r\n" + white;
	std::string s7 = yellow + "| $$         | $$        /$$$$$$| $$  | $$|  $$$$$$/\r\n" + white;
	std::string s8 = yellow + "|__/         |__//$$$$$$|______/|__/  |__/ \\______/ \r\n" + white;
	std::string s9 = yellow + "                |______/                          \r\n" + white;
	std::string s10 = yellow + "Enjoy!\r\n" + RESET;
	
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
	
	std::cout <<"starting point irssi over\n\n";
	return SUCCESS;
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
		if (cmp(answer) == 0) {
			std::cerr << "!irssi connexion!\n\n";
			this->irssi_base = answer;
			return 1;
		}
	}
	else
		std::cerr << "!netcat connexion!\n\n";
	return 0;
}