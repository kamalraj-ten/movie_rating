#ifndef MOVIE_H
#define MOVIE_H

#define PORT 8888   
#define NAMESIZE 20

//options user can choose
#define ADD_RATING 1        
#define MOD_RATING 2
#define VIEW_RATING 3
#define EXIT 4

struct Movie {
    char name[NAMESIZE];
    float rating;
};

struct MovieNode {
    int id;
    char name[NAMESIZE];
    float rating;
    struct MovieNode* next;
};

void copyFromNodeToMovie(struct MovieNode* node, struct Movie* movie);
struct MovieNode* search_using_name(char* str);
struct MovieNode* search_using_id(int search_id);
void add_new_movie_node(char* movieName, float rating);
void write_file();
void read_file();
void printll();

//linked list related funciton decalrations
struct MovieNode* getNewNode(char *str, float rating);
void copyFromMovieToNode(struct MovieNode* node, struct Movie* movie);
int getNewId();

// global linked list variable
struct MovieNode* head;
struct MovieNode* tail;
int ll_size;
#endif