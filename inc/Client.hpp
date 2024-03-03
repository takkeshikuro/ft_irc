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
		Client(std::string pw);
		~Client();
		int get_client_fd(); //-> getter for fd
 		void set_client_fd(int fd); //-> setter for fd
 		void set_IpAdd(std::string ipadd);//-> setter for ipadd
		void	client_starting_point();
		bool	check_password(std::string pw_to_check);
		void	starting_point_data_set();
		
		int getSocket() const;
		std::string& getUsername();
		std::string& getNickname();
		std::string& getIPAddress();

	private : 
		std::string password;
		int socket_usr;
		int client_fd; //-> client file descriptor
 		std::string IPadd; //-> client ip address

	//	struct sockaddr_in cliadd;
 	//	struct pollfd NewPoll;
		std::string username;
		std::string nickname;
		std::string ip_addr;
		std::string green;
		std::string white;
		std::string red;
		std::string yellow;
		
};

#endif