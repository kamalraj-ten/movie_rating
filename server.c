#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>

#include "movie.h"
#include "file_manip.c"

#define BUFFERSIZE 1024

//========= GLOBAL VARIABLES ==============================

extern struct MovieNode* head;
extern struct MovieNode* tail;
extern int ll_size;

//========= MOVIE DEFINITION AND FUNCTIONS =================

void printMovie(struct Movie m) {
    printf("name: %s\trating: %f\n", m.name, m.rating);
}

//========= END MOVIE DEFINITION AND FUNCTIONS ==============

int main()
{
    
    // reading the file to a linked list
    read_file();
    
    printll();
    int server_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (server_fd <= 0) {
        printf("Error creating the socket\n");
        return -1;
    }

    //forcefully attaching socket to port
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
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
    if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addr_len)) < 0){
        printf("Error accepting sockets\n");
        return -1;
    }

    printf("client connected\n")
    opt=0;
    int opt1 = 0;
    char user_name[40], passwd[10];
    while(1){
        recv(client_fd, &opt1, sizeof(int), 0);
        printf("option: %d\n",opt);
        if(opt1 == 1){
            //Sign Up
            recv(client_fd, user_name, 40, 0);
            recv(client_fd, passwd, 10, 0);
        }
        else if(opt1 == 2){
            //Log In
            recv(client_fd, user_name, 40, 0);
            recv(client_fd, passwd, 10, 0);
        }
        else if(opt1 == EXIT){
            break;
        }
    while(1){
        recv(client_fd, &opt, sizeof(int), 0);
        printf("option: %d\n",opt);
        if( opt == ADD_RATING){
            char name[NAMESIZE];
            float rating;
            int readval = recv(client_fd, name, NAMESIZE, 0);
            readval = recv(client_fd, &rating, sizeof(float), 0);
            add_new_movie_node( name, rating);
            strcpy(name,"movie rating added successfully!");
            send(client_fd,name,NAMESIZE,0);
            write_file();
        }else if( opt == VIEW_RATING){
            char name[NAMESIZE];
            recv(client_fd,name,NAMESIZE,0);
            struct MovieNode* res = search_using_name(name);
            if(res==NULL){
                float error=-1;
                send(client_fd,&error,sizeof(float),0);
            }else{
                float rating = res->rating;
                send(client_fd,&rating, sizeof(float),0);
            }
        }else if( opt == VIEW_ALL_RATING){
            send(client_fd,&ll_size,sizeof(int),0);
            struct MovieNode* cur = head;
            char name[NAMESIZE];
            float rating;
            while (cur!=NULL){
                strcpy(name,cur->name);
                rating = cur->rating;
                send(client_fd,name,NAMESIZE,0);
                send(client_fd,&rating,sizeof(rating),0);
                cur = cur->next;
            }
        }else if( opt == EXIT ){
            break;
        }
    }
    write_file();
    }
    return 0;
}
