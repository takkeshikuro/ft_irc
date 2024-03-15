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

		int		get_client_fd();
 		void	set_client_fd(int fd);
 		void	set_IpAdd(std::string ipadd);
        void    set_current_channel(std::string);
		void	client_starting_point();
		bool	check_password(std::string pw_to_check);
		void	starting_point_data_set();
		
		int getSocket() const;
		std::string& getUsername();
		std::string& getNickname();
		std::string& getIPAddress();
        std::string get_current_chan(void) const;
        void    setNickname(std::string new_nick);
		void	set_admin_perm();
		
        int in_channel;

	private : 
		std::string password;
		int socket_usr;
		int client_fd;
 		bool is_admin;
		std::string IPadd; //-> client ip address

		std::string username;
		std::string nickname;
		std::string ip_addr;

        std::string current_channel;
		
		std::string green;
		std::string white;
		std::string red;
		std::string yellow;
		
};

#endif