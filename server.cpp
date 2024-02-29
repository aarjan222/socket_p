#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

/*
    PORT NUMBER AND IP ADDRESS SHOULD BE SAME FOR BOTH SERVER AND CLIENT!!!!!
*/
#define PORT 3333

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    float client_data[3];

    /*
        1. CREATE THE SOCKET
        AF_INET = for communicating between processes on different host connected by IPV4
        SOCK_STREAM = FOR TCP(reliable, connection-oriented protocol)
        protocol = 0 (value for INTERNET PROTCOL)
    */
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cerr << "socket failed" << std::endl;
        return -1;
    }

    /*
        2. SETSOCKOPT
        completely optional
        TO REUSE ADDRESS AND PORT
    */
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        std::cerr << "setsockopt failed" << std::endl;
        return -1;
    }

    /*
        3. BIND
        BINDS THE SOCKET TO ADDRESS AND PORT NUMBER specified in addr(CUSTOM DATA STRUCTURE)
        we bind the server to the LOCALHOST, using INADDR_ANY to specify the IP ADDRESS.
    */

    //    CUSTOM DATA STURCTURE
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "bind failed" << std::endl;
        return -1;
    }

    /*
        4. LISTEN
        puts the SERVER in PASSIVE mode.
        waits for the CLIENT to approcah the SERVER to make a CONNECTION.
    */
    if (listen(server_fd, 3) < 0)
    {
        std::cerr << "listen failed" << std::endl;
        return -1;
    }

    /*
        5. ACCEPT
        EXTRACTS the first CONNECTION REQUEST ON THE QUEUE for LISTENING THE QUEUE.
        CREATES A NEW SOCKET
        AT THIS POINT OF TIME CLIENT, SERVER CONNECTION IS ESTABLISHED!!!  ENJOY!!!
    */
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        std::cerr << "accept failed" << std::endl;
        return -1;
    }

    while (true)
    {
        // READ FROM CLIENT
        // CLEAR BUFFER(input) BEFORE READING
        memset(client_data, 0, sizeof(client_data));
        ssize_t valread = read(new_socket, &client_data, sizeof(client_data));
        if (valread < 0)
        {
            std::cerr << "read failed" << std::endl;
            return -1;
        }

        // PRINT RECEIVED DATA
        std::cout << "Received data from client:" << std::endl;
        for (int i = 0; i < 3; i++)
        {
            std::cout << "client_data " << i << " = " << client_data[i] << std::endl;
        }
        memset(client_data, 0, sizeof(client_data));

        // SEND DATA TO CLIENT
        float server_data[3] = {1123.2342140, 28098.0234, 1231233.0089709};
        ssize_t bytes_sent = send(new_socket, &server_data, sizeof(server_data), 0);
        if (bytes_sent < 0)
        {
            std::cerr << "send failed" << std::endl;
            return -1;
        }
        /*
            CHANGE DELAY VALUES TO ENSURE
                1. DATA ISNOT MISPLACED.
                2. DATA RECEIVED IN CORRECT ORDER.
        */
        sleep(0.1);
    }

    // CLOSE THE NEW SOCKET
    close(new_socket);
    // CLOSE THE SOCKET
    close(server_fd);

    return 0;
}
