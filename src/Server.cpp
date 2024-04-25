/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 01:24:03 by keshikuro         #+#    #+#             */
/*   Updated: 2024/03/02 08:53:38 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

bool Server::signal = false;

Server::Server() : port(6667), password("default") {}

Server::Server(std::string passwd, struct tm *timeinfo) : port(6667), password(passwd) 
{
	this->setDatetime(timeinfo);
	green = "\e[1;32m";
	white = "\e[0;37m";
	red = "\e[1;31m";
	yellow = "\e[1;33m";
}

Server::~Server() {}

void    Server::configuration() 
{
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(this->port); // Port IRC standard
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
		throw SocketCreationError();

	int en = 1;
	if(setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
		throw SetOptionSO_REUSEADDRError();
	if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) == -1) 
		throw SetOptionO_NONBLOCKError();
	if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
		throw SocketLinkingError();
	if (listen(serverSocket, MAX_CLIENTS) == -1)
		throw SocketListeningError();
	
	default_channel_creation();
	NewPoll.fd = serverSocket; //-> add the server socket to the pollfd
	NewPoll.events = POLLIN; //-> set the event to POLLIN for reading data
	NewPoll.revents = 0; //-> set the revents to 0
	fds.push_back(NewPoll); //-> add the server socket to the pollfd
}

void	Server::launch_server() 
{	
	std::cout << GRE << "Server <" << serverSocket << "> Connected" << WHI << std::endl;
 	std::cout << "Waiting to accept a connection...\n";

	while (this->signal == false)
	{
		// Utilisation de poll() pour attendre l'activité sur les descripteurs de fichier
		if ((poll(&fds[0], fds.size(), -1) == -1) && this->signal == false)
			throw PollUsageError();
		
		//Nested Loop: Iterates through all fd (fds) to check if there is data to read.
		for (size_t i = 0; i < fds.size(); i++) //-> check all file descriptors
		{
			if (fds[i].revents & POLLIN)//-> check if there is data to read
			{
				if (fds[i].fd == serverSocket)
					manage_new_client();
				else
					manage_new_data(fds[i].fd);
			}
			else if (fds[i].revents & POLLOUT) // = "Alert me when I can send() data to this socket without blocking."
			{
				if (handlePolloutEvent(fds[i].fd) == BREAK)
					break;
			}
			else if (fds[i].revents & POLLERR)
			{
				if (handlePollerEvent(fds[i]) == BREAK)
					break ;
			}
		}
	}
	close_fds(); //close the fds when server stops
}

int	Server::manage_new_client() 
{
	Client				new_client(password, datetime);
	struct sockaddr_in	ClientAddr;
	struct pollfd		NewPoll;
	socklen_t			len = sizeof(ClientAddr);
	int 				incoming_fd;
	
	incoming_fd = accept(serverSocket, (sockaddr *)&(ClientAddr), &len);
	if (incoming_fd == -1) {
		std::cerr << RED << "[Server] Accept() failed" << RESET << std::endl;
		return (CONTINUE);
	}
	if (fds.size() > MAX_CLIENT_NB)
		return tooManyClients(incoming_fd);
	if (fcntl(incoming_fd, F_SETFL, O_NONBLOCK) == -1) //set the socket option (O_NONBLOCK) for non-blocking socket
		throw SetOptionO_NONBLOCKError();
	NewPoll.fd = incoming_fd;//-> add the client socket to the pollfd
	NewPoll.events = POLLIN; // set the event to POLLIN for reading data
	NewPoll.revents = 0;
	new_client.set_client_fd(incoming_fd); //-> set the client file descriptor
	new_client.set_IpAdd(inet_ntoa((ClientAddr.sin_addr))); //-> convert the ip address to string and set it
	client_vec.push_back(new_client); //-> add the client to the vector of clients
	fds.push_back(NewPoll); //-> add the client socket to the pollfd
	
	std::cout << GRE << "\nNouvelle connexion acceptée\n" << "+Client <" << incoming_fd << "> Connected\n" << RESET;
	if (check_irssi_entrance(incoming_fd)) {
		client_vec.back().set_is_irssi();
		if (client_vec.back().client_starting_point_irssi(this->irssi_base, *this) == FAILURE) {
			clear_clients(incoming_fd);
			return FAILURE;
		}
	}
	else {
		if (client_vec.back().client_starting_point() == FAILURE) {
			clear_clients(incoming_fd);
			return FAILURE;	
		}
	}
	return SUCCESS;
}

void	Server::manage_new_data(int fd) 
{
	memset(buffer, 0, sizeof(buffer)); //-> clear the buffer
	ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1 , 0); //-> receive the data
	unsigned long int i = 0;

	while (i < this->client_vec.size()) 
	{
		if (client_vec[i].get_client_fd() == fd)
			break ;
		i++;
	}
	Client current_client = client_vec[i];

	// check if the client disconnected
	if(bytes <= 0) 
	{ 
		std::cout << RED << "User <" << current_client.getUsername() << "> Disconnected" << WHI << std::endl;
		clear_clients(fd);
		close(fd);
	}
	else
	{ 
		buffer[bytes] = '\0';
		std::cerr << "[debug buffer] = " << buffer << "]\n"; 
		if (is_command(buffer, current_client))
			return ;
		if (current_client.get_is_irssi() == true) {
			is_irssi_command(buffer, current_client);
			return ;
		}
		if (current_client.in_channel)
		{
			std::string buf(buffer);
			buf  = "PRIVMSG " + current_client.get_current_chan() + " :" + buffer;
			for(size_t k = 0; k < buf.size(); k++)
			{
				if (buf[k] == '\n')
					buf[k] = '\r';
			}
			buf = buf + '\n';
			msg(buf, current_client);
		}
		else
			std::cout << YEL << current_client.getNickname() << ": " << WHI << buffer;
	}
}

void Server::clear_clients(int fd)
{
	int index;
	for(size_t i = 0; i < fds.size(); i++)  // rm the client from the pollfd 
	{	
		if (fds[i].fd == fd) {
			fds.erase(fds.begin() + i); 
			break;
		}
	}
	for(size_t i = 0; i < client_vec.size(); i++)  // rm the client from the vector 
	{
		if (client_vec[i].get_client_fd() == fd) {
			client_vec.erase(client_vec.begin() + i);
			break;
		}
 	}
	for (size_t i = 0; i < channel_vec.size(); i++)  //rm the client from channels
	{
		index = index_channel_fd(fd, channel_vec[i]);
		if (index != -1)
			channel_vec[i].client_list.erase(channel_vec[i].client_list.begin() + index);
	}
	for (size_t i = 0; i < channel_vec.size(); i++)  //rm the client from operator vectors 
	{
		index = index_operator_fd(fd, channel_vec[i]);
		if (index != -1)
			channel_vec[i].op_clients.erase(channel_vec[i].op_clients.begin() + index);
	}
	std::cout << GREEN << "[info] client cleared.\n" << RESET;
}
