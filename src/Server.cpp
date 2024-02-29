/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 01:24:03 by keshikuro         #+#    #+#             */
/*   Updated: 2024/02/29 14:27:00 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server() {}

Server::~Server() {}

void    Server::configuration() 
{
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
		throw SocketCreationError();
	// Configuration de l'adresse du serveur
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(6667); // Port IRC standard
	// Lier le socket à l'adresse et au port
	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
		throw SocketLinkingError();
		close(serverSocket);
	}
	// Mettre le socket en mode écoute
	if (listen(serverSocket, MAX_CLIENTS) == -1)
	{
		throw SocketListeningError();
		close(serverSocket);
	}
}

void	Server::launch_server() 
{
//	std::vector<pollfd> vector(1, {serverSocket, POLLIN});
// modif pour +98
	std::vector<pollfd> vector;
	pollfd fd;
	fd.fd = serverSocket;
	fd.events = POLLIN;
	vector.push_back(fd);
//
	fds = vector;

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
		// Nouvelle connexion entrante
		int newSocket = accept(serverSocket, NULL, NULL);
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