#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 9999
#define BUF_SIZE 1024

int main() 
{
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server_addr, client_addr;
    int client_len = sizeof(client_addr);
    char buffer[BUF_SIZE];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) 
    {
        printf("WSAStartup failed\n");
        return 1;
    }

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) 
    {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) 
    {
        printf("Bind failed\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("UDP server listening on port %d...\n", PORT);

    while (1) {
        int bytes = recvfrom(
            sock,
            buffer,
            BUF_SIZE - 1,
            0,
            (struct sockaddr*)&client_addr,
            &client_len
        );

        if (bytes == SOCKET_ERROR)
            break;

        buffer[bytes] = '\0';

        printf("Received from %s:%d -> %s\n",
            inet_ntoa(client_addr.sin_addr),
            ntohs(client_addr.sin_port),
            buffer
        );
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
