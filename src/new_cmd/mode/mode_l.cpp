#include "../../../inc/Server.hpp"

void    Server::mode_l(Channel &chan, Client c_client, std::string lim_str, char sign)
{
	int lim = std::atoi(lim_str.c_str());

	for (size_t i = 0; i < lim_str.size(); i++)
	{
		if ((!isdigit(lim_str[i]) && lim_str[i] != '\n' && lim_str[i] != ' ') || lim <= 0)
		{
			std::string invalid = "\e[1;31mError: Invalid limit value (" + lim_str + ")\n" + RESET;
			std::cout << YEL << c_client.getNickname() << RED 
			<< " Error: Invalid limit value (" + lim_str + ")\n" + RESET;
			
			send(c_client.get_client_fd(), invalid.c_str(), invalid.size(), 0);
			return ;
		}
	}

	if (sign == '-')
	{
		chan.set_limit(1000);
		std::string tosend = YEL + c_client.getNickname() 
			+ GRE + " has unset users limit in " + BLU + chan.get_name() + RESET + "\n";
		std::cout << tosend;
	}
	else if (sign == '+')
	{
		if (chan.get_size() > lim)
		{
			std::string invalid = "\e[1;31mError: Invalid limit value (" + lim_str + ")\n" + RESET;
			std::cout << YEL << c_client.getNickname() << RED 
			<< " Error: Invalid limit value (" + lim_str + ")\n" + RESET;
			send(c_client.get_client_fd(), invalid.c_str(), invalid.size(), 0);
			return ;
		}
		else
			chan.set_user_max(lim);
	}
	std::cout << "limit is : " << chan.get_limit() << "\n";
	return ;
}