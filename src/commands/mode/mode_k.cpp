#include "../../../inc/Server.hpp"

bool isAlpha(std::string str) 
{
    for (size_t i = 0; i < str.length(); i++) {
        if (!isalpha(str[i])) 
            return (false);
    }
    return (true);
}

void    Server::mode_k(Channel &chan, Client c_client, std::string keypass, char sign)
{
	if (sign ==  '+') 
	{
		if (isAlpha(keypass) == true)
		{
			if (chan.get_key_set() == false) {
				chan.set_key_set();
				chan.set_keypass_k(keypass);
			}
			else
				chan.set_keypass_k(keypass);
			std::string tosend = MODE_USERMSG(chan.get_name(), "+k");
			if (c_client.get_is_irssi() == false)
				tosend = chan.get_name() + " is now required a new keypass.\n";
			send(c_client.get_client_fd(), tosend.c_str(), tosend.size(), 0);
		}
	}
	else if (sign == '-') {
		if (chan.get_key_set() == true) {
			keypass.clear();
			chan.get_key_set();
			chan.set_keypass_k(keypass);
		}
		std::string tosend = MODE_USERMSG(chan.get_name(), "-k");
		if (c_client.get_is_irssi() == false)
			tosend = chan.get_name() + " is now free to join.\n";
		send(c_client.get_client_fd(), tosend.c_str(), tosend.size(), 0);
	}
}


