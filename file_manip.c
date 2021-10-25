#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "movie.h"
#include "userfile_manip.c"

extern struct MovieNode* head;
extern struct MovieNode* tail;
extern int ll_size;

struct MovieNode* getNewNode(char *str, float rating, char user[][40]) {
    struct MovieNode* node = (struct MovieNode*) malloc(sizeof(struct MovieNode));
    node->next = NULL;
    strcpy(node->name, str);
    node->rating = rating;
    int i=0;
    while(1)
    {
        strcpy(node->users[i],user[i]);
        if(strcmp(user[i],";")==0)
        {
            break;
        }        
        i++;
    }
    return node;
}

void copyFromMovieToNode(struct MovieNode* node, struct Movie* movie) {
    node->rating = movie->rating;
    strcpy(node->name, movie->name);
    int i=0;
    while(1)
    {
        strcpy(node->users[i],movie->users[i]);
        if(strcmp(movie->users[i],";")==0)
        {
            break;
        }        
        i++;
    }
}

void copyFromNodeToMovie(struct MovieNode* node, struct Movie* movie) {
    movie->rating = node->rating;
    strcpy(movie->name, node->name);
    int i=0;
    while(1)
    {
        strcpy(movie->users[i],node->users[i]);
        if(strcmp(node->users[i],";")==0)
        {
            break;
        }
        i++;
    }
}

int getNewId() {
    return ll_size+1;
}

struct MovieNode* search_using_name(char* str)
{
    //changing the name to lowercase - for uniformity           
    for(int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
    printf("serching movie %s...\n", str);
    struct MovieNode* itr = head;
    while( itr != NULL){
        if(strcmp(itr -> name,str) == 0){
            return itr;
        }
        itr = itr->next;
    }                   
    return itr;                                                    
}

struct MovieNode* search_using_id(int search_id)
{
    struct MovieNode* curr = head;
    while (curr != NULL) {
        if (curr->id == search_id) {
            return curr;
        }
        curr = curr->next;
    }
    return curr;
}

                                                                    
void add_new_movie_node(char* movieName, float rating,char* uName){
    
    for(int i = 0; movieName[i]; i++){
        movieName[i] = tolower(movieName[i]);
    }
    for(int i = 0; uName[i]; i++){
        uName[i] = tolower(uName[i]);
    }

    struct MovieNode* input = search_using_name(movieName);
    //input.id = getNewId();
    if( input!=NULL ){              //if movie is present
        int i=0,f=0,l;
        while(1)
        {
            if(strcmp(input->users[i],uName)==0)                //if user has rated the movie before
            {
                f=1;
                l=i;
            }
            if(strcmp(input->users[i],";")==0)
            {
                break;
            }
            i++;
        }
        //adding movie to the user's rated list  and function returns the previous rating if user rated already
        int prerating=add_new_movie(uName,movieName,rating);               
        
        if(f==1)          // changing the user's previous rating to new rating
        {
           int sum=(input->rating)*(i+1);
           if(prerating!=-1)
           {
              sum=sum-prerating+rating;                         
           }
           input->rating=sum/(i+1);
        }
        else if(f==0)          // adding the user to movie's user list
        {
            strcpy(input->users[i],uName);
            strcpy(input->users[i+1],";");
            input->rating = (input->rating + rating)/2;
        }

    }
    else{          //if movie is not present
        int prerating=add_new_movie(uName,movieName,rating);
        char user[10][40];
        strcpy(user[0],uName);
        strcpy(user[1],";");
        struct MovieNode* newNode = getNewNode(movieName,rating,user);
        newNode->id = getNewId();
        tail->next = newNode;
        tail = newNode;
        ++ll_size;
    }

}
void printll(){
    struct MovieNode* cur  = head;
    printf("%-20s\t%s\n", "MOVIE", "RATING");
    printf("-------------------------------------\n");
    while(cur!=NULL){
        printf("%-20s\t%f\n",cur->name,cur->rating);
        cur = cur->next;
    }
}

void write_file(){
    FILE *outfile;
	outfile = fopen ("Movie.txt", "w");
	if (outfile == NULL){
		fprintf(stderr, "\nError opening file\n");
		exit (1);
	}
    struct Movie input;
    struct MovieNode* cur  = head;
    while(cur!=NULL){
        strcpy(input.name,cur->name);
        input.rating = cur->rating;
        int i=0;
        while(1)
        {
            strcpy(input.users[i],cur->users[i]);
            if(strcmp(cur->users[i],";")==0)
            {
                break;
            }
            i++;
        }
        fwrite(&input,sizeof(struct Movie),1,outfile);
        cur=cur->next;
    }
    fclose(outfile);
}

// simplifying the read_file
void read_file() {
    ll_size = 0;
    FILE *infile;
    head = NULL;
    tail = head;

    infile = fopen ("Movie.dat", "r");
    if (infile == NULL){
        fprintf(stderr, "\nError opening file\n");
        exit (1);
    }

    struct Movie input;
    while(fread(&input, sizeof(struct Movie), 1, infile)){
        char user[][40]={";"};
        struct MovieNode* newNode = getNewNode("\0",-1,user);
        copyFromMovieToNode(newNode, &input);
        if (head == NULL) {
            head = newNode;
            tail = head;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        ll_size++;
    }
    fclose (infile);
}


// struct MovieNode* read_file_till_num(int n)
// {
//     FILE *infile;
//     struct Movie input;
//     struct MovieNode* head = NULL;
//     struct MovieNode* tail = head;

//     infile = fopen ("Movie.dat", "r");
//     if (infile == NULL){
//         fprintf(stderr, "\nError opening file\n");
//         exit (1);
//     }

//     int count=0;
//     while(fread(&input, sizeof(struct Movie), 1, infile) && count<n)
//     {
//         //printf ("id = %d name = %s rating = %d\n", input.id,input.name, input.rating);
//         struct MovieNode* newNode = getNewNode();
//         copyFromMovieToNode(newNode, &input);
//         if (head == NULL) {
//             head = newNode;
//             tail = head;
//         } else {
//             tail->next = newNode;
//             tail = newNode;
//         }
        
//         count++;
//     }
//     fclose (infile);
//     return head;
// }
// void read_file()
// {
//     FILE *infile;
//     struct Movie input;
//     infile = fopen ("Movie.dat", "r");
//     if (infile == NULL)
//     {
//         fprintf(stderr, "\nError opening file\n");
//         exit (1);
//     }
//     while(fread(&input, sizeof(struct Movie), 1, infile))
//         printf ("id = %d name = %s rating = %d\n", input.id,input.name, input.rating);
//     fclose (infile);
// }

/* reads and prints the entire file to stdout */
// void read_file()
// {
//     FILE *infile;
//     struct Movie input;
//     infile = fopen ("Movie.dat", "r");
//     if (infile == NULL)
//     {
//         fprintf(stderr, "\nError opening file\n");
//         exit (1);
//     }
//     while(fread(&input, sizeof(struct Movie), 1, infile))
//         printf ("id = %d name = %s rating = %d\n", input.id,input.name, input.rating);
//     fclose (infile);
// }

/* return the number of movie data stored */
// int num_of_data()
// {
//     FILE *infile;
//     struct Movie input;
//     infile = fopen ("Movie.dat", "r");
//     if (infile == NULL)
//     {
//         fprintf(stderr, "\nError opening file\n");
//         return -1;
//     }
//     int count=0;
//     while(fread(&input, sizeof(struct Movie), 1, infile))
//         count++;
//     fclose (infile);
//     return count;
// }

/*struct Movie search_using_name(char *str)
{
    /*FILE *infile;
    struct Movie input;
    for(int i = 0; str[i]; i++)
    {
        str[i] = tolower(str[i]);
    }
    infile = fopen ("Movie.dat", "r");
    if (infile == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        exit (1);
    }
    while(fread(&input, sizeof(struct Movie), 1, infile))
    {
        if(strcmp(str,input.name)==0)
        {
            //printf ("id = %d name = %s rating = %d\n", input.id,input.name, input.rating);
            return input;
        }
    }
    printf("\n data not found");
    
    fclose (infile);
    return NULL;
}*/
// struct Movie search_using_id(int search_id)
// {
//     FILE *infile;
//     struct Movie input;
//     infile = fopen ("Movie.dat", "r");
//     if (infile == NULL)
//     {
//         fprintf(stderr, "\nError opening file\n");
//         exit (1);
//     }
//     while(fread(&input, sizeof(struct Movie), 1, infile))
//     {
//         if(input.id==search_id)
//         {
//             //printf ("id = %d name = %s rating = %d\n", input.id,input.name, input.rating);
//             return input;
//         }
//     }
//     printf("\n data not found");
//     fclose (infile);
// }