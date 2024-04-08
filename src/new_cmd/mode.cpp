#include "../../inc/Server.hpp"

/*
Paramètres : <nom du chan> <+|-|o|p|s|i|t|n|b|v|m>+ <autres paramètres>

Réponses possibles :
ERR_CHANOPRIVSNEEDED (482), done
ERR_NOSUCHCHANNEL (403),    done
ERR_NOTONCHANNEL (442),     done
ERR_NOSUCHNICK (401),
ERR_KEYSET (467), 
ERR_UNKNOWNMODE (472)       done?
ERR_NEEDMOREPARAMS (461), 
RPL_CHANNELMODEIS (324), 
*/

void    Server::mode(std::string buffer, Client c_client)
{
    //debuf buffer = MODE #chan +o lulu
    //                   0    1    2   3
    std::cout << RED << "mode command called by " << YEL << c_client.getNickname() << "\n" << RESET;
    std::stringstream   sbuf(buffer);
    std::string         args[4];
    std::getline(sbuf, args[0], ' ');

    //channel name
    while (std::getline(sbuf, args[1], ' ')) {
		if (!args[1].empty())
			break;
	}
    //mode flags
	while (std::getline(sbuf, args[2], ' ')) {
		if (!args[2].empty())
			break;
	}
    //target user
	while (std::getline(sbuf, args[3], '\r')) {
		if (!args[3].empty())
			break;
	}
    if (args[1] == "CAP")
        return ;
    mode_parse(args, c_client);
}

void    Server::mode_parse(std::string args[4], Client c_client)
{
    size_t  i;
    size_t  j;
    int     size;

    //check if channel exists
    for (i = 0; i < channel_vec.size(); i++)
    {
        if (args[1] == channel_vec[i].get_name())
            break ;
    }
    if (i == channel_vec.size()){
        size = ERR_NOSUCHCHANNEL(args[1]).size();
		send(c_client.get_client_fd(), ERR_NOSUCHCHANNEL(args[1]).c_str(), size, 0);
        return ;
    }

    //check if is on channel
    for (j = 0; j < channel_vec[i].client_list.size(); j++){
        if (c_client.getNickname() == channel_vec[i].client_list[j].getNickname())
            break ;
    }
    if (j == channel_vec[i].client_list.size()){
        size = ERR_NOTONCHANNEL(c_client.getNickname(), args[1]).size();
		send(c_client.get_client_fd(), ERR_NOTONCHANNEL(c_client.getNickname(), args[1]).c_str(), size, 0);
        return ;
    }

    //check if is op on channel
    for (j = 0; j < channel_vec[i].op_clients.size(); j++){
        if (c_client.getNickname() == channel_vec[i].op_clients[j].getNickname())
            break ;
    }
    if (j == channel_vec[i].client_list.size()){
        size = ERR_CHANOPRIVSNEEDED(c_client.getNickname(), args[1]).size();
		send(c_client.get_client_fd(), ERR_CHANOPRIVSNEEDED(c_client.getNickname(), args[1]).c_str(), size, 0);
        return ;
    }

    for (size_t z = 0; args[2][z]; z++){
        if (args[2][z] != 'i' && args[2][z] != 't' && args[2][z] != 'k' && args[2][z] != 'o' 
                && args[2][z] != 'l' && args[2][z] != '-' && args[2][z] != '+')
        {
            std::string character;
            character.push_back(args[2][z]);
            size = ERR_UNKNOWNMODE(character).size();
		    send(c_client.get_client_fd(),ERR_UNKNOWNMODE(character).c_str(), size, 0);
            return ;
        }
    } 
}