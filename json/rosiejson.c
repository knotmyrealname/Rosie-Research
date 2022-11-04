/**
    @file rosiejson.c
    @author Daniel Dong
    Reads in Rosie outputted json and converts it to a more readable format
    Uses the json-c library for parsing
*/

#include <stdlib.h>
#include <stdio.h>
#include <json_object.h>
#include <json_tokener.h>
#include <string.h>

/** The starting size for the input array */
#define ARRAY_START_SIZE 20

/** The rate at which the input array grows when it fills up */
#define ARRAY_REALLOC_FACTOR 2

/**
    Prints out a usage message and exit with error status.
*/
static void usage() {
  printf( "usage:  rosiejson <json-file>\n" );
  exit( EXIT_FAILURE );
}

/**
    Prints out an error message and exits with error status.
*/
static void error(char const *msg) {
  fprintf(stderr, "%s\n", msg);
  exit(EXIT_FAILURE);
}

/**
    Reads input from a file pointer and returns it as a (potentially very long) string
    @param fp The file pointer to read from
    @return A string containing all the data inside the file pointer
*/
char *readInput( FILE *fp ) {
  char *input = (char *) malloc( ARRAY_START_SIZE * sizeof(char) );

  // Checks to see if the file is empty
  char next = fgetc(fp);
  if ( next == EOF ) {
    free(input);
    error("File is empty");
  }

  // Reads in data
  int count = 0;
  int capacity = ARRAY_START_SIZE;
  while (next != EOF) {
    input[count++] = next;
    next = fgetc(fp);
    // Not enough space, increase the size of the data array
    if (count >= capacity) {
      capacity = count * ARRAY_REALLOC_FACTOR;
      input = (char *) realloc( input, capacity * sizeof(char) );
    }
  }
  return input;
}

/**
    Generates the spacing for the output, based on the depth of the data
    @param depth The depth of the entry
    @return The spacing for the entry
*/
char *spacing(int depth) {
  int spaces_count = depth * 2;
  char *spaces = (char *) malloc((spaces_count + 1) * sizeof(char));
  for (int x = 0; x < spaces_count; x++) {
    spaces[x] = ' ';
  }
  spaces[spaces_count] = '\0';
  return spaces;
}

/**
    Recursively generates the output based on the json that was parsed
    @param parsed_input The json input to parse
    @param depth The depth of the input being analyzed
*/
void generate_output(struct json_object *parsed_input, int depth) {
  // Declare all of the necessary structs 
  struct json_object *end;
  struct json_object *start;
  struct json_object *data;
  struct json_object *type;

  // Gets all of the necessary data from the parsed input
  if (!(json_object_object_get_ex(parsed_input, "e", &end) &&
        json_object_object_get_ex(parsed_input, "s", &start) &&
        json_object_object_get_ex(parsed_input, "data", &data) &&
        json_object_object_get_ex(parsed_input, "type", &type))) {
    error("Invalid json data");
  }

  // Generate the output
  char *spaces = spacing(depth);
  printf("%s(%d, %d) %s", spaces, json_object_get_int(start), json_object_get_int(end), json_object_get_string(type));

  // Check to see if the data should be printed out
  const char *type_string = json_object_get_string(type);
  if (strcmp(type_string, "tx_begin_line") == 0 ||
      strcmp(type_string, "end_line") == 0 ||
      strcmp(type_string, "tx_end_line") == 0 ||
      strcmp(type_string, "data_line") == 0 ||
      strcmp(type_string, "incomplete_tx_begin_line") == 0 ||
      strcmp(type_string, "invalid_line") == 0) {
    printf(" \x1b[36m%s\x1b[0m", json_object_get_string(data));
  }
  printf("\n");

  // Iterates through the inner json objects, recursively 
  struct json_object *subs;
  if (json_object_object_get_ex(parsed_input, "subs", &subs)) {
    int length =  json_object_array_length(subs);
    for (size_t x = 0; x < length; x++) {
      struct json_object *sub = json_object_array_get_idx(subs, x);
      generate_output(sub, depth + 1);
    }
  }
  free(spaces);
}

/**
    The main method of rosiejson
    @param argc The length of the argument array
    @param argv The array of command line arguments
    @return The exit status of the program
*/
int main( int argc, char *argv[] ) {
  if ( argc != 2 )
    usage();

  // Opens the file and reads in input
  FILE *fp;
  if ((fp = fopen(argv[ 1 ], "r")) == NULL) {
    error(strcat("Can't open file: ", argv[ 1 ]));
  }
  char *input = readInput(fp);

  // Generate the json objects and print out formatted data
  struct json_object *parsed_input = json_tokener_parse(input);
  if (parsed_input == NULL) {
    error("Unable to parse input for json content");
  }
  generate_output(parsed_input, 0);

  // Free the memory
  while(json_object_put(parsed_input) != 1) {}
  free(input);
  fclose(fp);

  return(EXIT_SUCCESS);
}
