#include "../inc/irc.hpp"
#include "../inc/Server.hpp"

void	check_entrance(char **av) 
{
	std::string port = av[1];
	std::string passwd = av[2];
	if (port != "6667")
		throw WrongPortError();
	if (passwd != "root")
		throw WrongPasswordError();
}

int main(int ac, char **av) 
{
	if (ac == 3)
	{
		try 
		{
			check_entrance(av);
			Server Server;
			Server.configuration();
			Server.launch_server();
		}
		catch (std::exception &e) {
			std::cerr << "ERROR : " << e.what() << std::endl;
		}
	}
	else
		std::cerr <<"okok\n";
}