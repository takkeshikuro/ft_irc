/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 03:17:35 by keshikuro         #+#    #+#             */
/*   Updated: 2024/02/28 03:26:08 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/User.hpp"

User::User(int socket, std::string &nickname, std::string ip_addr) :
socket_usr(socket), nickname(nickname), ip_addr(ip_addr) 
{
	std::cout << "new user co : " << nickname << std::endl;
}

User::~User() {
	std::cout << "user " << nickname << " is gone\n";
}

int User::getSocket() const {
	return socket_usr;
}

std::string& User::getNickname() {
	return nickname;
}

std::string& User::getIPAddress() {
	return ip_addr;
}