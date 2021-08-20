#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 8888
#define NAMESIZE 20

int main()
{
    int socket_fd = socket(PF_INET, SOCK_STREAM, 0);

    if (socket_fd <= 0) {
        printf("Error in creating socket\n");
        return -1;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_family = PF_INET;

    if(inet_pton(PF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ( connect(socket_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection error\n");
        return -1;
    }

    char name[NAMESIZE] = "asuran\0";
    send(socket_fd, name, strlen(name), 0);

    int rating = 4;
    send(socket_fd, &rating, sizeof(rating), 0);

    // trining to send for second time
    char* newname = "theri\0";
    send(socket_fd, newname, strlen(newname), 0);

    int newrating = 3;
    send(socket_fd, &newrating, sizeof(newrating), 0);

    printf("Completed sending data\n");

    return 0;
}