/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 01:45:49 by keshikuro         #+#    #+#             */
/*   Updated: 2024/03/15 04:50:35 by keshikuro        ###   ########.fr       */
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

	//	void        new_user_update(Client &new_client);
	//	void        exit_user_update(Client &client);
		void	    rm_backslash_n(std::string &s);
		std::string &get_name();
		std::string &get_description();
		void	    set_description(std::string &s);
        void        send_to_all(std::string buffer, Client c_client);
        void        add_user(Client to_add);
		void	    rm_user(Client to_rm);
        void        add_operator(Client to_set);
        void        rm_operator(Client to_set);
        void        get_clients(void);
		
		bool		get_key_set();
		std::string get_keypass();
		void		set_keypass(Client c_client);
		bool		check_keypass(Client c_client);
        bool	check_kp(std::string kp_to_check);

		std::vector<Client> client_list;
        std::vector<Client> op_clients;

	private :
		std::string channel_name;
		int         creator_fd;
		std::string description;
		int			user_max;	

		bool		keypass_set;
		std::string channel_keypass;
		

		std::string red;
		std::string green;
		std::string white;
		std::string yellow;
};

#endif