#include "../../../inc/Server.hpp"

void    Server::mode_i(Channel &chan, Client c_client, char sign)
{
    if (chan.get_invite_set() == false && sign == '+')
    {
        std::string tosend = MODE_USERMSG(chan.get_name(), "+i");
        if (c_client.get_is_irssi() == false)
            tosend = chan.get_name() + " is now in invite only mode.\n";
        send(c_client.get_client_fd(), tosend.c_str(), tosend.size(), 0);
        chan.set_invite_set();
    }
    if (chan.get_invite_set() == true && sign == '-')
    {
        std::string tosend = MODE_USERMSG(chan.get_name(), "-i");
        if (c_client.get_is_irssi() == false)
            tosend = chan.get_name() + " is not in invite mode anymore.\n";
        send(c_client.get_client_fd(), tosend.c_str(), tosend.size(), 0);
        chan.set_invite_set();
    }
}