#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "movie.h"

void search_using_name(char *str)
{
    FILE *infile;
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
            printf ("id = %d name = %s rating = %d\n", input.id,input.name, input.rating);
            return ;
        }
    }
    printf("\n data not found");
    fclose (infile);
}

void search_using_id(int search_id)
{
    FILE *infile;
    struct Movie input;
    infile = fopen ("Movie.dat", "r");
    if (infile == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        exit (1);
    }
    while(fread(&input, sizeof(struct Movie), 1, infile))
    {
        if(input.id==search_id)
        {
            printf ("id = %d name = %s rating = %d\n", input.id,input.name, input.rating);
            return ;
        }
    }
    printf("\n data not found");
    fclose (infile);
}

void read_file_till_num(int n)
{
    FILE *infile;
    struct Movie input;
    infile = fopen ("Movie.dat", "r");
    if (infile == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        exit (1);
    }
    int count=0;
    while(fread(&input, sizeof(struct Movie), 1, infile) && count<n)
    {
        printf ("id = %d name = %s rating = %d\n", input.id,input.name, input.rating);
        count++;
    }
    fclose (infile);
}

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


void write_file()
{
    FILE *outfile;
	outfile = fopen ("Movie.dat", "a");
	if (outfile == NULL)
	{
		fprintf(stderr, "\nError opening file\n");
		exit (1);
	}
    struct Movie input;
    scanf("%[^\n]",input.name);
    for(int i = 0; input.name[i]; i++)
    {
        input.name[i] = tolower(input.name[i]);
    }
    scanf ("%d", &input.id);
    scanf ("%d", &input.rating);
	fwrite (&input, sizeof(struct Movie), 1, outfile);
	if(fwrite != 0)
		printf("contents to file written successfully !\n");
	else
		printf("error writing file !\n");
	fclose (outfile);
}

/* reads and prints the entire file to stdout */
void read_file()
{
    FILE *infile;
    struct Movie input;
    infile = fopen ("Movie.dat", "r");
    if (infile == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        exit (1);
    }
    while(fread(&input, sizeof(struct Movie), 1, infile))
        printf ("id = %d name = %s rating = %d\n", input.id,input.name, input.rating);
    fclose (infile);
}

/* return the number of movie data stored */
int num_of_data()
{
    FILE *infile;
    struct Movie input;
    infile = fopen ("Movie.dat", "r");
    if (infile == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        return -1;
    }
    int count=0;
    while(fread(&input, sizeof(struct Movie), 1, infile))
        count++;
    fclose (infile);
    return count;
}