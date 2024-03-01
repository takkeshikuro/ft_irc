/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 03:12:39 by keshikuro         #+#    #+#             */
/*   Updated: 2024/03/01 05:00:32 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include "irc.hpp"

class Client {
	public :
		Client();
		~Client();
		int get_client_fd() {return client_fd;} //-> getter for fd
 		void set_client_fd(int fd) {client_fd = fd;} //-> setter for fd
 		void set_IpAdd(std::string ipadd) {IPadd = ipadd;} //-> setter for ipadd
		
		
		int getSocket() const;
		std::string& getNickname();
		std::string& getIPAddress();

	private : 
		int socket_usr;
		int client_fd; //-> client file descriptor
 		std::string IPadd; //-> client ip address

	//	struct sockaddr_in cliadd;
 	//	struct pollfd NewPoll;
		std::string nickname;
		std::string ip_addr;
		
};

#endif