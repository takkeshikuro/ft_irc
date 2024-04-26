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
class Server;

class Client {
	public :
		Client();
		Client(std::string pw, std::string dt);
		~Client();
		
		int		client_starting_point_irssi(std::string &irssi_base, Server& server, std::vector<Client> &client_vec);
		void	client_starting_point();
	//	int		starting_point_data_set(std::vector<Client> &client_vec);
		bool	check_password(std::string pw_to_check);
		bool	check_double_usage(std::vector<Client> &client_vec, std::string nick);

		
		int		set_user_data(std::string &s_data, Server& server, std::vector<Client> &client_vec);
		void	set_client_fd(int fd);
 		void	set_IpAdd(std::string ipadd);
		void	setNickname(std::string new_nick);
		void	setUsername(std::string new_user);
		void	setRealname(std::string new_real);
		void	set_current_channel(std::string);
		void	set_invite_access();
		void	set_is_irssi();
		void	set_admin_perm();
		void	set_quit_status(int ok);

		int				get_client_fd();
		int 			getSocket() const;
		std::string&	getIPAddress();
		std::string&	getNickname();
		std::string&	getUsername();
		std::string&	getRealname();
		std::string 	get_current_chan(void) const;
		int				get_invite_access();
		bool			get_is_irssi() const;
		bool			get_quit_status() const;

		int 	in_channel;
		bool	is_registred;

	private : 
		std::string	password;
		std::string IPadd;
		int			socket_usr;
		bool		is_irssi;
		int			client_fd;
 		bool		is_admin;
		bool		quit_status;
		int			invite_access;
		std::string datetime;
		std::string ip_addr;
		std::string nickname;
		std::string username;
		std::string realname;
		std::string current_channel;
		
		std::string green;
		std::string white;
		std::string red;
		std::string yellow;
};

#endif