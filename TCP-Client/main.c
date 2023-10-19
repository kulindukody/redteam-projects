#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


int main()
{
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family =AF_INET;
    server_address.sin_port = htons(6969);
    server_address.sin_addr.s_addr = inet_addr("192.168.104.116");

    connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    char message[] = "Hello, server!";
    send(client_socket, message, sizeof(message), 0);

    char buffer[1024];
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Server response: %s\n", buffer);

    close(client_socket);

    return 0;

}
