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
		
		void	client_starting_point_irssi(std::string &irssi_base);
		void	set_user_data(std::string &s_data);

		void	set_client_fd(int fd);
		int		get_client_fd();
	//	int     get_size();
 		void	set_IpAdd(std::string ipadd);
		void	setNickname(std::string new_nick);
		void	setUsername(std::string new_user);
		void	setRealname(std::string new_real);

		void	client_starting_point();
		bool	check_password(std::string pw_to_check);
		void	starting_point_data_set();
		int 	getSocket() const;
		std::string&	getNickname();
		std::string&	getUsername();
		std::string&	getRealname();
		
		std::string&	getIPAddress();
		
		void		set_is_irssi();
		void		set_admin_perm();
		void		set_current_channel(std::string);
		std::string get_current_chan(void) const;
		void		set_invite_access();
		int			get_invite_access();

		bool		get_is_irssi() const;

		int in_channel;

	private : 
		std::string	password;
		std::string IPadd; //-> client ip address
		int			socket_usr;
		bool		is_irssi;
		int			client_fd;
 		bool		is_admin;
		int			invite_access;

		
		
		std::string nickname;
		std::string username;
		std::string realname;
		
		std::string ip_addr;
		std::string current_channel;
		
		std::string green;
		std::string white;
		std::string red;
		std::string yellow;
		
};

#endif