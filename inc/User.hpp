/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 03:12:39 by keshikuro         #+#    #+#             */
/*   Updated: 2024/02/28 03:26:06 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include "irc.hpp"

class User {
	public :
		User(int socket, std::string &nickname, std::string ip_addr);
		~User();

		int getSocket() const;
		std::string& getNickname();
		std::string& getIPAddress();
	
	private : 
		int socket_usr;
		std::string nickname;
		std::string ip_addr;
		
};

#endif