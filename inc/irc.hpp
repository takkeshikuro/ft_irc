#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>

const int MAX_CLIENTS = 10;
const int BUFFER_SIZE = 1024;