#pragma once

#include "Server.hpp"
class Client;
class Channel;

int index_operator(Client c_client, Channel &chan);
int index_channel(Client c_client, Channel &chan);
int index_operator_fd(int fd, Channel &chan);
int index_channel_fd(int fd, Channel &chan);
int index_operator_nick(std::string nick, Channel &chan);
int index_channel_nick(std::string nick, Channel &chan);
