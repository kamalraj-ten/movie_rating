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
    int rating;
};

void read_file();
void read_file_till_num(int n);

#endif