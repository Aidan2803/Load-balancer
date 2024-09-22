#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <exception>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <SocketWrapper.hpp>

int main(int argc, char** argv) {
    try {
        const char* port = "8080";
        if (argc >= 2) {
            port = argv[1];
        } else {
            std::cout << "No port specified, default 8080 will be used" << std::endl;
        }

        struct addrinfo hints{};
        struct addrinfo* ai;

        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        if (getaddrinfo(NULL, port, &hints, &ai) != 0) {
            throw std::runtime_error("Failed to get address info");
        }

        SocketWrapper server_socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);

        if (bind(server_socket.GetSocketFileDescriptor(), ai->ai_addr, ai->ai_addrlen) == -1) {
            throw std::runtime_error(std::string("Bind failed: ") + strerror(errno));
        }

        freeaddrinfo(ai);

        if (listen(server_socket.GetSocketFileDescriptor(), 1) == -1) {
            throw std::runtime_error(std::string("Listen failed: ") + strerror(errno));
        }

        while (true) {
            try {
                struct sockaddr_storage client_addr{};
                socklen_t client_len = sizeof(client_addr);

                int client_fd = accept(server_socket.GetSocketFileDescriptor(), (struct sockaddr*)&client_addr, &client_len);
                if (client_fd == -1) {
                    throw std::runtime_error(std::string("Accept failed: ") + strerror(errno));
                }

                SocketWrapper client_socket(client_fd);

                char request_buffer[1024];
                ssize_t bytes_received = recv(client_socket.GetSocketFileDescriptor(), request_buffer, sizeof(request_buffer), 0);
                if (bytes_received <= 0) {
                    throw std::runtime_error("Failed to receive data");
                }

                const char* response =
                    "HTTP/1.1 200 OK\r\n"
                    "Connection: close\r\n"
                    "Content-Type: text/plain\r\n\r\n"
                    "Local time is: ";

                ssize_t bytes_sent = send(client_socket.GetSocketFileDescriptor(), response, strlen(response), 0);
                if (bytes_sent == -1) {
                    throw std::runtime_error(std::string("Failed to send response: ") + strerror(errno));
                }

                time_t timer;
                time(&timer);
                char* time_msg = ctime(&timer);

                bytes_sent = send(client_socket.GetSocketFileDescriptor(), time_msg, strlen(time_msg), 0);
                if (bytes_sent == -1) {
                    throw std::runtime_error(std::string("Failed to send time message: ") + strerror(errno));
                }

                std::cout << "Sent bytes " << bytes_sent << "\n";

            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
