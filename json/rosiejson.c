/**
    @file rosiejson.c
    @author Daniel Dong
    Reads in Rosie outputted json and converts it to a more readable format
    Uses the json-c library for parsing
*/

#include <stdlib.h>
#include <stdio.h>
#include <json.h>

#define ARRAY_START_SIZE 20

#define ARRAY_REALLOC_FACTOR 2

/**
    Prints out a usage message and exit with error status.
*/
static void usage() {
  printf( "usage:  rosiejson <json-file>\n" );
  exit( EXIT_FAILURE );
}

char *readInput( FILE *fp ) {
  char *input = (char *) malloc( ARRAY_START_SIZE * sizeof(char) );

  char next = fgetc(fp);
  if ( next == EOF ) {
    free(input);
    return NULL;
  }

  int count = 0;
  int capacity = ARRAY_START_SIZE;
  while (next != EOF) {
    input[count++] = next;
    next = fgetc(fp);
    if (count >= capacity) {
      capacity = count * ARRAY_REALLOC_FACTOR;
      input = (char *) realloc( input, capacity * sizeof(char) );
    }
  }
  return input;
}


int main( int argc, char *argv[] ) {
  if ( argc != 2 )
    usage();

  FILE *fp;

  if ((fp = fopen(argv[ 1 ], "r")) == NULL) {
    fprintf(stderr, "Can't open file: %s\n", argv[ 1 ]);
    exit(EXIT_FAILURE);
  }

  char *input = readInput(fp);

  struct json_object *jsonInput = json_tokener_parse(input);

  

  fclose(fp);

  return(EXIT_SUCCESS);
}
