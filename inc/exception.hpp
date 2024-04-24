/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 01:54:21 by keshikuro         #+#    #+#             */
/*   Updated: 2024/03/01 09:00:23 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include "irc.hpp"

class SocketCreationError : public std::exception {
	virtual const char *what() const throw() {
		return "(socket()) Error during socket creation.";
	}
};

class SocketLinkingError : public std::exception {
	virtual const char *what() const throw() {
		return "(bind()) Error during linking socket and address.";
	}
};

class SocketListeningError : public std::exception {
	virtual const char *what() const throw() {
		return "(listen()) Error during socket listening.";
	}
};

class WrongPortError : public std::exception {
	virtual const char *what() const throw() {
		return "Wrong port selection, try 6667(IRC).";
	}
};

class NeedPasswordError : public std::exception {
	virtual const char *what() const throw() {
		return "Need password selection, pls try again.";
	}
};

class SetOptionSO_REUSEADDRError : public std::exception {
	virtual const char *what() const throw() {
		return "Failed to set option (SO_REUSEADDR) on socket.";
	}
};

class SetOptionO_NONBLOCKError : public std::exception {
	virtual const char *what() const throw() {
		return "(fcntl()) Failed to set option (O_NONBLOCK) on socket.";
	}
};

class PollUsageError : public std::exception {
	virtual const char *what() const throw() {
		return "poll() function failed.";
	}
};

#endif