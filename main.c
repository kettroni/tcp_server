#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>


#define PORT 6969

int main()
{
    struct sockaddr addr;
    int s;
    int addr_len = sizeof(addr);
    
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) return s;
    
    // Define the address structure
    addr.sa_family = AF_INET;
    
    // Set the port number in sa_data (first 2 bytes)
    uint16_t port_network = htons(PORT); // Convert to network byte order
    memcpy(addr.sa_data, &port_network, sizeof(port_network));

    // Set the IP address in sa_data (next 4 bytes)
    struct in_addr ip_addr;
    inet_pton(AF_INET, "0.0.0.0", &ip_addr); // Convert IP address to binary form
    memcpy(addr.sa_data + 2, &ip_addr, sizeof(ip_addr)); // Store IP address
    
    bind(s, &addr, addr_len);
    listen(s, 8);

    while (true) {
	int new_socket = accept(s, &addr, (socklen_t*)&addr_len);
	
	const char* message = "Hello, World!";
	send(new_socket, message, strlen(message), 0);
    
	close(new_socket);
    }

    close(s);
    
    return 0;
}
