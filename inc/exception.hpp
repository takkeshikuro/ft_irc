/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 01:54:21 by keshikuro         #+#    #+#             */
/*   Updated: 2024/02/28 02:04:31 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include "irc.hpp"

class SocketCreationError : public std::exception {
	virtual const char *what() const throw() {
		return "Error during socket creation.";
	}
};

class SocketLinkingError : public std::exception {
	virtual const char *what() const throw() {
		return "Error during linking socket and address.";
	}
};

class SocketListeningError : public std::exception {
	virtual const char *what() const throw() {
		return "Error during socket listening.";
	}
};

class WrongPortError : public std::exception {
	virtual const char *what() const throw() {
		return "Wrong port selection, try 6667(IRC).";
	}
};

class WrongPasswordError : public std::exception {
	virtual const char *what() const throw() {
		return "Wrong password selection, pls try again.";
	}
};


#endif