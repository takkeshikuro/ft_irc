/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 01:45:49 by keshikuro         #+#    #+#             */
/*   Updated: 2024/03/28 09:24:29 by keshikuro        ###   ########.fr       */
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

		void	    rm_backslash_n(std::string &s);
		std::string &get_name();
		std::string &get_description();
		void	    set_description(std::string &s);
        void        send_to_all(std::string buffer, Client c_client);
        void        send_string(std::string tosend, std::string nick);
        
		void        add_user(Client to_add);
		void	    rm_user(Client to_rm);
        void        add_operator(Client to_set);
        void        rm_operator(Client to_set);
		size_t			get_user_max();
        int         get_size(void);
        void        get_clients();
		
		bool		get_key_set(); // to know if keypass set or not
		void		set_key_set(); // to modify keypass_set
		std::string get_keypass(); // to get the string keypass
		void		set_keypass(Client c_client); // to set the string keypass
		bool		check_keypass(Client c_client);
        bool		check_kp(std::string kp_to_check);

        int         get_limit();
        void        set_limit(int lim);

		bool		get_invite_set();
		void		set_invite_set();

	//	std::vector<Client> &Channel::getClientList();
		std::vector<Client> client_list;
        std::vector<Client> op_clients;

	private :
		std::string channel_name;
		int         creator_fd;
		std::string description;
		size_t			user_max;	
        int         limit;
		bool		keypass_set;
		std::string channel_keypass;
		bool		invite_mode;

		std::string red;
		std::string green;
		std::string white;
		std::string yellow;
};

#endif