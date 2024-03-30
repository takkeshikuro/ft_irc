#include "../inc/Server.hpp"



/**
 * @brief The NICK command is used to give the client a nickname or 
 * 		change the previous one.
 * 
 * 	Syntax: NICK <nickname>
 * 
 * 	Nicknames are non-empty strings with the following restrictions:
 * 	
 * 	They MUST NOT contain any of the following characters: 
 * 	- space (' '), 
 * 	- comma (','), 
 * 	- asterisk ('*'), 
 * 	- question mark ('?'), 
 * 	- exclamation mark ('!'), 
 * 	- at sign ('@'),
 * 	- dot ('.').
 * 	
 * 	They MUST NOT start with any of the following characters: 
 * 	dollar ('$'), colon (':'), diese (#).
 * 	
 * 	Numeric Replies:
 * 
 * 	ERR_NONICKNAMEGIVEN (431)
 * 	ERR_ERRONEUSNICKNAME (432)
 * 	ERR_NICKNAMEINUSE (433)
 * 	ERR_NICKCOLLISION (436)
 * 
 * 	Example:
 * 	[CLIENT] /Nick tak
 * 
 */

void	Server::nick(std::string buffer, Client c_client)
{
	std::string new_nick;
	std::string old_nick = c_client.getNickname();
    if (buffer[buffer.length() - 1] == '\n')
		buffer.erase(buffer.length() - 2);
	std::size_t pos = buffer.find(' ');
	if (pos != std::string::npos)
		new_nick = buffer.substr(pos + 1);

	if (new_nick.empty()) {
		size_t size = ERR_NONICKNAMEGIVEN(old_nick).size();
		send(c_client.get_client_fd(), ERR_NONICKNAMEGIVEN(old_nick).c_str(), size, 0);
	}
	else if (invalid_char(new_nick)) {
		size_t size = ERR_ERRONEUSNICKNAME(old_nick, new_nick).size();
		send(c_client.get_client_fd(), ERR_ERRONEUSNICKNAME(old_nick, new_nick).c_str(), size, 0);
	}
	else if (nickname_already_used(this->client_vec, new_nick) == true) {
		size_t size = ERR_NICKNAMEINUSE(old_nick, new_nick).size();
		send(c_client.get_client_fd(), ERR_NICKNAMEINUSE(old_nick, new_nick).c_str(), size, 0);
	}
	else
	{	
		int index;
		c_client.setNickname(new_nick);
		for (size_t i = 0; i < client_vec.size(); i++) {
			if (client_vec[i].getNickname() == c_client.getNickname())
				client_vec[i].setNickname(new_nick);
		}
		for (size_t i = 0; i < channel_vec.size(); i++) {
			index = index_channel(c_client, channel_vec[i]);
			if (index != -1)
				channel_vec[i].client_list[index].setNickname(new_nick);
		}
		for (size_t i = 0; i < channel_vec.size(); i++) {
			index = index_operator(c_client, channel_vec[i]);
			if (index != -1)
				channel_vec[i].op_clients[index].setNickname(new_nick);
		}
		size_t size = RPL_NICK(old_nick, c_client.getUsername(), c_client.getNickname()).size();
		send(c_client.get_client_fd(), RPL_NICK(old_nick, c_client.getUsername(), c_client.getNickname()).c_str(), size, 0);
	}
	return ;
}

bool	isForbidden(char c)
{
	if (c == ' ' || c == ',' || c == '*' || c == '?' || c == '!' || c == '@' || c == '.')
		return (true);
	else
		return (false);
}

bool	invalid_char(std::string nickname)
{
	if (nickname[0] == '$' || nickname[0] == ':' || nickname[0] == '#')
		return true;
	for (size_t i = 0; i < nickname.size(); i++) {
			if (isForbidden(nickname[i]) == true)
				return true;
	}
	return false;			
}

bool	nickname_already_used(std::vector<Client> &client_vec, std::string name)
{
	for (size_t i = 0; i < client_vec.size(); i++) {
		if (client_vec[i].getNickname() == name)
			return true;
	}
	return false;
}