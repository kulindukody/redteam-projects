#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/time.h>
#include <ifaddrs.h>

#define PORT 6969
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

void broadcast(int sender, int clients[], int client_count, char *message) {
    for (int i = 0; i < client_count; ++i) {
       if (clients[i] != sender) {
          send(clients[i], message, strlen(message), 0);
       }
    }
}

void print_interfaces(){
   struct ifaddrs *ifaddr, *ifa;

   if(getifaddrs(&ifaddr) == -1){
      perror("getifaddrs");
      exit(EXIT_FAILURE);
   }
   for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next){
      if (ifa->ifa_addr == NULL)
      continue;

   if (ifa->ifa_addr->sa_family == AF_INET){
          struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
          printf("Interface: %s, IP Address:%s\n", ifa->ifa_name, inet_ntoa(addr->sin_addr));
      }
   }
   freeifaddrs(ifaddr);
}

int main() {

    printf("IP Addresses on all initerfaces:\n");
    print_interfaces();

    int server_socket, client_socket, client_sockets[MAX_CLIENTS];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    int new_socket;  // Declare new_socket before using it

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
       perror("Error creating socket");
       exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
       perror("Error binding socket");
       exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 10) == -1) {
       perror("Error listening for connections");
       exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    fd_set read_fds;
    int max_fd, activity;
    char buffer[BUFFER_SIZE];

    for (int i = 0; i < MAX_CLIENTS; ++i) {
       client_sockets[i] = 0;
    }

    while (1) {
       FD_ZERO(&read_fds);
       FD_SET(server_socket, &read_fds);
       max_fd = server_socket;

       for (int i = 0; i < MAX_CLIENTS; ++i) {
          if (client_sockets[i] > 0) {
             FD_SET(client_sockets[i], &read_fds);
             if (client_sockets[i] > max_fd) {
                max_fd = client_sockets[i];
             }
          }
       }

       activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);

       if ((activity < 0) && (errno != EINTR)) {
          perror("Error in select");
       }

       if (FD_ISSET(server_socket, &read_fds)) {
          if ((new_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size)) < 0) {
             perror("Error accepting connection");
             exit(EXIT_FAILURE);
          }

          printf("New connection, socket fd is %d, ip is: %s, port: %d\n",
          new_socket, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

          for (int i = 0; i < MAX_CLIENTS; ++i) {
             if (client_sockets[i] == 0) {
                client_sockets[i] = new_socket;
                printf("Adding to list of sockets at index %d\n", i);
                break;
             }
          }
       }

       for (int i = 0; i < MAX_CLIENTS; ++i) {
          if (FD_ISSET(client_sockets[i], &read_fds)) {
             if (recv(client_sockets[i], buffer, BUFFER_SIZE, 0) <= 0) {
                getpeername(client_sockets[i], (struct sockaddr*)&client_addr, (socklen_t*)&addr_size);
                printf("Host disconnected, ip %s, port %d\n",
                   inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                close(client_sockets[i]);
                client_sockets[i] = 0;
             } else {
                broadcast(client_sockets[i], client_sockets, MAX_CLIENTS, buffer);
             }
          }
       }
    }
    return 0;
}
