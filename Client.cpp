
#include "thread.h"
#include "socket.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace Sync;

bool done = false;

//re-define clienthread
class ClientThread: public Thread
{
private:
    Socket& socket;
    ByteArray data;
    std::string string_data;

public:
    ClientThread(Socket& socket)
    :socket(socket){}

    ~ClientThread()
    {}

    virtual long ThreadMain(){
        int open = socket.Open();
        while(true){
            std::cout << "Please input your data (done to exit): ";
            std::cout.flush(); // sanitize the input
            //Get the data frm stream
            std::getline(std::cin, string_data);
            if(string_data == "done"){
                std::cout << "Exiting" << std::endl;
                done = true;
                break;
            }

            data = ByteArray(string_data);
            //write to server
            socket.Write(data);
            //Read
            socket.Read(data);
//            if(socket.Read(data) ==0){
//                break;
//            }

            //Convert it to string
            string_data = data.ToString();

            std::cout << "server response: " << string_data << std::endl;

        }
        return 0;

    }
};

int main(void)
{
	// Welcome the user 
	std::cout << "SE3313 Lab 3 Client" << std::endl;

	// Create our socket
	Socket socket("127.0.0.1", 3000);
	ClientThread clientThread(socket);
	while(done == false){
	    sleep(1);
	}
	socket.Close();
	return 0;




	//To write to socket and read from socket. You may use ByteArray
	//socket.Write(data);
	//socket.Read(data);

	socket.Close();

	return 0;
}
