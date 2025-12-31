#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(4444);
    if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) <= 0) {
        std::cerr << "Invalid IP address\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    if (connect(sock, reinterpret_cast<sockaddr*>(&server), sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "Connection failed\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::string message = "Hello from client";
    send(sock, message.c_str(), static_cast<int>(message.size()), 0);

    std::vector<char> buffer(512);
    int bytes = recv(sock, buffer.data(), static_cast<int>(buffer.size()) - 1, 0);
    if (bytes > 0) {
        buffer[bytes] = '\0';
        std::cout << "[+] Server replied: " << buffer.data() << "\n";
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
