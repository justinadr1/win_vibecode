#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib") 
int main() 
{
 
    WSADATA wsaData;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
    {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }

    // Create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) 
    {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return 1;
    }

    // Setup server address
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(7000);  // HTTP port
    inet_pton(AF_INET, "localhost", &server.sin_addr);

    // Connect
    if (connect(sock, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) 
    {
        std::cerr << "Connect failed.\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Send HTTP request
    const char* request = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";
    send(sock, request, strlen(request), 0);

    // Receive data
    char buffer[4096];
    int bytes = recv(sock, buffer, sizeof(buffer), 0);

    if (bytes > 0)
    {
        buffer[bytes] = '\0';
        std::cout << buffer;
    }

    // Cleanup
    closesocket(sock);
    WSACleanup();

    return 0;
}
