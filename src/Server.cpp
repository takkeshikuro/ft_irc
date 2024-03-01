/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 01:24:03 by keshikuro         #+#    #+#             */
/*   Updated: 2024/03/01 07:04:33 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

bool Server::signal = false;

Server::Server() : port(6667) {}

Server::~Server() {}

void Server::SignalHandler(int sig)
{
	(void)sig;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::signal = true; //-> set the static boolean to true to stop the server
}

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
		throw SocketCreationError();
		//throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
	if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) == -1) 
		throw SocketCreationError();
		//throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
	if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
		throw SocketLinkingError();
	if (listen(serverSocket, MAX_CLIENTS) == -1)
		throw SocketListeningError();
	
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
			std::cerr << "poll() failed";
		
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
		}
	}
	close(serverSocket);
}

void	Server::manage_new_client() 
{
	Client			new_client;
	struct sockaddr_in	ClientAddr;
	struct pollfd	NewPoll;
	socklen_t		len = sizeof(ClientAddr);
	
	int incoming_fd = accept(serverSocket, (sockaddr *)&(ClientAddr), &len);
	if (incoming_fd == -1)
		std::cerr << "Erreur lors de l'acceptation de la connexion entrante" << std::endl;
	
	if (fcntl(incoming_fd, F_SETFL, O_NONBLOCK) == -1) //set the socket option (O_NONBLOCK) for non-blocking socket
		throw SocketLinkingError(); // a modif "fcntl() failed"
	
	NewPoll.fd = incoming_fd;//-> add the client socket to the pollfd
	NewPoll.events = POLLIN; // set the event to POLLIN for reading data
	NewPoll.revents = 0;
	
	new_client.set_client_fd(incoming_fd); //-> set the client file descriptor
	new_client.set_IpAdd(inet_ntoa((ClientAddr.sin_addr))); //-> convert the ip address to string and set it
	
	client_vec.push_back(new_client); //-> add the client to the vector of clients
	fds.push_back(NewPoll); //-> add the client socket to the pollfd
	
	std::cout << GRE << "Client <" << incoming_fd << "> Connected" << WHI << std::endl;
	std::cout << "Nouvelle connexion acceptée" << std::endl;

	std::string message001 = ":irc.server.com 001 utilisateur :Bienvenue sur le serveur IRC irc.server.com\r\n";
	send(incoming_fd, message001.c_str(), message001.length(), 0);
}

void	Server::manage_new_data(int fd) 
{
	memset(buffer, 0, sizeof(buffer)); //-> clear the buffer
	ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1 , 0); //-> receive the data

	if(bytes <= 0)//-> check if the client disconnected
	{ 
		std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
		clear_clients(fd);
		close(fd); //-> close the client socket
	}
	else//-> print the received data
	{ 
		buffer[bytes] = '\0';
		std::cout << YEL << "Client <" << fd << "> Data: " << WHI << buffer;
		// received data: parse, check, authenticate, handle the command
	}
}

void Server::clear_clients(int fd)
{
	for(size_t i = 0; i < fds.size(); i++)// rm the client from the pollfd
	{ 
		if (fds[i].fd == fd) 
		{
			fds.erase(fds.begin() + i); 
			break;
		}
	}
	for(size_t i = 0; i < client_vec.size(); i++)// rm the client from the vector 
	{ 
		if (client_vec[i].get_client_fd() == fd)
		{
			client_vec.erase(client_vec.begin() + i);
			break;
		}
 	}
}

void Server::close_fds()
{
	for(size_t i = 0; i < client_vec.size(); i++) { // close all the clients into vector
		std::cout << RED << "Client <" << client_vec[i].get_client_fd() << "> Disconnected" << WHI << std::endl;
		close(client_vec[i].get_client_fd());
	}
	if (serverSocket != -1)// close the server socket
	{ 
		std::cout << RED << "Server <" << serverSocket << "> Disconnected" << WHI << std::endl;
		close(serverSocket);
 	}
}