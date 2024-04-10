#include "../../inc/Server.hpp"

std::string find_username(std::string total_arg);
std::string	find_realname(std::string total_arg);

/**
 * @brief The USER command is used at the beginning of a connection to specify 
 * 		  the username and realname of a new user.
 * 		
 * 	The minimum length of <username> is 1, ie. it MUST NOT be empty. 
 * 	If it is empty, the server SHOULD reject the command with ERR_NEEDMOREPARAMS.
 * 
 *  If a client tries to send the USER command after they have already completed
 *  registration with the server, the ERR_ALREADYREGISTERED reply should be sent
 *  and the attempt should fail.
 * 	
 *  SYNTAX : USER <username> 0 * <realname>
 * 
 * 	Numeric Replies:
 * 		ERR_NEEDMOREPARAMS (461)
 * 		ERR_ALREADYREGISTERED (462)
 * 
 *  Examples:
 * 	[Client] USER keshikuro keshikuro 127.0.0.1 : Takeshi Kuro
 * 	=> Username is keshikuro, Realname is Takeshi Kuro
 * USER keshikuro keshikuro 127.0.0.1 :keshikuro
 */

void    Server::user(std::string buffer, Client c_client)
{
	std::cout << "COUCOU {user.cpp}\n";
    std::string total_arg;
    if (buffer[buffer.length() - 1] == '\n')
		buffer.erase(buffer.length() - 2);
	std::size_t pos = buffer.find(' ');
    total_arg = buffer.substr(pos + 1);
    std::string	username = find_username(total_arg);
	std::string realname = find_realname(total_arg);
    
    if (username.empty() || realname.empty())
		send(c_client.get_client_fd(), ERR_NEEDMOREPARAMS(c_client.getNickname(), "/user").c_str(), ERR_NEEDMOREPARAMS(c_client.getNickname(), "/user").size(), 0);
	else if (c_client.is_registred == true)
		send (c_client.get_client_fd(), ERR_ALREADYREGISTERED(c_client.getNickname()).c_str(), ERR_ALREADYREGISTERED(c_client.getNickname()).size(), 0);
	else
	{
        std::cout << username << "/"<< realname << std::endl;
		c_client.setUsername(username);
		c_client.setRealname(realname);
    }
}

std::string find_username(std::string total_arg)
{
    std::string username;
	username.clear();

	if (!total_arg.empty())
	{
        std::size_t pos = total_arg.find(' ');
		if (pos != std::string::npos)
			username.insert(0, total_arg, 0, total_arg.find(' '));
		else
			username.insert(0, total_arg, 0, std::string::npos);
	}
	return (username);
}

std::string	find_realname(std::string total_arg)
{
	std::string realname;
	realname.clear();

    size_t pos = total_arg.find(':');
    if (pos != std::string::npos)
    {
        realname = total_arg.substr(pos + 1);
        return realname;
    }
    else
        return realname;
}