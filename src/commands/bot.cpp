#include "../../inc/Server.hpp"

void	Server::call_bot(std::string buffer, Client c_client)
{
	(void)buffer;
	std::string	time_set;
	std::string to_send;
	char buf[80];
	time_t		rawtime;
	struct tm * timeinfo;
	time (&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buf, sizeof(buf), "%d-%m-%Y %H:%M:%S", timeinfo);
  	std::string str(buf);
	time_set = str;

	to_send = yellow + "[check time] : " + time_set + "\r\n" + white;
	send(c_client.get_client_fd(), to_send.c_str(), to_send.size(), 0);
}