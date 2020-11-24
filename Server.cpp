#include "thread.h"
#include "socketserver.h"
#include <stdlib.h>
#include <time.h>
#include <list>
#include <vector>
#include <algorithm>

using namespace Sync;
//Need to create threads that handle client

class ClientThread : public Thread
{
private:
    //store the data
    ByteArray data;

    Socket& socket;



public:
    ClientThread(Socket& socket) //constructor to instantiate the socket
    : socket(socket)
    {}

    ~ClientThread()
    {}

                        //    Socket& getSocket(){
                        //        return socket;
                        //    }

                        //    void establishConnection(){
                        //        try{
                        //            std::cout <<"Connecting ";
                        //            std::cout.flush();
                        //            socket.Open();
                        //            connection = true;
                        //            std::cout << "Connected" <<std::endl;
                        //
                        //        }catch (std:: string exception){
                        //            std::cout << "Connection failed ("<< exception <<")" << std::endl;
                        //            return;
                        //        }
                        //    }

    virtual  long ThreadMain(){
       while(1){
           try{

               socket.Read(data);
               std::string string_data = data.ToString();

               std::reverse(string_data.begin(), string_data.end());
               data = ByteArray(string_data);

               socket.Write(data);
           }
           catch (...){
              //nothing
           }
       }
       return 0;
    }

};





// This thread handles the server operations
class ServerThread : public Thread
{
private:
    //ptr to the server socket *******
    SocketServer& server; // wil accept incoming request for stream sockets from any ip

    // create flag for termination
    bool flag = false;

public:
    ServerThread(SocketServer &server)
    : server(server)
    {}

    ~ServerThread()
    {
        //Cleanup
        flag = true;
        std::cout <<"Threads have gracefully exited" << std::endl;

    }


    virtual long ThreadMain()
    {
        //wrap in try catch
        while(!flag){
            try{
                // Wait for a client socket connection
                Socket* newConnection = new Socket(server.Accept());

                // A reference to this pointer
                Socket& socketReference = *newConnection;
                ClientThread* clientThread = new ClientThread(socketReference);
            } catch (...){
                std::cout << " Server Shutdown" << std::endl;
            }
        }



	//You can use this to read data from socket and write data to socket. You may want to put this read/write somewhere else. You may use ByteArray
	// Wait for data
        //socketReference.Read(data);
        // Send it back
        //socketReference.Write(data);
	return 1;
    }
};


int main(void)
{
    std::cout << "I am a server." << std::endl;
	
    // Create our server
    SocketServer server(3000);    

    // Need a thread to perform server operations
    ServerThread serverThread(server);

	
    // This will wait for input to shutdown the server
    FlexWait cinWaiter(1, stdin);
    cinWaiter.Wait();
    //extract from stream
    std::cin.get();

    // Shut down and clean up the server
    server.Shutdown();

}
