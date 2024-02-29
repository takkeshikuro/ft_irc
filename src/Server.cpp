/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 01:24:03 by keshikuro         #+#    #+#             */
/*   Updated: 2024/02/29 21:35:27 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

bool Server::signal = false;

Server::Server() : port(6667) {}

Server::~Server() {}

// void Server::SignalHandler(int sig)
// {
// 	(void)sig;
// 	std::cout << std::endl << "Signal Received!" << std::endl;
// 	Server::signal = true; //-> set the static boolean to true to stop the server
// }

void    Server::configuration() 
{
	struct pollfd NewPoll;

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
	
	//pollfd fd;
	//fd.fd = serverSocket;
	//fd.events = POLLIN;
	///fds.push_back(fd);
//
	while (true)
	{
		// Utilisation de poll() pour attendre l'activité sur les descripteurs de fichier
		int activity = poll(fds.data(), fds.size(), -1);
		if (activity < 0) {
			std::cerr << "Erreur lors de la sélection" << std::endl;
			continue;
		}
		manage_new_connection();
		manage_client_data();
	}
	close(serverSocket);
}

void	Server::manage_new_connection() 
{
	// Vérification de l'activité sur le socket du serveur (nouvelle connexion entrante)
	if (fds[0].revents & POLLIN) 
	{
		
		struct sockaddr addrClient;
		socklen_t csize = sizeof(addrClient);
		// Nouvelle connexion entrante
		int newSocket = accept(serverSocket, (struct sockaddr *)&addrClient, &csize);
		if (newSocket == -1)
			std::cerr << "Erreur lors de l'acceptation de la connexion entrante" << std::endl;
		else {
			// Ajout du nouveau descripteur de fichier au vecteur surveillé par poll()
	//		fds.push_back({newSocket, POLLIN});
	// modif pour +98
			pollfd newDescriptor;
			newDescriptor.fd = newSocket;
			newDescriptor.events = POLLIN;
			fds.push_back(newDescriptor);
	/////		
			std::cout << "Nouvelle connexion acceptée" << std::endl;

		 	std::string message001 = ":irc.server.com 001 utilisateur :Bienvenue sur le serveur IRC irc.server.com\r\n";
            send(newSocket, message001.c_str(), message001.length(), 0);
		}
	}
}

void	Server::manage_client_data() 
{
// Parcours des descripteurs de fichier surveillés pour traiter les données reçues des clients connectés
	for (size_t i = 1; i < fds.size(); ++i)
	{
		if (fds[i].revents & POLLIN) 
		{
			int valread = read(fds[i].fd, buffer, BUFFER_SIZE);
			if (valread <= 0) {
				// Déconnexion du client
				std::cout << "Client déconnecté" << std::endl;
				close(fds[i].fd);
				fds.erase(fds.begin() + i);
				--i;
			} 
			else 
			{
				// Traiter les données reçues
				buffer[valread] = '\0'; // Assurer la terminaison de la chaîne
				std::cout << "Message du client : " << buffer << std::endl;
				
				if (strncmp(buffer, "NICK", 4) == 0) {
                    // Extraire le pseudo du message
                    std::string nick = buffer + 5; // Supposer que le message est de la forme "NICK <pseudo>\r\n"

                    // Envoyer une confirmation au client si nécessaire
                    std::string confirmation = "NICK " + nick + " défini avec succès\r\n";
                    send(fds[i].fd, confirmation.c_str(), confirmation.length(), 0);
				}
				else
					send(fds[i].fd, buffer, strlen(buffer), 0);
			}
		}
	}
}