#include "../../../inc/Server.hpp"

void	Server::mode_o(Channel &chan, Client c_client, std::string target, char sign)
{
	int	idx_target;
	int	idx_target_op;
	size_t	j;
	(void)c_client;
	// find target in server
	for (j = 0; j < client_vec.size(); j++){
		if (client_vec[j].getNickname() == target)
			break ;
	}
	if (j >= client_vec.size())
		return ;
	//check if target is in channel
	idx_target = index_channel_nick(target, chan);
	if (idx_target == -1)
		return ;

	//check if target is op the push back to ope vector
	idx_target_op = index_operator_nick(target, chan);
	if (idx_target_op == -1 && sign == '+') // if target not an operator yet, add it
		chan.add_operator(client_vec[j]);
	if (sign == '-' && idx_target_op > 0) // if target is an operator, remove it
		chan.rm_operator(client_vec[j]);
	
	std::cout << "Operators in " << chan.get_name() << "\n";
	for (size_t i = 0; i < chan.op_clients.size(); i++)
		std::cout << chan.op_clients[i].getNickname() << std::endl;
}