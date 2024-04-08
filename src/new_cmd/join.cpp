#include "../../inc/Server.hpp"

bool		is_alpha_in(std::string str);
std::string get_channel_name(const std::string& arg);

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
	if (c_client.get_is_irssi() == false)
	{
		std::stringstream sbuf(buffer);
		std::string arg[2];
		std::getline(sbuf, arg[0], ' ');
		std::getline(sbuf, arg[1], '\n');
		buffer = "JOIN #" + arg[1] + "\r\n";
	}	
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
		
		// erase de la string le channel = "#foo,#bar" devient "#,#bar"
		total_arg.erase(total_arg.find(channel_name), channel_name.length()); 
		
		if (channel_name[0] != '#')
			channel_name = "#" + channel_name;
		int check = 0;
		for (size_t i = 0; i < channel_vec.size(); i++) 
		{
			//check si channel existe		
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
						client_vec[j].set_current_channel(channel_vec[i].get_name());
					}
					send_infos(channel_vec[i], channel_name, c_client);
					// send_infos(channel_vec[i], channel_name, channel_vec[i].client_list[0]);
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
			for (size_t j = 0; j < client_vec.size(); j++) {
				if (client_vec[j].getNickname() == client_nickname)
				{
					client_vec[j].set_current_channel(new_channel.get_name());
					client_vec[j].in_channel += 1;
					break ;
				}  
			}
			send_infos(new_channel, channel_name, c_client);
			std::cout << "-creation of new chan named [" <<channel_name<< "]\n";
		}	
	}
}

std::string client_list_to_str(std::vector<Client> c_list)
{
	std::string list_str;
	list_str = "[users list] = <";
	for (size_t i = 0; i < c_list.size(); i++) {
		list_str += c_list[i].getNickname();
		if (i + 1 == c_list.size())
			break ;
		list_str += "> <";
	}
	list_str += ">";
	return list_str;
}

void		send_infos(Channel &channel, std::string channel_name, Client &client)
{
	std::string	nick		= client.getNickname();
	std::string username	= client.getUsername();

	std::string	list_of_members = client_list_to_str(channel.client_list);
	channel.send_string_all(RPL_JOIN(user_id(nick, username), channel_name));
	channel.send_string_all(RPL_PRIVMSG(nick, username, channel.get_name(), list_of_members));
	if (!channel.get_description().empty()) 
		channel.send_string_all(RPL_TOPIC(client.getNickname(), channel_name, channel.get_description()));
	else
		channel.send_string_all(RPL_NOTOPIC(client.getNickname(), channel_name));
		
	// channel.send_string_all(RPL_NAMREPLY(username, channel_name, list_of_members));
	// channel.send_string_all(RPL_ENDOFNAMES(username, channel_name));
	//marche pas
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

/*	Channel names are strings (beginning with specified prefix characters). Apart from the requirement 
	of the first character being a valid channel type prefix character; the only restriction on a channel 
	name is that it may not contain any spaces (' ', 0x20), a control G / BELL ('^G', 0x07), 
	or a comma (',', 0x2C) (which is used as a list item separator by the protocol).
*/
std::string get_channel_name(const std::string& arg) 
{
	std::string channel_name;
	bool name_find = false;
	for (std::size_t i = 0; i < arg.length(); ++i) 
	{
		char c = arg[i];
		if (!name_find) 
		{
			if (!std::isspace(c) && c != ' ' && c != '\a' && c != ',' && c != '#') {
				channel_name += c;
				name_find = true;
			}
		} 
		else 
		{
			if (c == ' ' || c == '\a' || c == ',')
				break;
			else if (!std::isprint(c))
				break;
			else
				channel_name += c;
		}
	}
	return channel_name;
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
