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

Server::Server(std::string passwd) : port(6667), password(passwd) {
	green = "\e[1;32m";
	white = "\e[0;37m";
	red = "\e[1;31m";
	yellow = "\e[1;33m";
}

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
		}
	}
	close_fds(); //close the fds when server stops
}

void	Server::manage_new_client() 
{
	Client			new_client(password);
	struct sockaddr_in	ClientAddr;
	struct pollfd	NewPoll;
	socklen_t		len = sizeof(ClientAddr);
	
	int incoming_fd = accept(serverSocket, (sockaddr *)&(ClientAddr), &len);
	if (incoming_fd == -1)
		std::cerr << "Erreur lors de l'acceptation de la connexion entrante" << std::endl;
	
	if (fcntl(incoming_fd, F_SETFL, O_NONBLOCK) == -1) //set the socket option (O_NONBLOCK) for non-blocking socket
		throw SetOptionO_NONBLOCKError();
	
	NewPoll.fd = incoming_fd;//-> add the client socket to the pollfd
	NewPoll.events = POLLIN; // set the event to POLLIN for reading data
	NewPoll.revents = 0;
	
	new_client.set_client_fd(incoming_fd); //-> set the client file descriptor
	new_client.set_IpAdd(inet_ntoa((ClientAddr.sin_addr))); //-> convert the ip address to string and set it
	
	client_vec.push_back(new_client); //-> add the client to the vector of clients
	fds.push_back(NewPoll); //-> add the client socket to the pollfd
	std::cout << GRE << "\nNouvelle connexion acceptée\n" << "+Client <" << incoming_fd << "> Connected" << WHI << std::endl;
	if (check_irssi_entrance(incoming_fd))
	{
		client_vec.back().set_is_irssi();
		client_vec.back().client_starting_point_irssi(this->irssi_base);
	}
	else
		client_vec.back().client_starting_point();
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
	if(bytes <= 0)//-> check if the client disconnected
	{ 
		std::cout << RED << "User <" << current_client.getUsername() << "> Disconnected" << WHI << std::endl;
		clear_clients(fd);
		close(fd); //-> close the client socket
	}
	else//-> print the received data
	{ 
		buffer[bytes] = '\0';
		std::cerr << "debug buffer = " << buffer << "\n";
		
		int iscmd = is_command(buffer, current_client);
		if (iscmd)
			return ;
		else if (iscmd == -1) {
			if (is_irssi_command(buffer, current_client))
				return ;
		}
		if (current_client.in_channel)
		{
			size_t j;
			for (j = 0; j < channel_vec.size(); j++)
			{
				if (channel_vec[j].get_name() == current_client.get_current_chan())
					break ;
			}
			channel_vec[j].send_to_all(buffer, current_client);
		}
		else
			std::cout << YEL << current_client.getNickname() << ": " << WHI << buffer;
		// received data: parse, check, authenticate, handle the command
	}
}

void Server::clear_clients(int fd)
{
    int index;

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
}

void Server::close_fds()
{
	for(size_t i = 0; i < client_vec.size(); i++)  // close all the clients into vector
	{
		std::cout << RED << "Client <" << client_vec[i].get_client_fd() << "> Disconnected" << WHI << std::endl;
		close(client_vec[i].get_client_fd());
	}
	if (serverSocket != -1)// close the server socket
	{
		std::cout << RED << "Server <" << serverSocket << "> Disconnected" << WHI << std::endl;
		close(serverSocket);
 	}
}

void	Server::default_channel_creation()
{
	Channel random("random");
	channel_vec.push_back(random);
	std::string bio_r = "default #random channel";
	channel_vec.back().set_description(bio_r);
	
	Channel announcements("announcements");
	channel_vec.push_back(announcements);
	std::string bio_a = "default #announcements channel to stay informed.";
	channel_vec.back().set_description(bio_a);
}
