#include "thread.h"
#include "socketserver.h"
#include <stdlib.h>
#include <time.h>
#include <list>
#include <vector>
#include <algorithm>

using namespace Sync;
// Need to create threads that handle client

class ClientThread : public Thread {

    private:
        ByteArray data; // Store data var
        Socket& socket;

    public:
        ClientThread(Socket& socket) // Instantiate the socket
        : socket(socket)
        {}

        ~ClientThread()
        {}

    virtual  long ThreadMain() {
       while(true) {
        //    try {
            socket.Read(data);
            std::string str_input = data.ToString();

            std::reverse(str_input.begin(), str_input.end());
            data = ByteArray(str_input);
            socket.Write(data);
        //    }
        //    catch (...) {}
       }
       return 0;
    }
};

// Thread that handles the server operations
class ServerThread : public Thread {
    private:
        SocketServer& server; // Will accept incoming request for stream sockets from any ip

        // create flag for terminating
        bool flag = true;

    public:
        ServerThread(SocketServer &server)
        : server(server) {}

        ~ServerThread() {
            flag = false;
            std::cout << "Threads were killed" << std::endl;
        }


    virtual long ThreadMain() {
        while(flag) {
            // try {
                // Wait for a client socket connection
                Socket* newConnection = new Socket(server.Accept());

                // A reference to this pointer
                Socket& socketReference = *newConnection;
                ClientThread* clientThread = new ClientThread(socketReference);
            // } catch (...){
            //     std::cout << " Server Shutdown" << std::endl;
            // }
        }
	return 1;
    }
};


int main(void) {
    std::cout << "I am a server." << std::endl;
	
    // Create the server
    SocketServer server(3000);    

    // Need thread to perform the server operations
    ServerThread serverThread(server);
	
    // Wait for input to shutdown server
    FlexWait cinWaiter(1, stdin);
    cinWaiter.Wait();
    // Extract from stream
    std::cin.get();

    // Shut down and clean up the server
    server.Shutdown();
}
