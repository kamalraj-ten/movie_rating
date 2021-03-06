#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
        
#include "movie.h"

void menu();
void yellow();
void red();
void reset();

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
    yellow();
    printf("%s\n\n",mov_name);
    reset();
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
       red();
       printf("Movie Not Found!!\n\n");
       reset();
   }else{
       yellow();
       printf("Movie Rating is: %.2f\n\n",movierating);
       reset();
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
  yellow();
  while(i<num){
        readval=recv(socket_fd,mov_name,sizeof(mov_name),0);
        readval=recv(socket_fd,&movierating,sizeof(movierating),0);
        printf("Record: %d\nMovie Name: %s\nMovie Rating: %.2f\n\n",i+1,mov_name,movierating);
        i++;
   }
   reset();
   printf("\n\n");
}

//Function to check user credentials
int check_credentials(int choice,int socket_fd, char user_name[40], char passwd[10]){
    int num;
    send(socket_fd,&choice,sizeof(int),0);
    //sending user_name & Password
    send(socket_fd, user_name,40, 0);
    send(socket_fd, passwd, 10, 0);
    printf("Sent\n");
    //receiving status of account
    int readval=recv(socket_fd,&num,sizeof(int),0);
    printf("%d",num);

    return num;
}

void view_user_rating(int choice, int socket_fd) {
    int num,i=0;
    float movierating;
    char mov_name[NAMESIZE];
    
    //sending the user choice to server
    send(socket_fd,&choice,sizeof(int),0);
    
    //receiving number of movies present in file
    int readval=recv(socket_fd,&num,sizeof(num),0);
    yellow();
    printf("\n---------- MY RATINGS -----------\n");
    //Loop to receive movie details from user and display it
    while(i<num){
            readval=recv(socket_fd,mov_name,sizeof(mov_name),0);
            readval=recv(socket_fd,&movierating,sizeof(movierating),0);
            printf("Record: %d\nMovie Name: %s\nMovie Rating: %.2f\n\n",i+1,mov_name,movierating);
            i++;
    }
    reset();
    printf("\n\n");
}


void menu(int socket_fd){
    // used to check if user wants to end the connection
    //return to main() function if exit_flag is 1 
    int exit_flag=0;

    int option, option1;
    //loop unitl user wants to exit
    while (exit_flag==0){
        home:
        printf("1. Sign Up\n");
        printf("2. Log In\n");
        printf("%d. Exit\n",EXIT);
        printf("Enter option: ");
        scanf("%d",&option1);
        getchar();
        char user_name[40], passwd[10];
        if(option1 == 1){
            printf("Enter User Name: ");
            scanf("%s",user_name);
            printf("\nEnter Password: ");
            scanf("%s",passwd);
            //function to send user credentials
            if(check_credentials(option1,socket_fd, user_name, passwd) == 1){
                printf("\nSign Up Successfull ...\n");
                goto home;
            }
            else{
                printf("\nSign Up Failed\n");
                goto home;
            }
        }
        else if(option1 == 2){
            
            printf("Enter User Name: ");
            scanf("%s",user_name);
            printf("\nEnter Password: ");
            scanf("%s",passwd);
            //function to send user credentials
            if(check_credentials(option1,socket_fd, user_name, passwd) == 1){
                yellow();
                printf("\nLog In Succesfull ...\n");
                reset();
                goto login;
            }
            else{
                red();
                printf("\ninvalid username password\n");
                reset();
                goto home;
            }
        }
        else if(option1 == EXIT){
            send(socket_fd,&option1,sizeof(option1),0);
            exit_flag=1;
            break;
        }
        else{
            red();
            printf("Incorrect option\n\n");
            reset();
            goto home;
        }
        login:
            printf("%d. Add new movie rating\n",ADD_RATING);
            printf("%d. modify movie rating\n", MOD_RATING);
            printf("%d. View given movie rating\n",VIEW_RATING);
            printf("%d. View all movie rating\n",VIEW_ALL_RATING);
            printf("%d. View all my ratings\n", VIEW_USER_RATINGS);
            printf("%d. Exit\n",EXIT);
            printf("enter option: ");
            scanf("%d",&option);
            getchar();
            if(option==ADD_RATING || option == MOD_RATING){
                //function to add rating
                option = ADD_RATING;
                add_rating(option,socket_fd);
                goto login;
            }
            else if(option==VIEW_RATING){
                //function to view particular movie rating
                view_rating(option,socket_fd);
                goto login;
            }
            else if(option==VIEW_ALL_RATING){
                //function to view all movies rating
                view_all_rating(option,socket_fd);
                goto login;
            } else if (option == VIEW_USER_RATINGS) {
                view_user_rating(option, socket_fd);
                goto login;
            }
            else if(option==EXIT){
                send(socket_fd,&option,sizeof(option),0);
                goto home;
            }else{
                red();
                printf("Incorrect option\n\n");
                reset();
                goto login;
            }
        
    }
}

void red () {
  printf("\033[1;31m");
}

void yellow() {
  printf("\033[1;33m");
}

void reset () {
  printf("\033[0m");
}