/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 01:45:49 by keshikuro         #+#    #+#             */
/*   Updated: 2024/04/24 23:53:06 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "irc.hpp"
#include "Client.hpp"

class Channel {
	public :
		Channel();
		Channel(std::string name);
		Channel(std::string name, int fd);
		~Channel();

		void	add_user(Client to_add);
		void	rm_user(Client to_rm);
		void	add_operator(Client to_set);
		void	rm_operator(Client to_set);
		void	send_to_all(std::string buffer, Client c_client);
		void	send_string(std::string tosend, std::string nick, std::string target, std::string msg);
		void	send_string_all(std::string to_send);
		void	rm_backslash_n(std::string &s);
		
		void	set_description(std::string &s);
		void	set_user_max(size_t max);
		void	set_keypass_k(std::string kp);
		void	set_key_set(); // to modify keypass_set
		void	set_limit(int lim);
		void	set_invite_set();
		void	set_topic_opr(bool to_set);

		std::string	&get_name();
		std::string	get_description();
		size_t		get_user_max();
		int        	get_size(void);
		void        get_clients();
		std::string	get_keypass(); // to get the string keypass
		bool		get_key_set(); // to know if keypass set or not
		int         get_limit();
		bool		get_invite_set();
		bool		get_topic_opr();
		
		std::vector<Client> client_list;
		std::vector<Client> op_clients;
		std::vector<int>	invited_clients;

	private :
		std::string channel_name;
		int         creator_fd;
		std::string description;
		size_t		user_max;	
		int         limit;
		bool		keypass_set;
		std::string channel_keypass;
		bool		invite_mode;
		bool		topic_opr;
		std::string red;
		std::string green;
		std::string white;
		std::string yellow;
};

#endif