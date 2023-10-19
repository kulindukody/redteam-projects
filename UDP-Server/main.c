#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(6868);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    char buffer[1024];
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_address, &client_address_len);
    sendto(server_socket,"Server: Message Recieved", sizeof("Server: Message Recieved"), 0, (struct sockaddr*)&client_address,client_address_len);

    close(server_socket);
}
