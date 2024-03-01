/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 03:17:35 by keshikuro         #+#    #+#             */
/*   Updated: 2024/03/01 06:57:09 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client::Client() {}

Client::~Client() {
	std::cout << "Client " << nickname << " is gone\n";
}

int Client::getSocket() const {
	return socket_usr;
}

std::string& Client::getNickname() {
	return nickname;
}

std::string& Client::getIPAddress() {
	return ip_addr;
}