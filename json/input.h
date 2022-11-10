/**
    @file input.c
    @author Daniel Dong
    Parses FILE pointers into easily readable character arrays
*/

#include <stdio.h>


/** The starting size for the input array */
#define ARRAY_START_SIZE 20

/** The rate at which the input array grows when it fills up */
#define ARRAY_REALLOC_FACTOR 2

/**
    The struct returned when calling the read_delimited_input function, contains an array of char pointers
    and it's size
*/
typedef struct {
  /** The array of character pointers, storing the data */
  char **data;
  /** The size of the data array, ie how many char pointers are being stored */
  int size;
} DelimitedInput;

/**
    Reads input from a file pointer and returns it as a (potentially very long) string
    The user can free the returned memory by simply calling free() on the object.
    @param fp The file pointer to read from
    @return A dynamically allocatedstring containing all the data inside the file pointer
*/
char *read_continous_input(FILE *fp);

/**
    Reads input from a file pointer and returns it as an array of character pointers, with a '\0'
    to mark the end. The input is deliminated by the specified delimiter, as well as a default '\n'.
    The user can free the returned memory by calling the provided free_delimited_input function on the object.
    @param fp The file pointer to read from
    @param delimiter The character to deliminate input by.
    @return A dynamically array of character pointers (ie strings)
*/
DelimitedInput *read_delimited_input(FILE *fp, char delimiter);

/**
    Helper function to free the array of character pointers returned by read_delimited_input.
    @param input The memory to free
*/
void free_delimited_input(DelimitedInput *input);
