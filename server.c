#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>

#define PORT 8888
#define BUFFERSIZE 1024
#define NAMESIZE 20

struct Movie {
    char name[NAMESIZE];
    int rating;
};

void printMovie(struct Movie m) {
    printf("name: %s\trating: %d\n", m.name, m.rating);
}

int main()
{
    int server_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (server_fd <= 0) {
        printf("Error creating the socket\n");
        return -1;
    }

    //forcefully attaching socket to port
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        return -1;
    }

    // binding the socket
    struct sockaddr_in address;
    address.sin_family = PF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;

    if ( bind(server_fd, (struct sockaddr*) &address, sizeof(address)) < 0 ) {
        printf("Error in binding\n");
        return -1;
    }

     // listening
    if (listen(server_fd, 2) < 0)
    {
        printf("Error Listening\n");
        return -1;
    }

    //accepting other sockets
    // currently accepting only one
    int client_fd, addr_len = sizeof(address);
    if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addr_len)) < 0)
    {
        printf("Error accepting sockets\n");
        return -1;
    }

    struct Movie m, a;
    // recv movie
    // name
    int readval = recv(client_fd, m.name, NAMESIZE, 0);
    readval = recv(client_fd, &m.rating, sizeof(int), 0);
    printMovie(m);

    readval = recv(client_fd, a.name, NAMESIZE, 0);
    readval = recv(client_fd, &a.rating, sizeof(int), 0);
    printMovie(a);

    return 0;
}
