/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands2.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marecarrayan <marecarrayan@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 21:41:39 by marecarraya       #+#    #+#             */
/*   Updated: 2024/03/12 23:45:51 by marecarraya      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

void    Server::MODE_oprt(Channel &chan, std::string args[3], Client c_client)
{
    size_t i;
    size_t j;
    (void)c_client;
    for (j = 0; j < client_vec.size(); j++)
    {
        if (client_vec[j].getUsername() == args[2])
            break ;
    }
    for (i = 0; i < chan.op_clients.size(); i++)
    {
        if (chan.client_list[i].getNickname() == args[2])
            break ;
    }
    if (i == chan.op_clients.size())
    {
        std::string err = "Error: " + args[2] + " is not in channel " + args[0];
    }
    if (args[1] == "+o")
        chan.add_operator(client_vec[j]);
    if (args[1] == "-o")
        chan.rm_operator(client_vec[j]);
}

void Server::check_MODE_args(std::string args[3], Client c_client)
{
    size_t  i;
    size_t  j;
    std::string ops;
    
    for (i = 0; i < channel_vec.size(); i++)
    {
        if (args[0] == channel_vec[i].get_name() 
            || args[0] == (channel_vec[i].get_name() + "\n"))
            break ;
    }
    if (i == channel_vec.size())    //check if the channel exists
    {
        std::string not_in_chan = "Error: channel #" + args[0] + " does not exists\n";
        send(c_client.get_client_fd(), not_in_chan.c_str(), not_in_chan.size(), 0);
        return ;
    }
    for (j = 0; j < channel_vec[i].op_clients.size(); j++)
    {
        if (c_client.getUsername() == channel_vec[i].op_clients[j].getUsername())
            break ;
    }
    if (j == channel_vec[i].op_clients.size())    //check if user is an operator in the active channel
    {
        std::string str = "You do not have the permissions to use /MODE in #"
                        + channel_vec[i].get_name() + "\n";
        send(c_client.get_client_fd(), str.c_str(), str.size(), 0);
        return ;
    }
    if (args[1] == "-o" || args[1] == "+o")
        MODE_oprt(channel_vec[i], args, c_client);
        
    if (args[1].empty())
    {
        for (size_t k = 0; k < channel_vec[i].op_clients.size(); k++)
            ops += channel_vec[i].op_clients[k].getNickname() + "\n";
        std::string ops_list = "Operators in #" + channel_vec[i].get_name() + ":\n" + ops;
        send(c_client.get_client_fd(), ops_list.c_str(), ops_list.size(), 0);
    }
    return ;
}

void    Server::MODE(std::string buffer, Client c_client)
{
    (void) c_client;
    std::stringstream   sbuf(buffer);
    std::string cmd;
    std::string args[3];
    std::getline(sbuf, cmd, ' ');
/*  parsing of /MODE cmd
    par exemple : /MODE      channel        -o        username
                   cmd       args[0]      args[1]      args[2]
*/
    while (std::getline(sbuf, args[0], ' '))
    {
        if (!args[0].empty())
            break;
    }
    while (std::getline(sbuf, args[1], ' '))
    {
        if (!args[1].empty())
            break;
    }
    while (std::getline(sbuf, args[2], '\n'))
    {
        if (!args[2].empty())
            break;
    }
    // std::cout << "0 : " << args[0] << "\n";
    // std::cout << "1 : " << args[1] << "\n";
    // std::cout << "2 : " << args[2] << "\n";
    check_MODE_args(args, c_client);
}