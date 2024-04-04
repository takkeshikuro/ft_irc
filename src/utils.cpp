#include "../inc/Server.hpp"

int index_channel(Client c_client, Channel &chan)       //if the client is in the channel, returns the index of the client in the clients vector. else returns -1
{
	for (int i = 0; i < chan.get_size(); i++)
	{
		if (chan.client_list[i].getUsername() == c_client.getUsername())
			return i;
	}
	return -1;
}

int index_operator(Client c_client, Channel &chan)     //if the client is an operator in the channel, returns the index of the client in the operator vector. else returns -1
{
	for (size_t i = 0; i < chan.op_clients.size(); i++)
	{
		if (chan.op_clients[i].getUsername() == c_client.getUsername())
			return i;
	}
	return -1;
}

int index_channel_fd(int fd, Channel &chan)
{
	for (int i = 0; i < chan.get_size(); i++)
	{
		if (chan.client_list[i].get_client_fd() == fd)
			return i;
	}
	return -1;
}

int index_operator_fd(int fd, Channel &chan)
{
	for (size_t i = 0; i < chan.op_clients.size(); i++)
	{
		if (chan.op_clients[i].get_client_fd() == fd)
			return i;
	}
	return -1;
}

int index_channel_nick(std::string nick, Channel &chan)
{
	for (int i = 0; i < chan.get_size(); i++)
	{
		if (chan.client_list[i].getNickname() == nick)
			return i;
	}
	return -1;
}

int index_operator_nick(std::string nick, Channel &chan)
{
	for (size_t i = 0; i < chan.op_clients.size(); i++)
	{
		if (chan.op_clients[i].getNickname() == nick)
			return i;
	}
	return -1;
}

int index_channel_name(std::string name, std::vector<Channel> vector)
{
	for (size_t i = 0; i < vector.size(); i++)
	{
		if (vector[i].get_name() == name)
			return i;
	}
	return -1;
}
