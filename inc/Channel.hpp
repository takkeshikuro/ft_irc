/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marecarrayan <marecarrayan@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 01:45:49 by keshikuro         #+#    #+#             */
/*   Updated: 2024/03/18 18:03:47 by marecarraya      ###   ########.fr       */
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
        void        send_string(std::string tosend);
        void        add_user(Client to_add);
		void	    rm_user(Client to_rm);
        void        add_operator(Client to_set);
        void        rm_operator(Client to_set);
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
		std::vector<Client> client_list;
        std::vector<Client> op_clients;

	private :
		std::string channel_name;
		int         creator_fd;
		std::string description;
		int			user_max;	
        int         limit;
		bool		keypass_set;
		std::string channel_keypass;
		

		std::string red;
		std::string green;
		std::string white;
		std::string yellow;
};

#endif