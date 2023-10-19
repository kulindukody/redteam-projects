#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netinet/in.h>


#define PORT_NUMBER 6969
#define MAX_CLIENTS 10

void printInterfaceAddresses(){
    struct ifaddrs *ifaddr, *ifa;

    if(getifaddrs(&ifaddr) == -1){
      perror("getifadders");
      exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next){
       if(ifa->ifa_addr == NULL){
         continue;
       }
       if(ifa->ifa_addr->sa_family == AF_INET) {
         struct sockaddr_in *sa = (struct sockaddr_in *)ifa->ifa_addr;
         char ip_address[INET6_ADDRSTRLEN];
         inet_ntop(AF_INET, &(sa->sin_addr), ip_address, INET6_ADDRSTRLEN);

         printf("Interface: %s, IP Address: %s\n", ifa->ifa_name, ip_address);
       }

    }
    freeifaddrs(ifaddr);
}

int main()
{
    printf("Network Interfaces:\n");
    printInterfaceAddresses();

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(6969);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket,(struct sockaddr*)&server_address, sizeof(server_address));

    printf("Server Address: %s:%d\n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));


    listen(server_socket, 10);
    printf("TCP Server is Listening...\n");

    int client_socket = accept(server_socket, NULL, NULL);

    char buffer[1024];
    recv(client_socket, buffer, sizeof(buffer), 0);
    send(client_socket, "Server: Message Recived", sizeof("Server: Message Recived"), 0);

    close(client_socket);
    close(server_socket);
}
