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

#define EXPECTED_ARGS_COUNT 4

#define _A 1

#define _I 2

#define _V 3

/**
    Checks the json object type, and if it matches an object in output_types, the data inside the data object is printed
    @param data The json object containing the data to potentially print
    @param type The json object containing the type of the data
    @param output_types The list of types for which data should be output
    @paarm types_count The number of types in the output_types array
*/
bool contains(StringArray *types, struct json_object *type) {
  const char *type_string = json_object_get_string(type);
  for (int x = 0; x < types->size; x++) {
    if (strcmp(types->data[x], type_string) == 0) {
      return true;
    }
  }
  return false;
}

/**
    Recursively generates output based on the json that was parsed into a more readable format.
    Theoretically, this should work with any rosie json output.
    @param parsed_input The json input to parse
    @param depth The depth of the input being analyzed
*/
void generate_all_output(struct json_object *parsed_input, StringArray *output_types, int depth) {
  // Declare all of the necessary structs
  struct json_object *end;
  struct json_object *start;
  struct json_object *data;
  struct json_object *type;

  // Gets all of the necessary data from the parsed input
  if (!(json_object_object_get_ex(parsed_input, "e", &end) &&
        json_object_object_get_ex(parsed_input, "s", &start) &&
        json_object_object_get_ex(parsed_input, "data", &data) &&
        json_object_object_get_ex(parsed_input, "type", &type)))
    error("Invalid json data");

  // Generate the output
  char *spaces = spacing(depth);
  printf("%s(%d, %d) %s", spaces, json_object_get_int(start),
         json_object_get_int(end), json_object_get_string(type));

  // Check to see if the data should be printed out
  if (contains(output_types, type))
    printf(" \x1b[36m%s\x1b[0m", json_object_get_string(data));
  printf("\n");

  // Iterates through the inner json objects, recursively
  struct json_object *subs;
  if (json_object_object_get_ex(parsed_input, "subs", &subs)) {
    int length = json_object_array_length(subs);
    for (size_t x = 0; x < length; x++) {
      struct json_object *sub = json_object_array_get_idx(subs, x);
      generate_all_output(sub, output_types, depth + 1);
    }
  }
  free(spaces);
}

/**
    Recursively generates output based on the json that was parsed, only printing out segments that are
    part of an invalid transaction.
    This may have the potential to be updated to be more flexible with other rosie patterns, but that will
    have to come in a future update.
    @param parsed_input The json input to parse
    @param in_invalid_tx Whether the parent was an invalid transaction
*/
void generate_invalid_output(struct json_object *parsed_input, StringArray *invalid_types,
                             StringArray *subtypes, bool in_invalid_tx) {
  // Declare all of the necessary structs
  struct json_object *data;
  struct json_object *type;

  // Gets all of the necessary data from the parsed input
  if (!(json_object_object_get_ex(parsed_input, "data", &data) &&
        json_object_object_get_ex(parsed_input, "type", &type))) {
    error("Invalid json data");
  }

  bool is_invalid_tx = contains(invalid_types, type);
  if (is_invalid_tx || (contains(subtypes, type) && in_invalid_tx)) {
    printf("%s\n", json_object_get_string(data));
  }

  struct json_object *subs;
  if (json_object_object_get_ex(parsed_input, "subs", &subs)) {
    int length = json_object_array_length(subs);
    for (size_t x = 0; x < length; x++) {
      struct json_object *sub = json_object_array_get_idx(subs, x);
      if (is_invalid_tx) {
        generate_invalid_output(sub, invalid_types, subtypes, true);
      }
      else {
        generate_invalid_output(sub, invalid_types, subtypes, false);
      }
    }
  }
}

/**
    Recursively generates output based on the json that was parsed, only printing out segments that are
    part of an valid transaction.
    This may have the potential to be updated to be more flexible with other rosie patterns, but that will
    have to come in a future update.
    @param parsed_input The json input to parse
    @param in_invalid_tx Whether the parent was an invalid transaction
*/
void generate_valid_output(struct json_object *parsed_input, StringArray *valid_types,
                           StringArray *subtypes, bool in_valid_tx) {
  // Declare all of the necessary structs
  struct json_object *data;
  struct json_object *type;

  // Gets all of the necessary data from the parsed input
  if (!(json_object_object_get_ex(parsed_input, "data", &data) &&
        json_object_object_get_ex(parsed_input, "type", &type))) {
    error("Invalid json data");
  }

  bool is_valid_tx = contains(valid_types, type);

  struct json_object *subs;
  if (json_object_object_get_ex(parsed_input, "subs", &subs)) {
    int length = json_object_array_length(subs);
    if (is_valid_tx) {
      int skipped_capacity = ARRAY_START_SIZE;
      int skipped_count = 0;
      size_t *skipped = (size_t *) malloc(skipped_capacity * sizeof(size_t));
      for (size_t x = 0; x < length; x++) {
        struct json_object *sub = json_object_array_get_idx(subs, x);
        struct json_object *sub_data;
        struct json_object *sub_type;

        // Gets all of the necessary data from the parsed input
        if (!(json_object_object_get_ex(sub, "data", &sub_data) &&
              json_object_object_get_ex(sub, "type", &sub_type))) {
           error("Invalid json data");
        }
        if (contains(subtypes, type)) {
          printf("%s\n", json_object_get_string(sub_data));
        }
        else {
          skipped[skipped_count++] = x;
          if (skipped_count >= skipped_capacity) {
            skipped_capacity = skipped_count * ARRAY_REALLOC_FACTOR;
            skipped = (size_t *) realloc(skipped, skipped_capacity * sizeof(size_t));
          }
        }
      }
      printf("\n\n\n\n");
      for (int x = 0; x < skipped_count; x++) {
        struct json_object *sub = json_object_array_get_idx(subs, skipped[x]);
        generate_invalid_output(sub, valid_types, subtypes, false);
      }
    }
    else {
      for (size_t x = 0; x < length; x++) {
        struct json_object *sub = json_object_array_get_idx(subs, x);
        generate_invalid_output(sub, valid_types, subtypes, false);
      }
   }
  }

}

/**
    The main method of rosiejson
    @param argc The length of the argument array
    @param argv The array of command line arguments
    @return The exit status of the program
*/
int main(int argc, char *argv[]) {
  if (argc != EXPECTED_ARGS_COUNT)
    usage();

  int state = 0;

  if (strcmp("-a", argv[1]) == 0) {
    state = _A;
  }
  else if (strcmp("-i", argv[1]) == 0) {
    state = _I;
  }
  else if (strcmp("-v", argv[1]) == 0) {
    state = _V;
  }
  else if (strcmp("-c", argv[1]) == 0) {
    config_msg();
    exit(EXIT_SUCCESS);
  }
  else {
    usage();
  }

  // Opens json file and reads in input
  FILE *json_fp;
  if ((json_fp = fopen(argv[ 2 ], "r")) == NULL) {
    error(strcat("Can't open file: ", argv[ 1 ]));
  }
  char *json_input = read_continous_input(json_fp);
  fclose(json_fp);

  FILE *types_fp;
  if ((types_fp = fopen(argv[ 3 ], "r")) == NULL) {
    error(strcat("Can't open file: ", argv[ 2 ]));
  }
  Config *config  = getConfig(types_fp);

  // Generate the json objects and print out formatted data
  struct json_object *parsed_input = json_tokener_parse(json_input);
  if (parsed_input == NULL) {
    error("Unable to parse input for json content");
  }

  switch (state) {
    case _A:
      generate_all_output(parsed_input, config->all_output_types,  0);
      break;
    case _I:
      generate_invalid_output(parsed_input, config->invalid_output_types, config->invalid_subtypes, false);
      break;
    case _V:
      generate_valid_output(parsed_input, config->valid_output_types, config->valid_subtypes, false);
      break;
    default:
      usage();
  }

  // Free the memory
  while(json_object_put(parsed_input) != 1) {}
  free(json_input);
  free_config_and_data(config);
  fclose(types_fp);

  return(EXIT_SUCCESS);
}
