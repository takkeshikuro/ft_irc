/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 01:19:18 by keshikuro         #+#    #+#             */
/*   Updated: 2024/02/29 18:55:48 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "irc.hpp"
#include "exception.hpp"
#include "User.hpp"

class User;

class Server {
	public :
		Server();
		~Server();

		void	configuration();
		void	launch_server();
		void	manage_new_connection();
		void	manage_client_data();
	//	static void SignalHandler(int sig);

	private :
		int serverSocket;
		struct sockaddr_in serverAddr;
		int port;
		int SerSocketFd; //-> server socket file descriptor
		static bool Signal; //-> static boolean for signal
		std::vector<User> users; //-> vector of clients/users
		std::vector<struct pollfd> fds; //-> vector of pollfd
		
		char buffer[BUFFER_SIZE];
};

#endif