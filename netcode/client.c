#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdint.h>

#define PORT 4000
#define SERVER_IP "127.0.0.1"

int main() 
{
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in serverAddr;
    uint64_t number;

    WSAStartup(MAKEWORD(2,2), &wsaData);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    printf("Connected to server.\n");

    // Receive number
    recv(sock, (char*)&number, sizeof(number), 0);
    number = ntohl(number);
    printf("Received number: 0x%x\n", number);

    // Add 0x44
    number = number + 0x44;
    printf("After adding 0x44: 0x%x\n", number);

    // Send it back
    uint64_t netNum = htonl(number);
    send(sock, (char*)&netNum, sizeof(netNum), 0);

    closesocket(sock);
    WSACleanup();
    return 0;
}
