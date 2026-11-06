#include <iostream>
#include <chrono>
#include <unordered_map>
#include <vector>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <poll.h>

constexpr int PORT = 2222;
constexpr int BUFFER_SIZE = 1024;

struct guest{
    // The file number given by the mac os to the program kernel table for the socket connection
    int file_descriptor;
    std::chrono::steady_clock::time_point last_byte_time;
    size_t banner_offset;
};

int main()
{
    // Open a server listening on laptop's ip
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0){
        perror("socket failed");
        return 1;
    }
    // Watch port 2222 on 127.0.0.1
    // First bind socket to ip / port
    sockaddr_in hint{};
    hint.sin_family = AF_INET;
    hint.sin_port = htons(PORT);
    // Parses text string into vlaid ipv4 address 4 bte binary network format
    // Writes into hint.sin_addr as in_addr struct
    inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);

    // Second deliver hint card to kernel along with socket file descriptor
    if(bind(server_fd, (sockaddr*)&hint, sizeof(hint)) < 0){
        perror("bind failed");
        close(server_fd);
        return 1;
    }

    // Third mark socket for listening
    if(listen(server_fd, SOMAXCONN) < 0){
        perror("listen failed");
        close(server_fd);
        return 1;
    }

    // Fourth accept place //
    

    // If any  tcp packets show up with the SYN flag set
    // handle handhshake and queue them up

    // Take client socket and add to program's internal list of guests


    // Loop = has (currentTime- guest.last_sent) = 10 seconds // no do nothing yes proceed

    // Keep their connection open by slow tcp packets entire while loop
    // Must have a waiter create a hash map of connection number and timestamp you check every time something happens like a new packet, or wakes up after 1000 millisecond
    // Use non blocking sockets

    // Check after if send() returns -1 and errno isnt saying try again later
    // if not -1 
    // Send 1 byte out of vector of ssh 
    // If send() is -1 then close(fd)

    // Record timestamp, leave socket alone until timer expires

    // Send byte #2 and continue

    return 0;
}