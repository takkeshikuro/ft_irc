#include "../../inc/Server.hpp"


/**
 * @brief The PART command removes the client from the given channel(s).
 * 
 * 	Syntax: PART <channel>{,<channel>} [<reason>]
 * 
 *  On sending a successful PART command, the user will receive a PART message 
 *  from the server for each channel they have been removed from. 
 *  <reason> is the reason that the client has left the channel(s).
 * 
 *  Numeric Replies:
 *   ERR_NEEDMOREPARAMS (461)
 *   ERR_NOSUCHCHANNEL (403)
 *   ERR_NOTONCHANNEL (442)
 * 	
 * 	Example:
 * 	[IRSSI] /PART #test,#hey :Dining
 * 	[SERVER] leaves both channels #test and #hey with the reason "Dining"
 * 	[SERVER to CLIENT]"@user_id PART #channel Dining" (for EACH channel they leave)
 */

void    Server::part(std::string buffer, Client c_client)
{
    
}