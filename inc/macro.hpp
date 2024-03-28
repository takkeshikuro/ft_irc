#ifndef	MACRO_HPP
#define MACRO_HPP

# define MAX_CLIENT_NB 10
# define SUCCESS 0
# define FAILURE -1
# define BREAK 2
# define CONTINUE 3



/*		MESSAGE		*/
//welcome
# define RPL_WELCOME(client, IPadd) (":localhost 001 " + client + " :Welcome to the Internet Relay Network, " + client +":"+ IPadd + "\r\n")
# define RPL_YOURHOST(client, serv_name) (":localhost 002 " + client + " :Your host is " + serv_name + " (localhost), running version 1.0.1\r\n")
# define RPL_CREATED(client) (":localhost 003 " + client + " :This server was created <include date_time>\r\n")
# define RPL_MYINFO(client, serv_name) (":localhost 004 " + client + " :" + serv_name + " 1.0.1 io kost k\r\n")
# define RPL_ISUPPORT(client) (":localhost 005 " + client+ " :CHANNELLEN=32 NICKLEN=9 TOPICLEN=307 : are supported by this server\r\nEnjoy!!\r\n")

//nick
# define ERR_NONICKNAMEGIVEN(client) (":localhost 431 " + client + " :There is no nickname.\r\n")
# define ERR_ERRONEUSNICKNAME(client, nickname) (":localhost 432 " + client + " " + nickname + " :Erroneus nickname\r\n")
# define ERR_NICKNAMEINUSE(client, nickname) (":localhost 433 " + client + " " + nickname + " :Nickname is already in use.\r\n")
# define RPL_NICK(oclient, uclient, client) (":" + oclient + "!" + uclient + "@localhost NICK " +  client + "\r\n")



//pong
# define RPL_PONG(client, token) ("PONG reply to " + client + " " + token + "\r\n")

///utils
# define ERR_FULL_SERV "[Server] You cannot join, the server is already full"
# define ERR_UNKNOWNCOMMAND(client, command) (":localhost 421 " + client + " " + command + " :Unknown command\r\n")


#endif