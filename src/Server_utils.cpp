#include "../inc/Server.hpp"


void	Server::setDatetime(struct tm *timeinfo)
{
	char buffer[80];

	strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
  	std::string str(buffer);

	datetime = str;
}

void Server::SignalHandler(int sig)
{
	(void)sig;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::signal = true; //  to stop the server
}

int	Server::handlePolloutEvent(const int current_fd)
{
	for (size_t i = 0; i < this->client_vec.size(); ++i) 
	{
		if (client_vec[i].get_client_fd() == current_fd)
		{
			if (client_vec[i].get_quit_status() == true) {
				std::cout << RED << "User <" << client_vec[i].getNickname() << "> Disconnected (pollout)" << WHI << std::endl;
				clear_clients(current_fd);
				close(current_fd);
				return (BREAK);
			}
		}
	}
	std::cout << "[Server] Did not found connexion to client sorry" << std::endl;
	return (SUCCESS);
}

int	Server::handlePollerEvent(pollfd &it)
{
	if (it.fd == serverSocket)
	{
		std::cerr << RED << "[Server] Listen socket error" << RESET << std::endl;
		return (FAILURE);
	}
	else
	{
		std::cout << RED << "User <" << it.fd << "> Disconnected (poller)" << WHI << std::endl;
		clear_clients(it.fd);
		close(it.fd);
		return (BREAK);
	}
}

int Server::tooManyClients(int client_socket)
{
	std::cout << RED << ERR_FULL_SERV << RESET << std::endl;
	send(client_socket, ERR_FULL_SERV, strlen(ERR_FULL_SERV) + 1, 0);
	close(client_socket);
	return SUCCESS;
}

void	Server::user_data_parsing(Client &c_client, std::string &buffer, int i)
{
	char* c_buff = new char[buffer.length() + 1];
	strcpy(c_buff, buffer.c_str());

	if (c_client.is_registred == false && c_client.get_is_irssi() == false)
	{
		if (!strncmp(buffer.c_str(), "/login", 6)) 
		{
			if (login(buffer, client_vec[i]) == SUCCESS) {
				client_vec[i].is_registred = true;
				std::cout << GRE << "Authentification ok : login over\n" << RESET;
				std::string login_ok = green +"New account creation ok ! Welcome " + yellow + client_vec[i].getNickname() + "\r\n" + white;
				send(client_vec[i].get_client_fd(), login_ok.c_str(), login_ok.size(), 0);
			}
			else {
				std::string login_ko = red + "Please try again.\r\n" + white;
				send(client_vec[i].get_client_fd(), login_ko.c_str(), login_ko.size(), 0);
			}
		}
		else {
			std::string login_ko = red + "Try : /login <password> <realname> <nickname>\r\n" + white;
			send(client_vec[i].get_client_fd(), login_ko.c_str(), login_ko.size(), 0);
		}
	}
	else if (is_command(c_buff, c_client)) 
	{
		delete[] c_buff;
		return ;
	}
	if (c_client.get_is_irssi() == true) 
	{
		is_irssi_command(c_buff, c_client);
		delete[] c_buff;
		return ;
	}
	if (c_client.in_channel) 
	{
		std::string buf(buffer);
		buf  = "PRIVMSG " + c_client.get_current_chan() + " :" + buf;
		for(size_t k = 0; k < buf.size(); k++) {
			if (buf[k] == '\n')
				buf[k] = '\r';
		}
		buf = buf + '\n';
		msg(buf, c_client);
	}
	delete[] c_buff;
	return ;
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
	Channel random("#random");
	channel_vec.push_back(random);
	std::string bio_r = "~default #random channel~";
	channel_vec.back().set_description(bio_r);
	
	Channel announcements("#announcements");
	channel_vec.push_back(announcements);
	std::string bio_a = "~default #announcements channel to stay informed~";
	channel_vec.back().set_description(bio_a);
}

void	Server::check_bot(std::string buffer, std::string &cmd_string)
{
	const char* s1 = "time";

	if (!strncmp(s1, buffer.c_str(), 4))
		cmd_string = "TIME";
	return ;
}

int	Server::index_client(std::string nick)
{
	for (size_t i = 0; i < client_vec.size(); i++)
	{
		if (client_vec[i].getNickname() == nick)
			return i;
	}
	return -1;
}
