#ifndef MOVIE_H
#define MOVIE_H

#define PORT 8888   
#define NAMESIZE 40
#define USERSIZE 40
#define PASSWORD 10

//options user can choose
#define ADD_RATING 1        
#define MOD_RATING 2
#define VIEW_RATING 3
#define VIEW_ALL_RATING 4
#define EXIT 5

struct Movie {
    char name[NAMESIZE];
    float rating;
    char users[30][40];
};

struct MovieNode {
    int id;
    char name[NAMESIZE];
    float rating;
    char users[30][40];
    struct MovieNode* next;
};

struct User{
    char username[USERSIZE];
    char passwd[PASSWORD];
    struct movies{
        char film[NAMESIZE];
        float filmrating;
    }
}

void copyFromNodeToMovie(struct MovieNode* node, struct Movie* movie);
struct MovieNode* search_using_name(char* str);
struct MovieNode* search_using_id(int search_id);
void add_new_movie_node(char* movieName, float rating);
void write_file();
void read_file();
void printll();

//linked list related funciton decalrations
struct MovieNode* getNewNode(char *str, float rating, char user[][40]);
void copyFromMovieToNode(struct MovieNode* node, struct Movie* movie);
int getNewId();

// global linked list variable
struct MovieNode* head;
struct MovieNode* tail;
int ll_size;
#endif