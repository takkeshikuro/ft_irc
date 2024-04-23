/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_commands.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keshikuro <keshikuro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 01:41:42 by keshikuro         #+#    #+#             */
/*   Updated: 2024/04/23 18:29:28 by keshikuro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

// 1 - help
// 2 - list_ch
// 3 - list_cl

void    Server::HELP(std::string buffer, Client c_client)
{
	(void)buffer;
	std::string helperblank = "|                                                             |\n"; 
	std::string helper = "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"; 
	std::string helper1_0 = "| -/list_ch                    #to see all active channels    |\n";
	std::string helper1_1 = "| -/list_cl                    #to see all active users       |\n";
	std::string helper1_2 = "| -/privmsg + <user_nickname>  #to dm an user (private)       |\n";
	std::string helper1_3 = "| -/invite + <user_nickname>   #to send invit to a channel    |\n";
	std::string helper2_0 = "|Commands to get into/leave channels :                        |\n";
	std::string helper2_1 = "| -/create + <channel_name>    #to create a new channels      |\n";
	std::string helper2_2 = "| -/join + <channel_name>      #to join a channel             |\n";
	std::string helper2_3 = "| -/leave                      #to leave a channel            |\n";
	std::string helper3_0 = "|User's datas :                                               |\n";
	std::string helper3_1 = "| -/nick + <new_name>          #to rename (nickname)          |\n";
	std::string helper3_2 = "| -/whois + <user_nickname>    #to get info about an user     |\n";
	std::string helper4_0 = "|Commands to manage channel :                                 |\n";
	std::string helper4_1 = "| -/topic                      #to display channel's topic    |\n";
	std::string helper4_2 = "| -/topic + <new topic>        #to update channel's topic     |\n";
	std::string helper5   = "| -/help_operator              #to see operator's commands    |\n";

	send(c_client.get_client_fd(), helper.c_str(), helper.size(), 0); // ~~~~~~~~~~~~~~~~~~~~~~~~~~
	send(c_client.get_client_fd(), helper1_0.c_str(), helper1_0.size(), 0); // list_ch
	send(c_client.get_client_fd(), helper1_1.c_str(), helper1_1.size(), 0); // list_cl
	send(c_client.get_client_fd(), helper1_2.c_str(), helper1_2.size(), 0); // prv_msg
	send(c_client.get_client_fd(), helper1_3.c_str(), helper1_3.size(), 0); // invite
	send(c_client.get_client_fd(), helperblank.c_str(), helperblank.size(), 0); // blank
	send(c_client.get_client_fd(), helper2_0.c_str(), helper2_0.size(), 0); // commands to get_into/leave
	send(c_client.get_client_fd(), helper2_1.c_str(), helper2_1.size(), 0); // create
	send(c_client.get_client_fd(), helper2_2.c_str(), helper2_2.size(), 0); // join 
	send(c_client.get_client_fd(), helper2_3.c_str(), helper2_3.size(), 0); // leave
	send(c_client.get_client_fd(), helperblank.c_str(), helperblank.size(), 0); // blank
	send(c_client.get_client_fd(), helper3_0.c_str(), helper3_0.size(), 0); //user's datas
	send(c_client.get_client_fd(), helper3_1.c_str(), helper3_1.size(), 0); // nick
	send(c_client.get_client_fd(), helper3_2.c_str(), helper3_2.size(), 0); // whois
	send(c_client.get_client_fd(), helperblank.c_str(), helperblank.size(), 0); // blank
	send(c_client.get_client_fd(), helper4_0.c_str(), helper4_0.size(), 0); // manage channel 
	send(c_client.get_client_fd(), helper4_1.c_str(), helper4_1.size(), 0); // topic
	send(c_client.get_client_fd(), helper4_2.c_str(), helper4_2.size(), 0); // topic 
	send(c_client.get_client_fd(), helper5.c_str(), helper5.size(), 0); // help_opertor
	send(c_client.get_client_fd(), helper.c_str(), helper.size(), 0); // ~~~~~~~~~~~~~~~~~
	return;
}

void    Server::HELP_OPERATOR(std::string buffer, Client c_client)
{
	(void)buffer;
	std::string helperblank = "|                                                             |\n"; 
	std::string helper = "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"; 
	std::string helper0 = "|Operator's options :                                         |\n";
	std::string helper1 = "| -/mode + <channel>           #to see all channel's operators|\n";
	std::string helper2_0 = "| -/mode + <channel> + <+o> + <nickname>                       |\n";
	std::string helper2_1 = "|        #to give operators privileges to an user              |\n";
	std::string helper2_2 = "| -/mode + <channel> + <-o> + <nickname>                       |\n";
	std::string helper2_3 = "|        #to remove operators privileges to an user            |\n";
	std::string helper3_0 = "| -/mode + <channel> + <-i>/<+i>                               |\n";
	std::string helper3_1 = "|        #to get or rm channel on invitation mode only         |\n";
	std::string helper4_0 = "| -/mode + <channel> + <+k>                                    |\n";
	std::string helper4_1 = "|        #to get/set a keypass/password to access the channel  |\n";
	std::string helper4_2 = "| -/mode + <channel> + <-k>                                    |\n";
	std::string helper4_3 = "|        #to remove a keypass/password of a channel            |\n";
	std::string helper5_0 = "| -/mode + <channel> + <+l>                                    |\n";
	std::string helper5_1 = "|        #to get/set a limit of users can access the channel   |\n";
	std::string helper5_2 = "| -/mode + <channel> + <-l>                                    |\n";
	std::string helper5_3 = "|        #to remove a limit of users                           |\n";
	std::string helper6_0 = "| -/mode + <channel> + <-t>/<+t>                               |\n";
	std::string helper6_1 = "|        #to get or rm restriction of /topic command for opt   |\n";

	send(c_client.get_client_fd(), helper.c_str(), helper.size(), 0); // ~~~~~~~~~~~~~~~~~
	send(c_client.get_client_fd(), helper0.c_str(), helper0.size(), 0); // opt commands
	send(c_client.get_client_fd(), helper1.c_str(), helper1.size(), 0); // mode channel
	send(c_client.get_client_fd(), helperblank.c_str(), helperblank.size(), 0); // blank
	send(c_client.get_client_fd(), helper2_0.c_str(), helper2_0.size(), 0); // mode -o / +o
	send(c_client.get_client_fd(), helper2_1.c_str(), helper2_1.size(), 0);	
	send(c_client.get_client_fd(), helper2_2.c_str(), helper2_2.size(), 0);
	send(c_client.get_client_fd(), helper2_3.c_str(), helper2_3.size(), 0);
	send(c_client.get_client_fd(), helperblank.c_str(), helperblank.size(), 0); // blank
	send(c_client.get_client_fd(), helper3_0.c_str(), helper3_0.size(), 0);	// -i/+i
	send(c_client.get_client_fd(), helper3_1.c_str(), helper3_1.size(), 0);
	send(c_client.get_client_fd(), helperblank.c_str(), helperblank.size(), 0); // blank	
	send(c_client.get_client_fd(), helper4_0.c_str(), helper4_0.size(), 0); // -k/+k
	send(c_client.get_client_fd(), helper4_1.c_str(), helper4_1.size(), 0);
	send(c_client.get_client_fd(), helper4_2.c_str(), helper4_2.size(), 0);
	send(c_client.get_client_fd(), helper4_3.c_str(), helper4_3.size(), 0);
	send(c_client.get_client_fd(), helperblank.c_str(), helperblank.size(), 0); // blank
	send(c_client.get_client_fd(), helper5_0.c_str(), helper5_0.size(), 0);	// -l/+l
	send(c_client.get_client_fd(), helper5_1.c_str(), helper5_1.size(), 0);
	send(c_client.get_client_fd(), helper5_2.c_str(), helper5_2.size(), 0);
	send(c_client.get_client_fd(), helper5_3.c_str(), helper5_3.size(), 0);
	send(c_client.get_client_fd(), helperblank.c_str(), helperblank.size(), 0); // blank
	send(c_client.get_client_fd(), helper6_0.c_str(), helper6_0.size(), 0); // -t/+t
	send(c_client.get_client_fd(), helper6_1.c_str(), helper6_1.size(), 0);
	send(c_client.get_client_fd(), helper.c_str(), helper.size(), 0); // ~~~~~~~~~~~~~~~~~

}

void    Server::LIST_CH(std::string buffer, Client c_client) 
{
	(void)buffer;
	std::string head = "=============================================\n List of all opened channels : \n";
	send(c_client.get_client_fd(), head.c_str(), head.size(), 0);
	
	for (size_t i = 0; i < channel_vec.size(); i++)
	{
		std::string ch_line;
		if (channel_vec[i].get_key_set() == true)
			ch_line = "-" + channel_vec[i].get_name() + " (keypass lock)\n";
		else
			ch_line = "-" + channel_vec[i].get_name() + "\n";
		send(c_client.get_client_fd(), ch_line.c_str(), ch_line.size(), 0);
		for (size_t j = 0; j < channel_vec[i].client_list.size(); j++)
		{
			std::string members = "   - " + channel_vec[i].client_list[j].getNickname() + "\n";
			send(c_client.get_client_fd(), members.c_str(), members.size(), 0);
		}
	}
	std::string bottom = "=============================================\n";
	send(c_client.get_client_fd(), bottom.c_str(), bottom.size(), 0);
}

void    Server::LIST_CL(std::string buffer, Client c_client) 
{
	(void)buffer;
	std::string head = "=============================================\n List of all clients in server : \n";
	send(c_client.get_client_fd(), head.c_str(), head.size(), 0);
	
	for (size_t i = 0; i < client_vec.size(); i++)
	{
		std::string cl_line = "-#" + client_vec[i].getNickname() + "\n";
		send(c_client.get_client_fd(), cl_line.c_str(), cl_line.size(), 0);
	}
	std::string bottom = "=============================================\n";
	send(c_client.get_client_fd(), bottom.c_str(), bottom.size(), 0);
}

// void    Server::NICK(std::string buffer, Client c_client)
// {
// 	std::stringstream	sbuf(buffer);
// 	std::string			new_nick;
// 	std::string			cmd;
//     int                 index;
// 	std::getline(sbuf, cmd, ' ');
// 	while (std::getline(sbuf, new_nick, '\n')) {
// 		if (!new_nick.empty())
// 			break ;
// 	}
// 	if (new_nick.empty()) {
// 		std::string err = red + "Error: empty new nickname.\n" + white;
// 		send(c_client.get_client_fd(), err.c_str(), err.size(), 0);
// 		return ;
// 	}
// 	if (invalid_char(new_nick))
// 	{
// 		std::string error = red + "Error: Invalid character in new nickname.\n" + white;
// 		send(c_client.get_client_fd(), error.c_str(), error.size(), 0);
// 		return ;	
// 	}
// 	if (nickname_already_used(client_vec, new_nick))
// 	{
// 		std::string error = red + "Error: nickname " + new_nick + " is already used by someone.\n";
// 		send(c_client.get_client_fd(), error.c_str(), error.size(), 0);
// 		return ;
// 	}
// 	size_t	i;
// 	for (i = 0; i < this->client_vec.size(); i++) {
// 		if (client_vec[i].getUsername() == c_client.getUsername())
// 			break ;
// 	}
//     for (size_t k = 0; k < channel_vec.size(); k++)         //change nickname in all channels vectors
//     {
//         index = index_channel(c_client, channel_vec[k]);
//         if (index != -1)
//             channel_vec[k].client_list[index].setNickname(new_nick);
//     }
//     for (size_t k = 0; k < channel_vec.size(); k++)         //change nickname in all channels operators vectors
//     {
//         index = index_operator(c_client, channel_vec[k]);
//         if (index != -1)
//             channel_vec[k].op_clients[index].setNickname(new_nick);
//     }
// 	client_vec[i].setNickname(new_nick);
// 	std::string	success = "\e[1;32mYour nickname has been changed to " + new_nick + ".\n\e[0m";
// 	send(c_client.get_client_fd(), success.c_str(), success.size(), 0);
// 	return ;
// }

// void	Server::SECRET_ROOT(std::string buffer, Client c_client)
// {
// 	(void)buffer;
// 	std::string root_password = "doumpied\n";
// 	std::string root = "please give root password to have root's permission : ";
// 	send(c_client.get_client_fd(), root.c_str(), root.size(),0);
// 	while (1) 
// 	{
// 		char buff[1024];
// 		int byte = read(c_client.get_client_fd(), buff, sizeof(buff));
// 		if (byte > 0) 
// 		{
// 			buff[byte] = '\0';
// 			std::string pw(buff);
// 			if (pw == root_password) 
// 			{	
// 				for (size_t i = 0; i < client_vec.size(); i++)  {
// 					if (client_vec[i].getNickname() == c_client.getNickname())
// 						client_vec[i].set_admin_perm();
// 					std::string good = yellow + "You are know admin." + white + "\n";
// 					send(c_client.get_client_fd(), good.c_str(), good.size(), 0);				
// 					return;
// 				}	
// 			}
// 			else {
// 				std::string bad = red + "bad password.\n" + white;
// 				send(c_client.get_client_fd(), bad.c_str(), bad.size(), 0);
// 				return ;
// 			}
// 		}
// 		else if (byte == 0) {
// 			std::cerr << "Connexion fermée par le client" << std::endl;
// 			break ;
// 		}
// 	}
// }

