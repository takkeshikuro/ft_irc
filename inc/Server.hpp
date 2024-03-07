/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 01:19:18 by keshikuro         #+#    #+#             */
/*   Updated: 2024/03/02 08:12:32 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "irc.hpp"
#include "exception.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Client;
//class Channel;

class Server {
	public :
		Server();
		Server(std::string passwd);
		~Server();

		void	configuration();
		void	default_channel_creation();
		void	launch_server();
		void	manage_new_client();
		void	manage_new_data(int fd);
		static void SignalHandler(int sig);
		void close_fds(); // close file descriptors
 		void clear_clients(int fd); // clear clients

        int is_command(char *buffer, Client c_client); //parsing command
        void    PRIVMSG(std::string buffer, Client c_client);
		void    WHOIS(std::string buffer, Client c_client);
		void	CREATE(std::string buffer, Client c_client);
		void	REMOVE(std::string buffer, Client c_client);
		void	JOIN(std::string buffer, Client c_client);
		void	QUIT(std::string buffer, Client c_client);
		void	LIST_CH(std::string buffer, Client c_client);
        void    NICK(std::string buffer, Client c_client);

	private :
		struct sockaddr_in serverAddr;
		struct pollfd NewPoll;
		int serverSocket;
		int port;
		std::string password;
		static bool signal;
		std::vector<Client> client_vec; //-> vector of clients/users
		std::vector<Channel> channel_vec;
		std::vector<struct pollfd> fds; //-> vector of pollfd		
		char buffer[BUFFER_SIZE];
};

#endif