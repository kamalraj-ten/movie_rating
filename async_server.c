#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdlib.h>

#include "movie.h"
#include "file_manip.c"
#include "userfile_manip.c"

#define BUFFERSIZE 1024

//========= GLOBAL VARIABLES ==============================

extern struct MovieNode* head;
extern struct MovieNode* tail;
extern int ll_size;
pthread_mutex_t lock;

//========= MOVIE DEFINITION AND FUNCTIONS =================

void printMovie(struct Movie m) {
    printf("name: %s\trating: %f\n", m.name, m.rating);
}

//========= END MOVIE DEFINITION AND FUNCTIONS ==============

struct UserNode* entryControl (int client_fd) {
    char user_name[NAMESIZE], passwd[PASSWORD];
    int opt1 = 0;

    while (1) {

        home:
        recv(client_fd, &opt1, sizeof(int), 0);
        printf("option1: %d\n",opt1);
        if(opt1 == 1){
            //Sign Up
            printf("entered\n");
            recv(client_fd, user_name, sizeof(user_name), 0);
            recv(client_fd, passwd, sizeof(passwd), 0);
            printf("%s %s\n",user_name,passwd);

            pthread_mutex_lock(&lock);
            int status = add_new_user_node(user_name, passwd);
            pthread_mutex_unlock(&lock);

            printf("%d", status);
            send(client_fd,&status,sizeof(status),0);
            if(status){
                write_userfile();
                goto home;
            }
            else{
                printf("error\n");
                goto home; 
            } 
        }
        else if(opt1 == 2){
            //Log In
            recv(client_fd, user_name, sizeof(user_name), 0);
            recv(client_fd, passwd, sizeof(passwd), 0);
            printf("%s %s\n",user_name,passwd);

            pthread_mutex_lock(&lock);
            struct UserNode* user = search_using_username(user_name);
            pthread_mutex_unlock(&lock);

            int status;
            if(user != NULL && strcmp(user->passwd ,passwd) == 0){
                status = 1;
            }
            else{
                printf("Password:%s\n",user->passwd);
                status = 0;
            }
            send(client_fd,&status,sizeof(status),0);
            printf("Status:%d\n",status);
            if(status){
                return user; // username - successful login
            }
            else{
                printf("error\n");
                goto home;
            }
        }
        else if(opt1 == EXIT){
            return NULL; // exit return
        }
        
    }

}

void* send_recv_req(void* args){
    printf("new client connected\n");
    int client_fd = *(int*)args,opt=-1;

    struct UserNode* user = entryControl(client_fd); // signup and login
    if (user == NULL ) {
        printf("(%s) exiting client connection...\n", "not found");
        return NULL;
    }

    char name[NAMESIZE];
    float rating;
    while(opt != EXIT){
        bzero(name,NAMESIZE);
        recv(client_fd, &opt, sizeof(int), 0);

        // =============== ADD RATING ===========
        // add rating and change rating are same for file system
        // same option for both
        if( opt == ADD_RATING){
            printf("(%s) add a movie\n", user->username);
            recv(client_fd, name, NAMESIZE, 0);
            recv(client_fd, &rating, sizeof(float), 0);
            pthread_mutex_lock(&lock); // locking critical section
            add_new_movie_node( name, rating, user->username);
            pthread_mutex_unlock(&lock); // unlocking the mutex lock
            strcpy(name,"movie rating added successfully!\n");
            send(client_fd,name,NAMESIZE,0);
        }
        
        // ================= VIEW RATING ===============
        else if( opt == VIEW_RATING){
            printf("(%s) view rating\n", user->username);
            recv(client_fd,name,NAMESIZE,0);
            pthread_mutex_lock(&lock);
            struct MovieNode* res = search_using_name(name);
            pthread_mutex_unlock(&lock);
            if(res==NULL){
                float error=-1;
                send(client_fd,&error,sizeof(float),0);
            }else{
                rating = res->rating;
                send(client_fd,&rating, sizeof(float),0);
            }
        }
        
        //=============== VIEW ALL RATING =================
        else if( opt == VIEW_ALL_RATING){
            printf("(%s) view all rating\n", user->username);
            pthread_mutex_lock(&lock);
            send(client_fd,&ll_size,sizeof(int),0);
            struct MovieNode* cur = head;
            while (cur!=NULL){
                strcpy(name,cur->name);
                rating = cur->rating;
                send(client_fd,name,NAMESIZE,0);
                send(client_fd,&rating,sizeof(rating),0);
                cur = cur->next;
            }
            pthread_mutex_unlock(&lock);
        }
    }
    pthread_mutex_lock(&lock);
    write_file();
    write_userfile();
    pthread_mutex_unlock(&lock);

    printf("(%s) exiting client connection...\n", user->username);
}

int main(){

    // reading the file to a linked list
    read_file(); 
    read_userfile();   
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

    //creating mutex lock
    if ((pthread_mutex_init(&lock, NULL)) < 0) {
        printf("Error creating mutex locks\n");
        return -1;
    }
    if (listen(server_fd, 2) < 0){
        printf("Error Listening\n");
        return -1;
    }

    while(1){

        struct sockaddr_in client_addr;
        int client_fd, addr_len = sizeof(client_addr);
        if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addr_len)) < 0){
            printf("Error accepting connection\n");
            return -1;
        }
        pthread_t tid;
        pthread_create(&tid,NULL,send_recv_req,&client_fd);

        // char acceptChoice;
        // printf("\nShould the server stop(y/n):");
        // scanf("%c", &acceptChoice);

        // if (acceptChoice == 'y') break;
    }
    pthread_mutex_destroy(&lock);
    write_file();
    write_userfile();
    return 0;
}
