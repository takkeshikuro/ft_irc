#include "../inc/irc.hpp"
#include "../inc/Server.hpp"

void	check_entrance(char **av) 
{
	std::string port = av[1];
	std::string password = av[2];

	if (!port.empty()) 
	{
		if (port.size() < 2 && port.size() > 5)
			throw WrongPortError();
		for (size_t i = 0; i < port.length(); i++) {
			if (!isdigit(port[i]))
				throw WrongPortError();
		}
	}
	if (!password.empty()) {
		if (password.size() > 10)
			throw WrongPasswdSelectionError();
	}
	if (port.empty() || password.empty())
		throw NeedValidParamError();
}

int main(int ac, char **av) 
{
	if (ac == 3)
	{
		try 
		{
			check_entrance(av);
			time_t rawtime;
			struct tm * timeinfo;
			time (&rawtime);
			timeinfo = localtime(&rawtime);
			
			Server Server(av[1], av[2], timeinfo);
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
		std::cerr << "need ./ft_irc <port> <password>\n";
}