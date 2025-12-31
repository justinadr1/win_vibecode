#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

int main() 
{
    WSADATA wsa;
    SOCKET listen_sock, client_sock;
    struct sockaddr_in server, client;
    int client_len = sizeof(client);
    char buffer[512];

    WSAStartup(MAKEWORD(2,2), &wsa);

    listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(4444);

    bind(listen_sock, (struct sockaddr*)&server, sizeof(server));
    listen(listen_sock, 1);

    printf("[+] Server listening on port 4444...\n");

    client_sock = accept(listen_sock, (struct sockaddr*)&client, &client_len);
    printf("[+] Client connected\n");

    int bytes = recv(client_sock, buffer, sizeof(buffer)-1, 0);
    buffer[bytes] = '\0';
    printf("[+] Received: %s\n", buffer);

    send(client_sock, "Hello from server", 17, 0);

    closesocket(client_sock);
    closesocket(listen_sock);
    WSACleanup();

    return 0;
}
