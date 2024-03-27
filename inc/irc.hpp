#pragma once

#include <iostream>
#include <vector> //-> for vector
#include <sys/socket.h> //-> for socket()
#include <netinet/in.h> //-> for sockaddr_in
#include <sys/types.h> //-> for socket()
#include <fcntl.h> //-> for fcntl()
#include <unistd.h> //-> for close()
#include <arpa/inet.h> //-> for inet_ntoa()
#include <poll.h> //-> for poll()
#include <csignal> //-> for signal()
#include <cstring>
#include <string>
#include <sstream>
#include <list>
#include <algorithm>

#include "macro.hpp"


#define RED "\e[1;31m" //-> for red color
#define WHI "\e[0;37m" //-> for white color
#define GRE "\e[1;32m" //-> for green color
#define YEL "\e[1;33m" //-> for yellow color
#define PUR	"\e[1;35m"
#define BLU	"\e[1;34m"

//couleurs pas en gras
#define BLACK	"\e[0;30m"
#define GREEN	"\e[0;32m"
#define YELLOW	"\e[0;33m"
#define BLUE	"\e[0;34m"
#define PURPLE	"\e[0;35m"
#define CYAN	"\e[0;36m"
#define WHITE	"\e[0;37m"
#define RESET  "\e[0m"

const int MAX_CLIENTS = 10;
const int BUFFER_SIZE = 1024;

