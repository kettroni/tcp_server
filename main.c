#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <assert.h>

#define PORT 6969

int main()
{
  struct sockaddr address;
  int s;
  int address_len = sizeof(address);
  printf("[INFO] -------------------------------------\n");
  printf("[INFO] Setting up TCP Server\n");

  printf("[INFO] Generating socket...\n");
  s = socket(AF_INET, SOCK_STREAM, 0);
  assert(s >= 0);
  printf("[INFO] Socket generated.\n");

  printf("[INFO] Defining the address structure...\n");
  address.sa_family = AF_INET;

  // Set the port number in sa_data (first 2 bytes)
  uint16_t port_network = htons(PORT); // Convert to network byte order
  memcpy(address.sa_data, &port_network, sizeof(port_network));

  // Set the IP address in sa_data (next 4 bytes)
  struct in_addr ip_addr;
  inet_pton(AF_INET, "0.0.0.0", &ip_addr); //Convert IP address to binary form
  memcpy(address.sa_data + 2, &ip_addr, sizeof(ip_addr)); // Store IP address
  printf("[INFO] Address structure defined.\n");

  printf("[INFO] Binding an address to the socket...\n");
  bind(s, &address, address_len);
  listen(s, 8);

  printf("[INFO] Setup completed.\n");
  printf("[INFO] -------------------------------------\n");

  while (true) {
    printf("[INFO] Waiting client...\n");
    int new_socket = accept(s, &address, (socklen_t*)&address_len);
    printf("[INFO] Client %d found!\n", new_socket);
    assert(new_socket >= 0);

    char buffer[1024];
    ssize_t message_len = recv(new_socket, &buffer, sizeof(buffer), 0);
    printf("[Client %d] %s\n", new_socket, buffer);

    const char* message = "Hello, World!";
    printf("[INFO] Sending message %s\n", message);
    send(new_socket, message, strlen(message), 0);

    printf("[INFO] Closing client connection %d\n", new_socket);
    close(new_socket);
  }

  close(s);

  return 0;
}
