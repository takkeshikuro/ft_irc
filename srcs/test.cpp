#include "../inc/irc.hpp"

int main() {
    // Création du socket du serveur
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Erreur lors de la création du socket" << std::endl;
        return 1;
    }

    // Configuration de l'adresse du serveur
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(6667); // Port IRC standard

    // Lier le socket à l'adresse et au port
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Erreur lors de la liaison du socket à l'adresse" << std::endl;
        close(serverSocket);
        return 1;
    }

    // Mettre le socket en mode écoute
    if (listen(serverSocket, MAX_CLIENTS) == -1) {
        std::cerr << "Erreur lors de la mise en mode écoute du socket" << std::endl;
        close(serverSocket);
        return 1;
    }

    std::vector<int> clientSockets;
    char buffer[BUFFER_SIZE];

    while (true) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(serverSocket, &readfds);

        int maxSocket = serverSocket;

        for (size_t i = 0; i < clientSockets.size(); ++i) {
            int socket = clientSockets[i];
            FD_SET(socket, &readfds);
            if (socket > maxSocket) {
                maxSocket = socket;
            }
        }

        // Attendre une activité sur l'un des sockets
        int activity = select(maxSocket + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR)) {
            std::cerr << "Erreur lors de la sélection" << std::endl;
            continue;
        }

        // Nouvelle connexion entrante
        if (FD_ISSET(serverSocket, &readfds)) {
            int newSocket = accept(serverSocket, NULL, NULL);
            if (newSocket == -1) {
                std::cerr << "Erreur lors de l'acceptation de la connexion entrante" << std::endl;
            }
            else {
                clientSockets.push_back(newSocket);
                std::cout << "Nouvelle connexion acceptée" << std::endl;
            }
        }

        // Traiter les données des clients existants
        for (std::vector<int>::iterator it = clientSockets.begin(); it != clientSockets.end(); ++it) {
            int clientSocket = *it;
            if (FD_ISSET(clientSocket, &readfds)) {
                int valread = read(clientSocket, buffer, BUFFER_SIZE);
                if (valread == 0) {
                    // Déconnexion du client
                    std::cout << "Client déconnecté" << std::endl;
                    close(clientSocket);
                    it = clientSockets.erase(it);
                } else {
                    // Traiter les données reçues
                    buffer[valread] = '\0'; // Assurer la terminaison de la chaîne
                    std::cout << "Message du client : " << buffer << std::endl;
                    // Écho du message au client
                    send(clientSocket, buffer, strlen(buffer), 0);
                }
            }
        }
    }

    // Fermeture du socket du serveur
    close(serverSocket);

    return 0;
}
