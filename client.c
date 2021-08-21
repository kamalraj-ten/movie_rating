#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
        
#include "movie.h"

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

//Function to add movie rating
void add_rating(int choice,int socket_fd){
    int mov_id;
    char mov_name[NAMESIZE];
    float movierating;

    //Getting the movie name and rating from user
    printf("Enter the movie name:");
    scanf("%[^\n]%*c",mov_name);
    printf("Enter the movie rating:");
    scanf("%f",&movierating);

    //sending the user choice,movie name and rating to server
    send(socket_fd,&choice,sizeof(int),0);
    send(socket_fd,mov_name,NAMESIZE,0);
    send(socket_fd,&movierating,sizeof(float),0);
    recv(socket_fd,mov_name,NAMESIZE,0);
    printf("%s\n\n",mov_name);
}

//Function to view rating of particular movie
void view_rating(int choice,int socket_fd)
{
   char mov_name[NAMESIZE];
   float movierating;
   int readval;
   
   //sending the user choice to server
   send(socket_fd,&choice,sizeof(int),0);

   //Getting movie name from user
   printf("Enter the movie name:");
   scanf("%[^\n]%*c",mov_name);

   //sending the movie name to user and receiving rating
   send(socket_fd,mov_name,NAMESIZE,0);
   readval=recv(socket_fd,&movierating,sizeof(movierating),0);
   if(movierating==-1){
       printf("Movie Not Found!!\n\n");
   }else{
       printf("Movie Rating is: %.2f\n\n",movierating);
   }
}

//Function to view rating of all movies
void view_all_rating(int choice,int socket_fd)
{
  int num,i=0;
  float movierating;
  char mov_name[NAMESIZE];
  
  //sending the user choice to server
  send(socket_fd,&choice,sizeof(int),0);
  
  //receiving number of movies present in file
  int readval=recv(socket_fd,&num,sizeof(num),0);
  
  //Loop to receive movie details from user and display it
  while(i<num){
        readval=recv(socket_fd,mov_name,sizeof(mov_name),0);
        readval=recv(socket_fd,&movierating,sizeof(movierating),0);
        printf("Record: %d\nMovie Name: %s\nMovie Rating: %.2f\n\n",i+1,mov_name,movierating);
        i++;
   }
   printf("\n\n");
}


void menu(int socket_fd){
    // used to check if user wants to end the connection
    //return to main() function if exit_flag is 1 
    int exit_flag=0;

    int option;
    //loop unitl user wants to exit
    while (exit_flag==0){
        printf("%d. Add new movie rating\n",ADD_RATING);
        printf("%d. View given movie rating\n",VIEW_RATING);
        printf("%d. View all movie rating\n",VIEW_ALL_RATING);
        printf("%d. Exit\n",EXIT);
        printf("enter option: ");
        scanf("%d",&option);
        getchar();
        if(option==ADD_RATING){
            //function to add rating
            add_rating(option,socket_fd);
        }
        else if(option==VIEW_RATING){
            //function to view particular movie rating
            view_rating(option,socket_fd);
        }
        else if(option==VIEW_ALL_RATING){
            //function to view all movies rating
            view_all_rating(option,socket_fd);
        }
        else if(option==EXIT){
            send(socket_fd,&option,sizeof(option),0);
            exit_flag=1;
        }else{
            printf("Incorrect option\n\n");
        }
    }
}