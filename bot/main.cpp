#include <cstdlib> 
#include <iostream> 

int main() {

    // Construct the command to execute the Bash script with the provided port
    std::string command = "/mnt/nfs/homes/tmorikaw/Documents/ft_irc/bot/script.sh";

    // Execute the command
    system(command.c_str());
    
    return 0;
}