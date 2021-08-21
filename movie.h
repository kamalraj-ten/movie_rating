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
    int id;
    char name[NAMESIZE];
    float rating;
};

struct MovieNode {
    int id;
    char name[NAMESIZE];
    float rating;
    struct MovieNode* next;
};

struct Movie search_using_name(char *str);
struct Movie search_using_id(int search_id);
struct MovieNode* read_file_till_num(int n);
void write_file(char* movieName, float rating); // rating is of float type
struct MovieNode* read_file();
int num_of_data();

//linked list related funciton decalrations
struct MovieNode* getNewNode();
void copyFromMovieToNode(struct MovieNode* node, struct Movie* movie);
int getNewId();

// global linked list variable
struct MovieNode* head;
struct MovieNode* tail;
int ll_size;
#endif