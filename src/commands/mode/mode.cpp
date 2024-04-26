#include "../../../inc/Server.hpp"
std::vector<std::string> ft_split(const std::string& str, const std::string& delimiters);
/*
Paramètres : <nom du chan> <+|-|o|p|s|i|t|n|b|v|m>+ <autres paramètres>

flags a faire :
k	
l	done
o	done
t	
i	done	

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


// void    Server::mode_o()
// void    Server::mode_k()
// void    Server::mode_i()
// void    Server::mode_topic()

void    invisible_mode(Client &client){
    std::string ret;
	if (client.get_is_irssi() == false)
		return ;
    ret = MODE_USERMSG(client.getNickname(), "+i");
    send(client.get_client_fd(), ret.c_str(), ret.size(), 0);
}

int	verif_args_mode(std::vector<std::string> args)
{
	char sign;
	int	nb = 0;
	int	verif = args.size() - 3;
	// /mode #chan +ol nick 123


	for (size_t i = 0; i < args[2].size(); i++)
	{

		if (args[2][i] == '+' || args[2][i] == '-')
			sign = args[2][i];

		// if (args[2][i] != '+' && args[2][i] != '-')
		// 	std::cout << "flag : " << sign << args[2][i] << "\n" << "args : " << args[j] << "\n\n";
		if (args[2][i] == 'o')
			nb++;
		else if (args[2][i] == 'k')
		{
			if (sign == '+')
				nb++;
		}
		else if (args[2][i] == 'l')
		{
			if (sign == '+')
				nb++;
		}
	}
	if (nb != verif)
		return 0;
	return 1;
}

void    Server::mode(std::string buffer, Client c_client)
{
	std::cout << RED << "mode command called by " << YEL << c_client.getNickname() << RESET << " with " << buffer << RESET;
	
	std::string 				delimiters = " \r\n/";
	std::vector<std::string>	args;
	char						sign;
	size_t						j = 3;
	
	args = ft_split(buffer, delimiters);

	if (args[1][0] != '#')
	{
			invisible_mode(c_client);
			return ;
	}
	if (!verif_args_mode(args))
	{
		std::string to_send = "Error (mode): missing arguments\n";
		if (c_client.get_is_irssi() == true)
			to_send = ERR_NEEDMOREPARAMS(c_client.getNickname(), "mode");
		send(c_client.get_client_fd(), to_send.c_str(), to_send.size(), 0);
		return ;
	}
	int	chan_idx = index_channel_name(args[1], channel_vec);
	if (chan_idx == -1)
		return ;
	if (mode_verif(args, c_client))
		return ;
	if (args.size() <= j)
		j--;
	for (size_t i = 0; i < args[2].size(); i++)
	{
		if (args.size() <= j)
			j--;
		if (args[2][i] == '+' || args[2][i] == '-')
			sign = args[2][i];

		if (args[2][i] != '+' && args[2][i] != '-')
			std::cout << "flag : " << sign << args[2][i] << "\n" << "args : " << args[j] << "\n\n";
		if (args[2][i] == 'o')
		{
			mode_o(channel_vec[chan_idx], c_client, args[j], sign);
			j++;
		}
		else if (args[2][i] == 'k')
		{
			mode_k(channel_vec[chan_idx], c_client, args[j], sign);
			if (sign == '+')
				j++;
		}
		else if (args[2][i] == 'i')
		    mode_i(channel_vec[chan_idx], c_client, sign);
		else if (args[2][i] == 'l')
		{
			mode_l(channel_vec[chan_idx], c_client, args[j], sign);
			if (sign == '+')
				j++;
		}
		else if (args[2][i] == 't')
		    mode_t(channel_vec[chan_idx], c_client, sign);
	}
	std::cout << "mode fini\n";
	return;
}


