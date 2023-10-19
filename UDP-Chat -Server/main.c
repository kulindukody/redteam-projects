#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 6767
#define MAX_MESSAGE_SIZE 1024

int main()
{
    int serverSocket = socket(AF_INET, SOCK_DGRAM,0);
    if(serverSocket == -1){
       perror("Error creating socket");
       exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if(bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr))== -1){
       perror("Error binding socket");
       close(serverSocket);
       exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n",PORT);

    while (1){
       struct sockaddr_in clientAddr;
       socklen_t clientAddrLen = sizeof(clientAddr);
       char buffer[MAX_MESSAGE_SIZE];
       memset(buffer, 0, sizeof(buffer));

       ssize_t receivedBytes = recvfrom(serverSocket, buffer, sizeof(buffer), 0,(struct sockaddr*)&clientAddr, &clientAddrLen);

       if (receivedBytes == -10){
          perror("Error receiving messge");
          continue;
       }
       printf("Recieved from %s:%d: %s\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buffer);

    sendto(serverSocket, buffer, receivedBytes, 0, (struct sockaddr*)&clientAddr, clientAddrLen);
    }

    close(serverSocket);

    return 0;

}
