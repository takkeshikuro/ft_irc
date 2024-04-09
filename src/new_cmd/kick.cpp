#include "../../inc/Server.hpp"

std::vector<std::string> ft_split(const std::string& str, const std::string& delimiters);
std::string get_data(std::string buffer, int hook);

/**
 * @brief The KICK command can be used to request the forced removal of a user 
 *  from a channel. It causes the <user> to be removed from the <channel> by force. 
 *  If no comment is given, the server SHOULD use a default message instead.
 * 
 *  Parameters: <channel> <user> *( "," <user> ) [<comment>]
 * 
 * Numeric Replies:
 * 
 * ERR_NEEDMOREPARAMS (461)
 * ERR_NOSUCHCHANNEL (403)
 * ERR_CHANOPRIVSNEEDED (482)
 * ERR_USERNOTINCHANNEL (441)
 * ERR_NOTONCHANNEL (442)
 * 
 * Example:
 * Client's request : KICK #Finnish John :Speaking English
 * 
 * Server's Response: " Command to kick John from #Finnish using "Speaking English" 
 * 						as the reason (comment)."
 *
 * @param server
 * @param cmd_infos Structure w/ prefix, command name and message
 */


void    Server::kick(std::string buffer, Client c_client)
{
	std::string	client_nickname	= c_client.getNickname();
	std::string channel_name = get_data(buffer, 0);
	std::string kicked_name = get_data(buffer, 1);
	std::string comment = get_data(buffer, 2);
	comment = (comment.empty()) ? "Kicked by the channel's operator" : comment;

	std::cout<<"user "<<client_nickname<<" try to kick "<<kicked_name<< " because :{"<< comment<< "}\n";

	if (channel_name.empty() || kicked_name.empty() || channel_name[0] != '#') {
		size_t size = ERR_NEEDMOREPARAMS(client_nickname, "/kick").size();
		send(c_client.get_client_fd(), ERR_NEEDMOREPARAMS(client_nickname, "/kick").c_str(), size, 0);
		return ;
	}
	int i = index_channel_name(channel_name, this->channel_vec);
	if (i == -1) {
		size_t size = ERR_NOSUCHCHANNEL(channel_name).size();
		send(c_client.get_client_fd(), ERR_NOSUCHCHANNEL(channel_name).c_str(), size, 0);
		return ;
	}
	if (index_channel_nick(client_nickname, channel_vec[i]) == -1) {
		size_t size = ERR_NOTONCHANNEL(client_nickname, channel_name).size();
		send(c_client.get_client_fd(), ERR_NOTONCHANNEL(client_nickname, channel_name).c_str(), size, 0);
		return ;	
	}
	int j = index_channel_nick(kicked_name, channel_vec[i]);
	if (j == -1) {
		size_t size = ERR_USERNOTINCHANNEL(client_nickname, kicked_name, channel_name).size();
		send(c_client.get_client_fd(), ERR_USERNOTINCHANNEL(client_nickname, kicked_name, channel_name).c_str(), size, 0);
		return ;	
	}
	if (index_operator_nick(client_nickname, channel_vec[i]) == -1) {
		size_t size = ERR_CHANOPRIVSNEEDED(client_nickname, channel_name).size();
		send(c_client.get_client_fd(), ERR_CHANOPRIVSNEEDED(client_nickname, channel_name).c_str(), size, 0);
		return ;	
	}// voir si il faut checker si kicked_name n'est pas operator
	else {
		int j = index_client(kicked_name);
	//	channel_vec[i].rm_user(this->client_vec[j]);
		channel_vec[i].send_string_all(RPL_KICK(client_nickname, c_client.getUsername(), channel_name, kicked_name, comment));
		std::string to_move = "/PART " + channel_name + "\r\n";
		//std::string info = ""
		part(to_move, client_vec[j]);
		
	}
}

std::string get_data(std::string buffer, int hook)
{
	std::string channel_name;
	std::string kicked_name;
	std::string comment;
	std::vector<std::string> vec_arg;
	std::vector<std::string> vec_arg_bis;

	comment.clear();
	if (buffer[buffer.length() - 1] == '\n')
		buffer.erase(buffer.length() - 2);
	vec_arg = ft_split(buffer, " ");
	channel_name = vec_arg[1];
	vec_arg_bis = ft_split(channel_name, ",");
	if (vec_arg_bis.size() == 2) {
		channel_name = vec_arg_bis[0];
		kicked_name = vec_arg_bis[1];
	}
	else
		kicked_name = vec_arg[2];
	size_t pos_comment = buffer.find(':');
	if (pos_comment != std::string::npos)
		comment = buffer.substr(pos_comment + 1);
	if (hook == 0)
		return channel_name;
	else if (hook == 1)
		return kicked_name;
	else
		return comment;
	
}
