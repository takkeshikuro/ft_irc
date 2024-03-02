#include "../inc/irc.hpp"
#include "../inc/Server.hpp"

void	check_entrance(char **av) 
{
	std::string port = av[1];
	//std::string passwd = av[2];
	if (port != "6667")
		throw WrongPortError();
	if (!av[2])
		throw WrongPasswordError(); // modif le msg
}

int main(int ac, char **av) 
{
	if (ac == 3)
	{
		try 
		{
			check_entrance(av);
			Server Server(av[2]);
			signal(SIGINT, Server::SignalHandler); //-> catch the signal (ctrl + c)
  			signal(SIGQUIT, Server::SignalHandler); //-> catch the signal (ctrl + \)
			Server.configuration();
			Server.launch_server();
		}
		catch (std::exception &e) {
			std::cerr << "ERROR : " << e.what() << std::endl;
		}
	}
	else
		std::cerr <<"need ./ft_irc <port> <password>\n";
}