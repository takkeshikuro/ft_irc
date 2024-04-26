/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 01:19:18 by keshikuro         #+#    #+#             */
/*   Updated: 2024/03/02 08:12:32 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "irc.hpp"
#include "exception.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "utils.hpp"
class Client;
//class Channel;

class Server {
	public :
		Server();
		Server(const char *s_port, std::string passwd, struct tm *timeinfo);
		~Server();

		void	configuration();
		void	default_channel_creation();
		void	launch_server();
		int		manage_new_client();
		void	manage_new_data(int fd);
		int		handlePolloutEvent(const int current_fd);
		int		handlePollerEvent(pollfd &it);
		
		static void	SignalHandler(int sig);
		int			tooManyClients(int client_socket);
		void 		close_fds();
 		void 		clear_clients(int fd);
		int			check_irssi_entrance(int fd);
		void		setDatetime(struct tm *timeinfo);
		int			index_client(std::string nick);

		//irssi
		int		is_irssi_command(char *buffer, Client c_client);
		void	check_bot(std::string buffer, std::string &cmd_string);
		int    login(std::string buffer, Client &c_client);

		void	ping(std::string buffer, Client c_client);
		void	nick(std::string buffer, Client c_client);
		void	list(std::string buffer, Client c_client);
		void	join(std::string buffer, Client c_client);
		void	access_channel(Client c_client, std::string channel_name, int i);
		void	msg(std::string buffer, Client c_client);
		void	msg_channel(std::string args[3], Client c_client);
		void	topic(std::string buffer, Client c_client);
		void	user(std::string buffer, Client c_client);
		void	part(std::string buffer, Client c_client);
		void	kick(std::string buffer, Client c_client);
		void	invite(std::string buffer, Client c_client);
		void	quit(std::string buffer, Client c_client);
		int		pass(std::string buffer, Client c_client);
		void	mode(std::string buffer, Client c_client);
		int		mode_verif(std::vector<std::string> args, Client c_client);
		void	mode_l(Channel &chan, Client c_client, std::string lim_str, char sign);
		void	mode_o(Channel &chan, Client c_client, std::string target, char sign);
		void	mode_i(Channel &chan, Client c_client, char sign);
		void	mode_t(Channel &chan, Client c_client, char sign);
		void	mode_k(Channel &chan, Client c_client, std::string keypass, char sign);

		void	command_unknow(Client c_client, std::string cmd);
		void	call_bot(std::string buffer, Client c_client);

		//netcat
		int     is_command(char *buffer, Client c_client);
		void	LIST_CH(std::string buffer, Client c_client);
		void	LIST_CL(std::string buffer, Client c_client);
		void    HELP(std::string buffer, Client c_client);
		void    HELP_OPERATOR(std::string buffer, Client c_client);

	private :
		static bool			signal;
		struct sockaddr_in	serverAddr;
		struct pollfd		NewPoll;
		int 				serverSocket;
		int					port;
		std::string			datetime;
		std::string			password;
		std::string			irssi_base;

		std::vector<struct pollfd> fds;	
		std::vector<Client> client_vec;
		std::vector<Channel> channel_vec;
			
		char buffer[BUFFER_SIZE];
		std::string green;
		std::string white;
		std::string red;
		std::string yellow;
};

#endif