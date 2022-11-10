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
#include <stdbool.h>

#include "input.h"
#include "utility.h"

/** The types of json objects to output the data of */
DelimitedInput *output_types;

/**
    Checks the json object type, and if it matches an object in output_types, the data inside the data object is printed
    @param data The json object containing the data to potentially print
    @param type The json object containing the type of the data
    @param output_types The list of types for which data should be output
    @paarm types_count The number of types in the output_types array
*/
void try_print_additional_data(struct json_object *data, struct json_object *type) {
  const char *type_string = json_object_get_string(type);
  for (int x = 0; x < output_types->size; x++) {
    if (strcmp(output_types->data[x], type_string) == 0) {
      printf(" \x1b[36m%s\x1b[0m", json_object_get_string(data));
      x = output_types->size;
    }
  }
}

/**
    Recursively generates the output based on the json that was parsed
    @param parsed_input The json input to parse
    @param depth The depth of the input being analyzed
*/
void generate_all_output(struct json_object *parsed_input, int depth) {
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
  try_print_additional_data(data, type);
  printf("\n");

  // Iterates through the inner json objects, recursively
  struct json_object *subs;
  if (json_object_object_get_ex(parsed_input, "subs", &subs)) {
    int length = json_object_array_length(subs);
    for (size_t x = 0; x < length; x++) {
      struct json_object *sub = json_object_array_get_idx(subs, x);
      generate_all_output(sub, depth + 1);
    }
  }
  free(spaces);
}


void generate_invalid_output(struct json_object *parsed_input) {

}

/**
    The main method of rosiejson
    @param argc The length of the argument array
    @param argv The array of command line arguments
    @return The exit status of the program
*/
int main( int argc, char *argv[] ) {
  if ( argc != 3 )
    usage();

  // Opens json file and reads in input
  FILE *json_fp;
  if ((json_fp = fopen(argv[ 1 ], "r")) == NULL) {
    error(strcat("Can't open file: ", argv[ 1 ]));
  }
  char *json_input = read_continous_input(json_fp);

  FILE *types_fp;
  if ((types_fp = fopen(argv[ 2 ], "r")) == NULL) {
    error(strcat("Can't open file: ", argv[ 2 ]));
  }
  output_types = read_delimited_input(types_fp, ' ');

  // Generate the json objects and print out formatted data
  struct json_object *parsed_input = json_tokener_parse(json_input);
  if (parsed_input == NULL) {
    error("Unable to parse input for json content");
  }
  generate_all_output(parsed_input, 0);

  // Free the memory
  while(json_object_put(parsed_input) != 1) {}
  free(json_input);
  free_delimited_input(output_types);
  fclose(json_fp);
  fclose(types_fp);

  return(EXIT_SUCCESS);
}
