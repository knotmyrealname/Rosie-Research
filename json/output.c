/**
    @file output.c
    @author Daniel Dong
    Contains output functions for all, invalid, and valid inputs
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <json_object.h>
#include <json_tokener.h>

#include "input.h"
#include "utility.h"
#include "output.h"

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
    @return A value based on the success of the function; true when successful and false when unsuccessful
*/
bool generate_all_output(struct json_object *parsed_input, StringArray *output_types, int depth) {
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
    return false;

  // Generate the output
  char *spaces = spacing(depth);
  printf("%s(%d, %d) %s", spaces, json_object_get_int(start),
         json_object_get_int(end), json_object_get_string(type));

  // Check to see if the data should be printed out
  if (contains(output_types, type)) {
    printf(" \x1b[36m%s\x1b[0m", json_object_get_string(data));
    output_printed_count++;
  }
  printf("\n");

  // Iterates through the inner json objects, recursively
  struct json_object *subs;
  if (json_object_object_get_ex(parsed_input, "subs", &subs)) {
    int length = json_object_array_length(subs);
    for (size_t x = 0; x < length; x++) {
      // Runs generate_all_output on each sub object
      struct json_object *sub = json_object_array_get_idx(subs, x);
      if (!generate_all_output(sub, output_types, depth + 1)) {
        free(spaces);
        return false;
      }
    }
  }
  free(spaces);
  return true;
}

/**
    Recursively generates output based on the json that was parsed, only printing out segments that are
    part of an invalid transaction.
    This may have the potential to be updated to be more flexible with other rosie patterns, but that will
    have to come in a future update.
    @param parsed_input The json input to parse
    @param in_invalid_tx Whether the parent was an invalid transaction
    @return A value based on the success of the function; true when successful and false when unsuccessful
*/
bool generate_invalid_output(struct json_object *parsed_input, StringArray *invalid_types,
                             StringArray *subtypes, bool in_invalid_tx) {
  // Declare all of the necessary structs
  struct json_object *data;
  struct json_object *type;

  // Gets all of the necessary data from the parsed input
  if (!(json_object_object_get_ex(parsed_input, "data", &data) &&
        json_object_object_get_ex(parsed_input, "type", &type)))
    return false;

  // Determines whether to print output
  bool is_invalid_tx = contains(invalid_types, type);
  if (is_invalid_tx || (contains(subtypes, type) && in_invalid_tx)) {
    printf("%s\n", json_object_get_string(data));
    output_printed_count++;
  }

  // Recursively runs through all of the sub objects
  struct json_object *subs;
  if (json_object_object_get_ex(parsed_input, "subs", &subs)) {
    int length = json_object_array_length(subs);
    for (size_t x = 0; x < length; x++) {
      // Runs generate_invalid_output on each sub object
      struct json_object *sub = json_object_array_get_idx(subs, x);
      if (!generate_invalid_output(sub, invalid_types, subtypes, is_invalid_tx))
        return false;
    }
  }
  return true;
}

/**
    Recursively generates output based on the json that was parsed, only printing out segments that are
    part of an valid transaction.
    This may have the potential to be updated to be more flexible with other rosie patterns, but that will
    have to come in a future update.
    @param parsed_input The json input to parse
    @param in_invalid_tx Whether the parent was an invalid transaction
    @return A value based on the success of the function; true when successful and false when unsuccessful
*/
bool generate_valid_output(struct json_object *parsed_input, StringArray *valid_types, StringArray *subtypes) {
  // Declare all of the necessary structs
  struct json_object *data;
  struct json_object *type;

  // Gets all of the necessary data from the parsed input
  if (!(json_object_object_get_ex(parsed_input, "data", &data) &&
        json_object_object_get_ex(parsed_input, "type", &type)))
    return false;

  // Checks to see if the current transaction is valid
  bool is_valid_type = contains(valid_types, type);

  // Checks to see if there are subs
  struct json_object *subs;
  if (json_object_object_get_ex(parsed_input, "subs", &subs)) {
    int length = json_object_array_length(subs);
    // Different behavior for when the current transaction is valid or not
    if (is_valid_type) {
      // Keeps track of the skipped objects (those that weren't a valid subtype)
      int skipped_capacity = ARRAY_START_SIZE;
      int skipped_count = 0;
      size_t *skipped = (size_t *) malloc(skipped_capacity * sizeof(size_t));
      for (size_t x = 0; x < length; x++) {
        // Initializes json objects
        struct json_object *sub = json_object_array_get_idx(subs, x);
        struct json_object *sub_data;
        struct json_object *sub_type;

        // Checks to ensure that the json objects are created properly
        if (!(json_object_object_get_ex(sub, "data", &sub_data) &&
              json_object_object_get_ex(sub, "type", &sub_type))) {
           free(skipped);
           return false;
        }
        // If the current object is a valid subtype, print it out
        if (contains(subtypes, sub_type)) {
          printf("%s\n", json_object_get_string(sub_data));
          output_printed_count++;
        }
        else {
          // The current object is not a valid subtype, add it to the list of skipped objects
          skipped[skipped_count++] = x;
          if (skipped_count >= skipped_capacity) {
            skipped_capacity = skipped_count * ARRAY_REALLOC_FACTOR;
            skipped = (size_t *) realloc(skipped, skipped_capacity * sizeof(size_t));
          }
        }
      }
      printf("\n");
      // Recursively run this function on the skipped objects
      for (int x = 0; x < skipped_count; x++) {
        struct json_object *sub = json_object_array_get_idx(subs, skipped[x]);
        if (!generate_valid_output(sub, valid_types, subtypes)) {
          free(skipped);
          return false;
        }
      }
      free(skipped);
    }
    else {
      // Is not a valid type, just recursively run function on the sub objects
      for (size_t x = 0; x < length; x++) {
        struct json_object *sub = json_object_array_get_idx(subs, x);
        if (!generate_valid_output(sub, valid_types, subtypes))
          return false;
      }
    }
  }
  return true;
}


