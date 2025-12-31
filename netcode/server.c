#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdint.h>

#define PORT 4000

int main() 
{
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    uint64_t number = 0xaabbccdd; 
    uint64_t received;

    WSAStartup(MAKEWORD(2,2), &wsaData);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 1);

    printf("Server listening on port %d...\n", PORT);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
    printf("Client connected!\n");

    // Convert to network byte order and send
    uint64_t netNum = htonl(number);
    send(clientSocket, (char*)&netNum, sizeof(netNum), 0);
    printf("Sent number 0x%x to client.\n", number);

    // Receive multiplied number from client
    recv(clientSocket, (char*)&received, sizeof(received), 0);
    received = ntohl(received); // convert back to host byte order
    printf("Received number from client: 0x%x\n", received);

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
