#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "movie.h"

extern struct UserNode* head1;
extern struct UserNode* tail1;
extern int l_size;

struct UserNode* getNewUserNode(char *str, char *ptr, char* ftr,float rating) {
    struct UserNode* node = (struct UserNode*) malloc(sizeof(struct UserNode));
    node->next = NULL;
    strcpy(node->username, str);
    strcpy(node->passwd,ptr);
    strcpy(node->movie[0].film,ftr);
    node->movie[0].filmrating = rating;
    return node;
}

void copyFromUserToNode(struct UserNode* node, struct User* user) {
    strcpy(node->username,user->username);
    strcpy(node->passwd,user->passwd);
    int i=0;
    while(1)
    {
        strcpy(node->movie[i].film,user->movie[i].film);
        node->movie[i].filmrating=user->movie[i].filmrating;
        if(user->movie[i].filmrating==-1)
        {
            break;
        }
        i++;
    }    
}

void copyFromNodeToUser(struct UserNode* node, struct User* user) {
    strcpy(user->username,node->username);
    strcpy(user->passwd,node->passwd);
    int i=0;
    while(1)
    {
        strcpy(user->movie[i].film,node->movie[i].film);
        user->movie[i].filmrating=node->movie[i].filmrating;
        if(node->movie[i].filmrating==-1)
        {
            break;
        }
        i++;
    }    
}

struct UserNode* search_using_username(char* str)
{
    //changing the name to lowercase - for uniformity           
    for(int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
    printf("searching user %s...\n", str);
    struct UserNode* itr = head1;
    if(head1==NULL)
    {
    printf("hello\n");
    }
    printf("hello1\n");
    printf("hello3.1\n");
    while( itr != NULL){
        printf("hello2\n");
        if(strcmp(itr->username,str) == 0){
            return itr;
        }
        itr = itr->next;
    }  

    printf("hello3\n");
    return itr;                                                    
}

int add_new_movie(char* uname,char *mname,float rating)                  //to add movie to the user's  rating list
{
    struct UserNode* input = search_using_username(uname);
    if(input==NULL)
    {
        printf("Username does not exist to add movie");
        return -1;
    }
    int i=0,f=0,prerating;
    while(1)
    {
      if(strcmp(input->movie[i].film,mname)==0)
      {
          f=1;
          break;
      }
      if(strcmp(input->movie[i].film,";")==0)
      {
          f=0;
          break;
      }
      i++;
    }
    if(f==1)                  //user has already rated the movie, so changing it to new one
    {
        prerating=input->movie[i].filmrating;
        input->movie[i].filmrating=rating;
    }
    else if(f==0)             //adding movie and rating to the list
    {
        strcpy(input->movie[i].film,mname);
        input->movie[i].filmrating=rating;
        strcpy(input->movie[i+1].film,";");
        input->movie[i+1].filmrating=-1;
        prerating=-1;
    }
    return prerating;
}
int add_new_user_node(char* uname,char* psd)
{
    for(int i = 0; uname[i]; i++){
        uname[i] = tolower(uname[i]);
    }
    struct UserNode* input = NULL;//search_using_username(uname);
    printf("%s\n",uname);
    if( input!=NULL ){
        printf("Username already exists\n");
        return 0;
    }
    else{
        struct UserNode* newNode = getNewUserNode(uname,psd,";",-1);
        if(tail1 == NULL){
            tail1 = newNode;
            head1 = newNode;
        }
        else{
            tail1->next = newNode;
            tail1 = newNode;
        }
        ++l_size;
        printf("Username added\n");
        return 1;
    }
}

void write_userfile(){
    FILE *outfile;
	outfile = fopen ("User.dat", "wb");
	if (outfile == NULL){
		fprintf(stderr, "\nError opening file\n");
		exit (1);
	}
    struct User input;
    struct UserNode* cur  = head1;
    while(cur!=NULL){
        strcpy(input.username,cur->username);
        strcpy(input.passwd,cur->passwd);
        int i=0;
        while(1)                                               
        {
          strcpy(input.movie[i].film,cur->movie[i].film);
          input.movie[i].filmrating=cur->movie[i].filmrating;
          if(cur->movie[i].filmrating==-1)
          {
             break;
          }
          i++;
        }
        fwrite(&input,sizeof(struct User),1,outfile);
        cur=cur->next;
    }
    fclose(outfile);
}

void read_userfile(){
    FILE *infile;
    head1 = NULL;
    tail1 = head1;
    infile = fopen ("User.dat", "rb");
    if (infile == NULL){
        fprintf(stderr, "\nError opening file read_userfile\n");
        exit (1);
    }
    struct User input;
    while(fread(&input, sizeof(struct User), 1, infile)){
        printf("while\n");
        struct UserNode* newNode = getNewUserNode("\0","\0",";",-1);
        copyFromUserToNode(newNode, &input);
        if(head1 == NULL) {
            head1 = newNode;
            tail1 = head1;
        } else {
            tail1->next = newNode;
            tail1 = newNode;
        }
        l_size++;
    }
    fclose (infile);
}


