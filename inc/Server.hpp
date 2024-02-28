/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 01:19:18 by keshikuro         #+#    #+#             */
/*   Updated: 2024/02/28 02:04:31 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "irc.hpp"
#include "exception.hpp"

class Server {
	public :
		Server();
		~Server();

		void	configuration();
		void	launch_server();
		void	manage_new_connection();
		void	manage_client_data();

	private :
		int serverSocket;
		struct sockaddr_in serverAddress;
		std::vector<pollfd> fds;
		char buffer[BUFFER_SIZE];
};

#endif