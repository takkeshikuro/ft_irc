#include "../../../inc/Server.hpp"

void    Server::mode_t(Channel &chan, Client c_client, char sign)
{
	std::cout << "topic flag command\n";
	if (chan.get_topic_opr() == false && sign == '+')
	{
		std::string tosend = MODE_USERMSG(chan.get_name(), "+t");
		if (c_client.get_is_irssi() == false)
			tosend = chan.get_name() + "'s /topic now operator restricted.\n";
		send(c_client.get_client_fd(), tosend.c_str(), tosend.size(), 0);
		chan.set_topic_opr(true);        
	}
	if (chan.get_topic_opr() == true && sign == '-')
	{
		std::string tosend = MODE_USERMSG(chan.get_name(), "-t");
		if (c_client.get_is_irssi() == false)
			tosend = chan.get_name() + "'s /topic isn't operator restricted anymore.\n";
		send(c_client.get_client_fd(), tosend.c_str(), tosend.size(), 0);
		chan.set_topic_opr(false);
	}
	std::cout << chan.get_topic_opr() << "\n";
}