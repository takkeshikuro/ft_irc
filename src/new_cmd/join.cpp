#include "../../inc/Server.hpp"

bool		is_alpha_in(std::string str);
std::string get_channel_name(std::string arg);

void		send_infos(Channel &channel, std::string channel_name, Client &client);

/**
 * @brief The JOIN command indicates that the client wants to join the given channel(s), 
 * 	each channel using the given key for it. The server receiving the command checks 
 * 	whether or not the client can join the given channel, and processes the request. 
 * 
 * 	While a client is joined to a channel, they receive all relevant information about 
 * 	that channel including the JOIN, PART, KICK, and MODE messages affecting the channel. 
 * 	They receive all PRIVMSG and NOTICE messages sent to the channel, and they also 
 * 	receive QUIT messages from other clients joined to the same channel (to let them 
 * 	know those users have left the channel and the network). 
 * 
 *  Numeric Replies:
 *  
 *  ERR_NEEDMOREPARAMS (461)
 *  ERR_NOSUCHCHANNEL (403)
 *  ERR_TOOMANYCHANNELS (405)
 *  ERR_BADCHANNELKEY (475)
 *  ERR_BANNEDFROMCHAN (474)
 *  ERR_CHANNELISFULL (471)
 *  ERR_INVITEONLYCHAN (473)
 *  ERR_BADCHANMASK (476)
 *  RPL_TOPIC (332)
 *  RPL_TOPICWHOTIME (333)
 *  RPL_NAMREPLY (353)
 *  RPL_ENDOFNAMES (366)
 *  
 *  Examples:
 * 	[CLIENT]  JOIN #foobar
 *  [SERVER] ; join channel #foobar.
 * 
 * 	[CLIENT]  JOIN #foo,#bar fubar,foobar
 * 	[SERVER]; join channel #foo using key "fubar" and channel #bar using key "foobar".
 */

void    Server::join(std::string buffer, Client c_client)
{
	std::string client_nickname = c_client.getNickname();
	std::string	channel_name;
	std::string total_arg;

	if (buffer[buffer.length() - 1] == '\n')
		buffer.erase(buffer.length() - 2);
	std::size_t pos = buffer.find(' ');
	if (pos != std::string::npos)
		total_arg = buffer.substr(pos + 1);

	if (is_alpha_in(total_arg) == false) {
		size_t size = ERR_NEEDMOREPARAMS(client_nickname, "/join").size();
		send(c_client.get_client_fd(), ERR_NEEDMOREPARAMS(client_nickname, "/join").c_str(), size, 0);
	}
	while (is_alpha_in(total_arg) == true)
	{
		channel_name.clear();
		channel_name = get_channel_name(total_arg);
		if (channel_name[0] != '#')
			channel_name = "#" + channel_name;
		// erase de la string le channel = "#foo,#bar" devient "#,#bar"
		total_arg.erase(total_arg.find(channel_name), channel_name.length()); 
		
		// Récupérer le bon channel grâce au channel name
		int check = 0;
		for (size_t i = 0; i < channel_vec.size(); i++) 
		{
			if (channel_vec[i].get_name() == channel_name) 
			{
				check = 1;
				// if (channel_vec[i].get_key_set()) // Si channel en mode +k
				// {
				// 	std::string key = retrieveKey(cmd_infos.message);
				// 	total_arg.erase(cmd_infos.message.find(key), key.length()); // on erase la key de la string
				// 	if (key != channel_vec[i].getChannelPassword())
				// 	{
				// 		size_t size = ERR_BADCHANNELKEY(client_nickname, channel_name).size();
				// 		send(c_client.get_client_fd(), ERR_BADCHANNELKEY(client_nickname, channel_name).c_str(), size, 0);
				// 		continue; // on passe la suite, au prochain channel à ajouter síl y en a un
				// 	}
				// }
				// vérifier si le channel est full
				if (channel_vec[i].client_list.size() + 1 > channel_vec[i].get_user_max())
				{
					size_t size = ERR_CHANNELISFULL(client_nickname, channel_name).size();
					send(c_client.get_client_fd(), ERR_CHANNELISFULL(client_nickname, channel_name).c_str(), size, 0);
					continue ;
				}
				else 
				{
					int check_1 = 0;
					for (size_t j = 0; j < channel_vec[i].client_list.size(); j++) {
						if (channel_vec[i].client_list[j].getNickname() == client_nickname)
							check_1 = 1;	
						//std::cout << YELLOW << client.getNickname() << "already here\n" << RESET;
					}	
					if (!check_1)
					{
						size_t j;
						for (j = 0; j < client_vec.size(); j++) {
							if (client_vec[j].getNickname() == client_nickname)
								break ;  
						}
						channel_vec[i].add_user(client_vec[j]);
						client_vec[j].in_channel += 1;
					}
//					if (channel_vec[i].op_clients.empty())
//						channel_vec[i].addFirstOperator(client.getNickname());
					send_infos(channel_vec[i], channel_name, c_client);
				}
			}
		}
		// si on ne le trouve pas, créer le channel
		if (!check)
		{
			Channel new_channel(channel_name, c_client.get_client_fd());
			new_channel.add_user(c_client);
			new_channel.add_operator(c_client);
			channel_vec.push_back(new_channel);
			std::cout << "-creation of new chan named [" <<channel_name<< "]\n";
		}	
	}
}

std::string client_list_to_str(std::vector<Client> c_list)
{
	std::string list_str;
	list_str = "users list :\n-";
	for (size_t i = 0; i < c_list.size(); i++) {
		list_str += c_list[i].getNickname();
		list_str += "\n-";
	}
	return list_str;
}

void		send_infos(Channel &channel, std::string channel_name, Client &client)
{
	std::string	nick		= client.getNickname();
	std::string username	= client.getUsername();
 	
	for (size_t i = 0; i < channel.client_list.size(); i++) 
	{
		size_t size_0 = RPL_JOIN(user_id(nick, username), channel_name).size();
		send(channel.client_list[i].get_client_fd(), RPL_JOIN(user_id(nick, username), channel_name).c_str(), size_0, 0);
		
		//size_t size_1 = RPL_TOPIC(nick, channel_name, channel.get_description()).size();
		//send(channel.client_list[i].get_client_fd(), RPL_TOPIC(nick, channel_name, channel.get_description()).c_str(), size_1, 0);
		
		std::string	list_of_members = client_list_to_str(channel.client_list);
		
		size_t size_2 = RPL_NAMREPLY(username, channel_name, list_of_members).size();
		send(channel.client_list[i].get_client_fd(), RPL_NAMREPLY(username, channel_name, list_of_members).c_str(), size_2, 0);
		
		size_t size_3 = RPL_ENDOFNAMES(username, channel_name).size();
		send(channel.client_list[i].get_client_fd(), RPL_ENDOFNAMES(username, channel_name).c_str(), size_3, 0);
	}
}

bool		is_alpha_in(std::string str)
{
	if (str[0] == ' ')
		str.erase(0, 1);
	if (str.find(" ") != str.npos)
		str = str.substr(0, str.find(" "));
	for (size_t i = 0; i < str.size(); i++) {
		if (isalpha(str[i]))
			return true;
	}
	return false;
}

std::string get_channel_name(std::string arg)
{
	std::string channel;
	for (size_t i = 0; i < arg.size(); i++) {
		while (!isalpha(arg[i]))
			i++;
		while (isalpha(arg[i]))
			channel += arg[i++];
	}
	return channel;
}

/*
std::string	retrieveKey(std::string msg_to_parse)
{
	std::string	key;
	key.clear();
	
	msg_to_parse = msg_to_parse.substr(msg_to_parse.find_last_of(" "), msg_to_parse.npos);
	if (msg_to_parse[0] == ' ')
		msg_to_parse.erase(0, 1); // Expected output : |fubar_75,foobar|
		
	int	begin_pos = (msg_to_parse.find(",") == 0) ? msg_to_parse.find(",") + 1 : 0; // Expected: begin à |fubar_75,foobar| ou |foobar| si 2eme key
	
	while ( msg_to_parse[begin_pos] != ',' && msg_to_parse[begin_pos])
	{
		if (msg_to_parse[begin_pos] == '_' || msg_to_parse[begin_pos] == '-'|| isalpha(msg_to_parse[begin_pos]) || isdigit(msg_to_parse[begin_pos]))
			key += msg_to_parse[begin_pos];
		begin_pos++;
	}
	return (key);
}
*/
