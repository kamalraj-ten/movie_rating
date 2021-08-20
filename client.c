#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#define PORT 8888           //port number
#define MAX 80              //max input size

//optinons user can choose
#define ADD_RATING 1        
#define MOD_RATING 2
#define VIEW_RATING 3
#define EXIT 4

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

void menu(int socket_fd){
    // used to check if user wants to end the connection
    //return to main() function if exit_flag is 1 
    int exit_flag=0;

    int option;
    //loop unitl user wants to exit
    while (exit_flag==0){
        printf("1. Add new movie rating\n");
        printf("2. Modify movie rating\n");
        printf("3. View movie rating\n");
        printf("4. Exit\n");
        printf("enter option: ");
        scanf("%d",&option);
        if(option==ADD_RATING){
            //function to add rating
        }else if(option==MOD_RATING){
            //function to modify rating
        }else if(option==VIEW_RATING){
            //function to view movie rating
        }else if(option==EXIT){
            exit_flag=1;
        }else{
            printf("Incorrect option\n\n");
        }
    }
}