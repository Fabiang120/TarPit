#include <iostream>
#include <chrono>
#include <unordered_map>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <queue>
#include <fcntl.h>

// The objective of this program is to be part of a bigger honeypot on a virtual cloud and waste the resources of attackers
constexpr int PORT = 2222;
constexpr int BUFFER_SIZE = 1024;
using Clock = std::chrono::steady_clock;
// Each connection will be saved as a guest and will have an entry for its time logging
namespace {
    struct guest {
        Clock::time_point last_byte_time;
        std::size_t banner_offset = 0;
    };
    struct Entry {
        Clock::time_point next_send;
        int file_descriptor{};
        bool operator>(const Entry& other) const {
            return next_send > other.next_send;
        }
    };
}
int main()
{
    int server_fd = socket(AF_INET6, SOCK_STREAM, 0);
    if(server_fd < 0){
        perror("socket failed");
        return 1;
    }
    // Put server in non block mode so it can read multiple connections
    fcntl(server_fd, F_SETFL, O_NONBLOCK);
    // Watch port 2222 on ::1
    // First bind socket to ip / port
    sockaddr_in6 hint{};
    hint.sin6_family = AF_INET6;
    hint.sin6_port = htons(PORT);
    // Parses text string into ipv6 address binary network format
    // Writes into hint.sin_addr as in_addr struct
    inet_pton(AF_INET6, "::1", &hint.sin6_addr);

    // Second deliver hint card to kernel along with socket file descriptor
    if(bind(server_fd, reinterpret_cast<sockaddr *>(&hint), sizeof(hint)) < 0){
        perror("bind failed");
        close(server_fd);
        return 1;
    }

    // Third mark socket for listening
    // For every connection consistently listen for if were are getting input
    if(listen(server_fd, SOMAXCONN) < 0){
        perror("listen failed");
        close(server_fd);
        return 1;
    }
    // Priority queue to save incoming connections and how close they are to deadlines
    // This will be passed to poll() when to wake up connections and ask have I received something
    std::priority_queue<Entry, std::vector<Entry>, std::greater<>> pq;
    // Job is to tell which sockets are actively talking to server_fd
    std::vector<struct pollfd> pollfds;
    short revents = 0;
    // Revents only change when os alerts after poll is used
    pollfds.emplace_back(server_fd, POLLIN, revents);
    std::unordered_map<int,guest> guest_cabinet;
    // Need a while loop that checks priority_queue
    while (true) {
        int timeout = -1;
        // Check if anything has changed with the server or new conn
        // poll updates revents
        poll(pollfds.data(), pollfds.size(), timeout);
        if ((pollfds[0].revents & POLLIN) != 0) {
            // Accept new conn
            while (true) {
                struct sockaddr_storage client_addr{};
                socklen_t client_len = sizeof(client_addr);
                // Puts client info into client addr, and clent_len
                int client_fd = accept(server_fd, reinterpret_cast<struct sockaddr *>(&client_addr), &client_len);
                if (client_fd < 0 ) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        break;
                    }
                    perror("accept failed");
                    break;
                }
                pollfds.emplace_back(client_fd,POLLIN,revents);
                using namespace std::chrono_literals;
                guest client{.last_byte_time = Clock::now(), .banner_offset = 0};
                Entry client_pqueue_entry = {.next_send = client.last_byte_time + 10s, .file_descriptor = client_fd};
                pq.emplace(client_pqueue_entry);
                guest_cabinet.emplace(client_fd, client);
                // Loop = has (currentTime- guest.last_sent) = 10 seconds // no do nothing yes proceed
                // Keep their connection open by slow tcp packets entire while loop
                // Must have a waiter create a hash map of connection number and timestamp you check every time something happens like a new packet, or wakes up after 1000 millisecond
                // Use non blocking sockets
                // Check after if send() returns -1 and errno is not saying try again later
                // if not -1
                // Send 1 byte out of vector of ssh
                // If send() is -1 then close(fd)
                // Record timestamp, leave socket alone until timer expires
                // Send byte #2 and continue
            }
        }
    }
    return 0;
}