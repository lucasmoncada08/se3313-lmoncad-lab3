
#include "thread.h"
#include "socket.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace Sync;

int main(void) {
	// Welcome the user 
	std::cout << "SE3313 Lab 3 Client" << std::endl;

	// Create our socket
	Socket socket("127.0.0.1", 3000);
    ByteArray data;
    
    std::string str_input;

    socket.Open();

    while (true) {
        std::cout << "Please input your data (done to exit): ";
        std::cout.flush(); // Sanitize input
        std::cin >> str_input;
        if (str_input == "done") {
            std::cout << "Program Exiting" << std::endl;
            break;
        }
        data = ByteArray(str_input);
        socket.Write(data);
        socket.Read(data);
        str_input = data.ToString();
        if (str_input.empty()) {
            std::cout << "Program Exiting" << std::endl;
            break;
        }
        std::cout << "Server sent back: " << str_input << std::endl;
    }

	socket.Close();

	return 0;
}
