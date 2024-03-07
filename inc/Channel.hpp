/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 01:45:49 by keshikuro         #+#    #+#             */
/*   Updated: 2024/03/07 04:01:45 by keshikuro        ###   ########.fr       */
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

	//	void    new_user_update(Client &new_client);
	//	void    exit_user_update(Client &client);
		void	rm_backslash_n(std::string &s);
		std::string &get_name();
		std::string &get_description();
		void	set_description(std::string &s);
	private :
		std::string channel_name;
	//	std::string topic_name;
		std::list<Client> client_list;
		int creator_fd;
		std::string description;

		std::string channel_pw;
		int			user_max;




		std::string red;
};

#endif