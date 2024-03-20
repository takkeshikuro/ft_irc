#pragma once

#include "Server.hpp"
class Client;
class Channel;

int index_operator(Client c_client, Channel &chan);
int index_channel(Client c_client, Channel &chan);