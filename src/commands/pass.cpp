#include "../../inc/Server.hpp"


/**
 * @brief The PASS command is used to set a ‘connection password’.
 * The password supplied must match the one defined in the server configuration.
 * 	
 * Syntax: PASS <password>
 * 
 * Numeric replies:
 * 	ERR_NEEDMOREPARAMS (461)
 * 	ERR_ALREADYREGISTERED (462)
 * 	ERR_PASSWDMISMATCH (464)
 * 
 * Example :
 *  [CLIENT] /PASS secretpassword
 */

int	Server::pass(std::string buffer, Client c_client)
{
	std::string password_check;
	size_t pos = buffer.find("PASS");

	if (c_client.is_registred == true)
	{
		size_t size = ERR_ALREADYREGISTERED(c_client.getNickname()).size();
		send(c_client.get_client_fd(), ERR_ALREADYREGISTERED(c_client.getNickname()).c_str(), size, 0);
		return FAILURE;
	}
	if (pos != std::string::npos)
	{
		std::size_t start_pass = (buffer.find(' ', pos) + 1);
		std::size_t end_pass = (buffer.find('\n', start_pass) - 1);
		password_check = buffer.substr(start_pass, (end_pass - start_pass));	
		std::cout << YEL << "[password given = "<<password_check<<"]\n"<< RESET;
		if (c_client.check_password(password_check) == true)
			return SUCCESS;
		else 
		{
			std::cout << RED << "[authentification failed]\n"<< RESET;
			size_t size = ERR_PASSWDMISMATCH(c_client.getNickname()).size();
			send(c_client.get_client_fd(), ERR_PASSWDMISMATCH(c_client.getNickname()).c_str(), size, 0);
			return FAILURE;	
		}
	}
	else {
		std::cout << YEL << "[no password given by user]\n"<< RESET;
		size_t size = ERR_NEEDMOREPARAMS(c_client.getNickname(), "add [-w <password>]").size();
		send(c_client.get_client_fd(), ERR_NEEDMOREPARAMS(c_client.getNickname(), "add [-w <password>]").c_str(), size, 0);
		return FAILURE;
	}
}