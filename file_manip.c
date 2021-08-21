#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "movie.h"

extern struct MovieNode* head;
extern struct MovieNode* tail;
extern int ll_size;

struct MovieNode* getNewNode(char *str, float rating) {
    struct MovieNode* node = (struct MovieNode*) malloc(sizeof(struct MovieNode));
    node->next = NULL;
    strcpy(node->name, str);
    node->rating = rating;

    return node;
}

void copyFromMovieToNode(struct MovieNode* node, struct Movie* movie) {
    node->rating = movie->rating;
    strcpy(node->name, movie->name);
}

void copyFromNodeToMovie(struct MovieNode* node, struct Movie* movie) {
    movie->rating = node->rating;
    strcpy(movie->name, node->name);
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

void add_new_movie_node(char* movieName, float rating){
    
    for(int i = 0; movieName[i]; i++){
        movieName[i] = tolower(movieName[i]);
    }

    struct MovieNode* input = search_using_name(movieName);
    //input.id = getNewId();
    if( input!=NULL ){
        input->rating = (input->rating + rating)/2;
    }
    else{
        struct MovieNode* newNode = getNewNode(movieName, rating);
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
	outfile = fopen ("Movie.dat", "w");
	if (outfile == NULL){
		fprintf(stderr, "\nError opening file\n");
		exit (1);
	}
    struct Movie input;
    struct MovieNode* cur  = head;
    while(cur!=NULL){
        strcpy(input.name,cur->name);
        input.rating = cur->rating;
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
        struct MovieNode* newNode = getNewNode("\0",-1);
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