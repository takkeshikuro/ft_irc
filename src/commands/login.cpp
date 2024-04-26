#include "../../inc/Server.hpp"

bool	invalid_char(std::string nickname);
bool	nickname_already_used(std::vector<Client> &client_vec, std::string name);
std::vector<std::string> ft_split(const std::string& str, const std::string& delimiters);



int    Server::login(std::string buffer, Client &c_client)
{
	std::string cmd;
	std::string passwd;
	std::string nick;
	std::string user;
	std::vector<std::string> arg;

 	if (c_client.get_is_irssi() == true)
		return FAILURE;

	arg = ft_split(buffer, " ");
	if (arg.size() == 4)
	{
		cmd = arg[0];
		if (arg[1].empty() || arg[2].empty() || arg[3].empty()) {
			std::string nmp = "Need more param : /login <password> <realname> <nickname>\r\n";
			send(c_client.get_client_fd(), nmp.c_str(), nmp.size(), 0);
			return FAILURE;
		}
		passwd = arg[1];
		if (c_client.check_password(passwd) == false) {
			std::string invpass = "Password selection mismatch.\r\n";
			send(c_client.get_client_fd(), invpass.c_str(), invpass.size(), 0);
			return FAILURE;
		}
		user = arg[2];
		if (invalid_char(user)) {
			std::string invuser = "Invalid char in username selection.\r\n";
			send(c_client.get_client_fd(), invuser.c_str(), invuser.size(), 0);
			return FAILURE;
		}
		nick = arg[3];
		if (invalid_char(nick)) {
			std::string invnick = "Invalid char in nickname selection.\r\n";
			send(c_client.get_client_fd(), invnick.c_str(), invnick.size(), 0);
			return FAILURE;
		}
		else if (nickname_already_used(this->client_vec, nick) == true) {
			std::string nickinuse = "Nickname already in use.\r\n";
			send(c_client.get_client_fd(), nickinuse.c_str(), nickinuse.size(), 0);
			return FAILURE;
		}
		c_client.setUsername(user);
		c_client.setNickname(nick);
	}
	else {
		std::string nmp = "Need more param : /login <password> <realname> <nickname>\r\n";
		send(c_client.get_client_fd(), nmp.c_str(), nmp.size(), 0);
		return FAILURE;
	}
	return SUCCESS;
}
