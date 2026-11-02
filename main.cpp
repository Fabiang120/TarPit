#include <iostream>



int main() {
    // Open a server listening on laptop's ip and a specific port
    // Watch port 2222 on 127.0.0.1 
    // If any  tcp packets show up with the SYN flag set
    // handle handhshake and queue them up 

    // Take client socket and add to program's internal list of guests


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