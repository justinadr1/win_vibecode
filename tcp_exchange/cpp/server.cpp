#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

int main() 
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) 
    {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_sock == INVALID_SOCKET) 
    {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(4444);

    if (bind(listen_sock, reinterpret_cast<sockaddr*>(&server), sizeof(server)) == SOCKET_ERROR) 
    {
        std::cerr << "Bind failed\n";
        closesocket(listen_sock);
        WSACleanup();
        return 1;
    }

    if (listen(listen_sock, 1) == SOCKET_ERROR) 
    {
        std::cerr << "Listen failed\n";
        closesocket(listen_sock);
        WSACleanup();
        return 1;
    }

    std::cout << "[+] Server listening on port 4444...\n";

    sockaddr_in client{};
    int client_len = sizeof(client);
    SOCKET client_sock = accept(listen_sock, reinterpret_cast<sockaddr*>(&client), &client_len);
    if (client_sock == INVALID_SOCKET) 
    {
        std::cerr << "Accept failed\n";
        closesocket(listen_sock);
        WSACleanup();
        return 1;
    }

    std::cout << "[+] Client connected\n";

    std::vector<char> buffer(512);
    int bytes = recv(client_sock, buffer.data(), static_cast<int>(buffer.size()) - 1, 0);
    if (bytes > 0) 
    {
        buffer[bytes] = '\0';
        std::cout << "[+] Received: " << buffer.data() << "\n";
    }

    std::string reply = "Hello from server";
    send(client_sock, reply.c_str(), static_cast<int>(reply.size()), 0);

    closesocket(client_sock);
    closesocket(listen_sock);
    WSACleanup();
    return 0;
}
