#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
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
void* send_recv_req(void* args){
    int client_fd = *(int*)args,opt=-1;
    char name[NAMESIZE];
    float rating;
    while(opt != EXIT){
        bzero(name,NAMESIZE);
        recv(client_fd, &opt, sizeof(int), 0);
        if( opt == ADD_RATING){
            recv(client_fd, name, NAMESIZE, 0);
            recv(client_fd, &rating, sizeof(float), 0);
            add_new_movie_node( name, rating);
            strcpy(name,"movie rating added successfully!");
            send(client_fd,name,NAMESIZE,0);
        }else if( opt == VIEW_RATING){
            recv(client_fd,name,NAMESIZE,0);
            struct MovieNode* res = search_using_name(name);
            if(res==NULL){
                float error=-1;
                send(client_fd,&error,sizeof(float),0);
            }else{
                rating = res->rating;
                send(client_fd,&rating, sizeof(float),0);
            }
        }else if( opt == VIEW_ALL_RATING){
            send(client_fd,&ll_size,sizeof(int),0);
            struct MovieNode* cur = head;
            while (cur!=NULL){
                strcpy(name,cur->name);
                rating = cur->rating;
                send(client_fd,name,NAMESIZE,0);
                send(client_fd,&rating,sizeof(rating),0);
                cur = cur->next;
            }
        }
    }
}

int main(){

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

    while(1){
        if (listen(server_fd, 2) < 0){
            printf("Error Listening\n");
            return -1;
        }
        struct sockaddr_in client_addr;
        int client_fd, addr_len = sizeof(client_addr);
        if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addr_len)) < 0){
            printf("Error accepting connection\n");
            return -1;
        }
        pthread_t tid;
        pthread_create(&tid,NULL,send_recv_req,&client_fd);
    }
    write_file();
    return 0;
}
