#include "../../../inc/Server.hpp"

int Server::mode_verif(std::vector<std::string> args, Client c_client)
{
	size_t  i;
	size_t  j;
	int     size;
	// int     flags = 0;
	// int     args_count = 0;
	//check if channel exists
	for (i = 0; i < channel_vec.size(); i++)
	{
		if (args[1] == channel_vec[i].get_name())
			break ;
	}
	if (i == channel_vec.size()){
		size = ERR_NOSUCHCHANNEL(args[1]).size();
		send(c_client.get_client_fd(), ERR_NOSUCHCHANNEL(args[1]).c_str(), size, 0);
		return 1;
	}

	//check if is on channel
	for (j = 0; j < channel_vec[i].client_list.size(); j++){
		if (c_client.getNickname() == channel_vec[i].client_list[j].getNickname())
			break ;
	}
	if (j == channel_vec[i].client_list.size()){
		size = ERR_NOTONCHANNEL(c_client.getNickname(), args[1]).size();
		send(c_client.get_client_fd(), ERR_NOTONCHANNEL(c_client.getNickname(), args[1]).c_str(), size, 0);
		return 1;
	}

	//check if is op on channel
	for (j = 0; j < channel_vec[i].op_clients.size(); j++){
		if (c_client.getNickname() == channel_vec[i].op_clients[j].getNickname())
			break ;
	}
	if (j == channel_vec[i].op_clients.size()){
		size = ERR_CHANOPRIVSNEEDED(c_client.getNickname(), args[1]).size();
		send(c_client.get_client_fd(), ERR_CHANOPRIVSNEEDED(c_client.getNickname(), args[1]).c_str(), size, 0);
		return 1;
	}
	//check valid flags (i t k o l)
	int checkin = 0;
	size_t z;
	for (z = 0; args[2][z]; z++){

		if ((args[2][z] == '+' || args[2][z] == '-') && (args[2][z + 1] == '+' || args[2][z + 1] == '-')){
			checkin = 1;
			break ;
		}
		if (args[2][z] != 'i' && args[2][z] != 't' && args[2][z] != 'k' && args[2][z] != 'o' 
				&& args[2][z] != 'l' && args[2][z] != '-' && args[2][z] != '+'){
			checkin = 1;
			break ;
		}
	}
	if (checkin){
		std::string character;
		character.push_back(args[2][z]);
		size = ERR_UNKNOWNMODE(character).size();
		send(c_client.get_client_fd(),ERR_UNKNOWNMODE(character).c_str(), size, 0);
		return 1; 
	}
	return 0;
}


std::vector<std::string> ft_split(const std::string& str, const std::string& delimiters) {
	std::vector<std::string> parts;
	std::string token;
	std::istringstream iss(str);
	
	while (getline(iss, token)) {
		size_t pos = 0;
		while ((pos = token.find_first_of(delimiters, pos)) != std::string::npos) {
			token.replace(pos, 1, " ");
			pos++;
		}
		std::istringstream tokenStream(token);
		std::string part;
		while (tokenStream >> part) {
			parts.push_back(part);
		}
	}
	// for (size_t i = 0; i < parts.size(); i++){
	//     std::cout << parts[i] << std::endl;
	// }
	return parts;
}