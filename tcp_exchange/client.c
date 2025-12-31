#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

int main() 
{
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    char buffer[512];

    WSAStartup(MAKEWORD(2,2), &wsa);

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    server.sin_family = AF_INET;
    server.sin_port = htons(4444);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    connect(sock, (struct sockaddr*)&server, sizeof(server));

    send(sock, "Hello from client", 17, 0);

    int bytes = recv(sock, buffer, sizeof(buffer)-1, 0);
    buffer[bytes] = '\0';
    printf("[+] Server replied: %s\n", buffer);

    closesocket(sock);
    WSACleanup();

    return 0;
}