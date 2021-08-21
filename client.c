#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
        
#include "movie.h"

#define MAX 20              //max input size

void menu();

int main(){
    int socket_fd = socket(PF_INET, SOCK_STREAM, 0);

    if (socket_fd <= 0) {
        printf("Error in creating socket\n");
        return -1;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_family = PF_INET;

    if(inet_pton(PF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ( connect(socket_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection error\n");
        return -1;
    }
    printf("Connected to server\n");
    
    menu(socket_fd);

    close(socket_fd);
    return 0;
}

void add_rating(int choice,int socket_fd){
    int mov_id;
    char mov_name[NAMESIZE];
    float movierating;
    printf("Enter the movie name:");
    scanf("%[^\n]%*c",mov_name);
    printf("Enter the movie rating:");
    scanf("%f",&movierating);
    send(socket_fd,&choice,sizeof(int),0);
    send(socket_fd,mov_name,NAMESIZE,0);
    send(socket_fd,&movierating,sizeof(float),0);
}

void menu(int socket_fd){
    // used to check if user wants to end the connection
    //return to main() function if exit_flag is 1 
    int exit_flag=0;

    int option;
    //loop unitl user wants to exit
    while (exit_flag==0){
        printf("%d. Add new movie rating\n",ADD_RATING);
        printf("%d. View movie rating\n",VIEW_RATING);
        printf("%d. Exit\n",EXIT);
        printf("enter option: ");
        scanf("%d",&option);
        getchar();
        if(option==ADD_RATING){
            //function to add rating
            add_rating(option,socket_fd);
            //exit_flag=1;
        }
        else if(option==VIEW_RATING){
            //function to view movie rating
        }else if(option==EXIT){
            exit_flag=1;
        }else{
            printf("Incorrect option\n\n");
        }
    }
}