#include <iostream>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <poll.h>

#include <SocketWrapper.hpp>

int main(int argc, char *argv[]){

    if (argc < 3) {
        fprintf(stderr, "usage: client hostname port\n");
        return 1;
    }

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));

    struct addrinfo *peer_addr;

    hints.ai_socktype = SOCK_STREAM;

    if(getaddrinfo(argv[1], argv[2], &hints, &peer_addr)){
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", errno);
        return 1;
    }

    printf("Remote address is: ");
    char address_buffer[100];
    char service_buffer[100];
    getnameinfo(peer_addr->ai_addr, peer_addr->ai_addrlen,
            address_buffer, sizeof(address_buffer),
            service_buffer, sizeof(service_buffer),
            NI_NUMERICHOST);
    printf("%s %s\n", address_buffer, service_buffer);


    SocketWrapper socket_wrapper(peer_addr->ai_family, peer_addr->ai_socktype, peer_addr->ai_protocol);


    if(connect(socket_wrapper.GetSocketFileDescriptor(), peer_addr->ai_addr, peer_addr->ai_addrlen)){
        fprintf(stderr, "connect() failed. (%d)\n", errno);
        return 1;
    }

    freeaddrinfo(peer_addr);

    char read[15] = "Client message";
    printf("Sending: %s\n", read);
    int bytes_sent = send(socket_wrapper.GetSocketFileDescriptor(), read, strlen(read), 0);
    printf("Sent %d bytes.\n", bytes_sent);

    struct pollfd poll_fds;
    poll_fds.fd =socket_wrapper.GetSocketFileDescriptor();
    poll_fds.events = POLLIN;

    while(true){
        if (poll(&poll_fds, 1, -1) > 0){
            char read[4096];
            int bytes_received = recv(socket_wrapper.GetSocketFileDescriptor(), read, 4096, 0);
            if (bytes_received < 1) {
                printf("Connection closed by peer.\n");
                break;
            } 
            printf("Received (%d bytes):\n %.*s",
                    bytes_received, bytes_received, read);
        }
    }
    
}