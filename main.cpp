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
using Clock = std::chrono::steady_clock;

struct guest{
    // The file number given by the mac os to the program kernel table for the socket connection
    int file_descriptor;
    Clock::time_point last_byte_time;
    size_t banner_offset;
};

int main()
{
    // Open a server listening on laptop's ipv6
    int server_fd = socket(AF_INET6, SOCK_STREAM, 0);
    if(server_fd < 0){
        perror("socket failed");
        return 1;
    }
    // Watch port 2222 on ::1
    // First bind socket to ip / port
    sockaddr_in6 hint{};
    hint.sin6_family = AF_INET6;
    hint.sin6_port = htons(PORT);
    // Parses text string into vlaid ipv6 address binary network format
    // Writes into hint.sin_addr as in_addr struct
    inet_pton(AF_INET6, "::1", &hint.sin6_addr);

    // Second deliver hint card to kernel along with socket file descriptor
    if(bind(server_fd, (sockaddr*)&hint, sizeof(hint)) < 0){
        perror("bind failed");
        close(server_fd);
        return 1;
    }

    // Third mark socket for listening
    // For every connection consistently poll for if were are getting input

    if(listen(server_fd, SOMAXCONN) < 0){
        perror("listen failed");
        close(server_fd);
        return 1;
    }
    // Priority queue to save incoming connections and how close they are to deadlines
    // This will be passed to poll() when to wake up connections and ask have I received something
    using Entry = std::pair<Clock::time_point,int>;
    std::priority_queue<Entry, std::vector<Entry>, std::greater<>> pq;

    // Job is to tell which sockets are actively talking to server_fd
    std::vector<struct pollfd> pollfds;
    short revents = 0;
    // Revents only chane when os alerts
    pollfds.emplace_back(server_fd, POLLIN, revents);

    // Holds the file descriptor to guest link
    std::unordered_map<int,guest> guest_cabinet;
    // Fourth accept place //
    // Need a while loop that checks priority_queue
    while (true) {
        int timeout = -1;
        // Check if anything has changed with the server or new conn
        if (pollfds[0].revents & POLLIN == true) {
            // Accept new conn
            struct sockaddr_storage client_addr;
            socklen_t client_len = sizeof(client_addr);

            // Puts client info into client addr, and clent_len
            int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        }
    }
    // second Register client_fd from accept into pollfds, update priority_queue, upload guests
    //

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