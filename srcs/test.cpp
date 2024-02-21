#include "../inc/irc.hpp"

int main() 
{
	// Création du socket du serveur
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
	{
		std::cerr << "Erreur lors de la création du socket" << std::endl;
		return 1;
	}

	// Configuration de l'adresse du serveur
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(6667); // Port IRC standard

	// Lier le socket à l'adresse et au port
	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
	{
		std::cerr << "Erreur lors de la liaison du socket à l'adresse" << std::endl;
		close(serverSocket);
		return 1;
	}

	// Mettre le socket en mode écoute
	if (listen(serverSocket, MAX_CLIENTS) == -1)
	{
		std::cerr << "Erreur lors de la mise en mode écoute du socket" << std::endl;
		close(serverSocket);
		return 1;
	}

	// Création d'un vecteur de pollfd pour stocker les descripteurs de fichier surveillés par poll()
	std::vector<pollfd> fds(1, {serverSocket, POLLIN});

	char buffer[BUFFER_SIZE];

	while (true)
	{
		// Utilisation de poll() pour attendre l'activité sur les descripteurs de fichier
		int activity = poll(fds.data(), fds.size(), -1);
		if (activity < 0)
		{
			std::cerr << "Erreur lors de la sélection" << std::endl;
			continue;
		}

		// Vérification de l'activité sur le socket du serveur (nouvelle connexion entrante)
		if (fds[0].revents & POLLIN)
		{
			// Nouvelle connexion entrante
			int newSocket = accept(serverSocket, nullptr, nullptr);
			if (newSocket == -1) {
				std::cerr << "Erreur lors de l'acceptation de la connexion entrante" << std::endl;
			} else {
				// Ajout du nouveau descripteur de fichier au vecteur surveillé par poll()
				fds.push_back({newSocket, POLLIN});
				std::cout << "Nouvelle connexion acceptée" << std::endl;
			}
		}

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
					// Écho du message au client
					send(fds[i].fd, buffer, strlen(buffer), 0);
				}
			}
		}
	}

	// Fermeture du socket du serveur
	close(serverSocket);

	return 0;
}
