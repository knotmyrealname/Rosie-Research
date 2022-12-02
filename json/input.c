/**
    @file input.c
    @author Daniel Dong
    Parses FILE pointers into easily readable character arrays
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "input.h"
#include "utility.h"

/**
    Reads input from a file pointer and returns it as a (potentially very long) string.
    The user can free the returned memory by simply calling free() on the object.
    @param fp The file pointer to read from
    @return A dynamically allocated string containing all the data inside the file pointer
*/
char *read_continous_input(FILE *fp) {
  // Checks to see if the file is empty
  char next = fgetc(fp);
  if ( next == EOF ) {
    error("File is empty");
  }

  char *input = (char *)malloc(ARRAY_START_SIZE * sizeof(char));

  // Reads in data
  int count = 0;
  int capacity = ARRAY_START_SIZE;
  while (next != EOF) {
    input[count++] = next;
    next = fgetc(fp);
    // Not enough space, increase the size of the data array
    if (count >= capacity) {
      capacity = count * ARRAY_REALLOC_FACTOR;
      input = (char *)realloc(input, capacity * sizeof(char));
    }
  }
  input[count] = '\0';
  return input;
}

/**
    Reads input from a file pointer and returns it as a StringArray, with a '\0'. The input is deliminated by
    the specified delimiter, as well as a default '\n'.
    The user can free the returned memory by calling the provided free_delimited_input function on the object.
    @param fp The file pointer to read from
    @param delimiter The character to deliminate input by.
    @return A StringArray struct containing an array of char pointers (to strings) and the number of strings read
*/
StringArray *read_delimited_input(FILE *fp, char delimiter) {
  // Checks to see if the file is empty
  char next = fgetc(fp);
  if ( next == EOF ) {
    error("File is empty");
  }

  // Allocate Memory
  char **input = (char **) malloc(ARRAY_START_SIZE * sizeof(char *));
  char *word = (char *) malloc(ARRAY_START_SIZE * sizeof(char));

  // Declare Variables
  int input_size = 0;
  int input_capacity = ARRAY_START_SIZE;
  int word_size = 0;
  int word_capacity = ARRAY_START_SIZE;
  bool after_delim = true;

  // Iterate until the end of input
  while (next != EOF) {
    if ((next == delimiter || next == '\n')) {
      // If we get a duplicate delimiter match, we ignore it
      // May be undesireable for certain forms of input, but it's better for our use case
      if (!after_delim) {
        // Finished matching an input phrase, add it to the main array and reset the word array
        word[word_size] = '\0';
        input[input_size++] = word;
        word = (char *) malloc(ARRAY_START_SIZE * sizeof(char));
        word_size = 0;
        word_capacity = ARRAY_START_SIZE;
        // Checks to make sure that there is enough capacity for the next entry
        if (input_size >= input_capacity) {
          input_capacity = input_size * ARRAY_REALLOC_FACTOR;
          input = (char **) realloc(input, input_capacity * sizeof(char*));
        }
        after_delim = true;
      }
    }
    else {
      // Normal Character, pass to word array
      word[word_size++] = next;
      if (word_size >= word_capacity) {
        word_capacity = word_size * ARRAY_REALLOC_FACTOR;
        word = (char *) realloc(word, word_capacity * sizeof(char));
      }
      after_delim=false;
    }
    next = fgetc(fp);
  }
  // Free input and clean
  free(word);
  input[input_size] = '\0';

  // Creates the StringArray data structure for returning
  StringArray *input_struct = (StringArray *) malloc(sizeof(StringArray));
  input_struct->data = input;
  input_struct->size = input_size;

  return input_struct;
}

/**
    Reads a subsection of the input, from the beginning of config_name to next_config_name. This data (exclusive) is read
    into a StringArray and then returned.
    A string of config_name will be expected at input->data[index];
    @param config_name The config name of the current config entry (i.e. the title)
    @param next_config_name The config name of the next expected config entry (if last config, just pass a blank string)
    @param input The StringArray to read input from.
    @param index The index to start parsing the input StringArray
    @param A StringArray containing the subsection of data entries for the current config
*/
StringArray *config_helper(char *config_name, char *next_config_name, StringArray *input, int index) {
  // Initialize variables
  StringArray *subconfig = (StringArray *) malloc(sizeof(StringArray));
  int capacity = ARRAY_START_SIZE;
  int size = 0;
  char **data = (char **) malloc(capacity * sizeof(char *));

  // Checks the first string to make sure it matches config_name and that there is more input to be read
  char *next_string = input->data[index];
  if (index >= input->size || strcmp(config_name, next_string) != 0) {
    subconfig->size = -1;
    subconfig->data = data;
    return subconfig;
  }
  // Loops through the input until the next_config_name is found
  while (size + index < input->size && strcmp(next_config_name, next_string) != 0) {
    // Adds the string to the data (since we know it's not the next config entry)
    data[size] = input->data[index + size + 1];
    next_string = input->data[index + size + 1];
    size++;
    // Ensures there is room in the data array for more entries
    if (size >= capacity) {
      capacity = size * ARRAY_REALLOC_FACTOR;
      data = (char **) realloc(input, capacity * sizeof(char *));
    }
  }
  // Stores data into the subconfig StringArray for returning
  subconfig->size = size - 1;
  subconfig->data = data;
  return subconfig;
}

/**
    Gets input and, if it's the correct format, parses it as a Config struct
    @param fp The file pointer to read from
    @return A config struct containing StringArrays of every config subtype. In the case of an error, NULL is returned
*/
Config *getConfig(FILE *fp) {
  // Reads in input and initializes variables
  StringArray *input = read_delimited_input(fp, ' ');
  Config *config = (Config *) malloc(sizeof(Config));

  // Checks to ensure there is more data than config names
  if (input->size < MIN_CONFIG_SIZE) {
    free(config);
    free_string_array(input);
    return NULL;
  }

  // Parses input for the "all:" subconfig
  int index_a = 0;
  StringArray *subconfig = config_helper("all:", "invalid:", input, index_a);
  config->all_output_types = subconfig;

  // Parses input for the "invalid:" subconfig
  int index_b = index_a + subconfig->size + 1;
  subconfig = config_helper("invalid:", "invalid_sub:", input, index_b);
  config->invalid_output_types = subconfig;

  // Parses input for the "invalid_sub:" subconfig
  int index_c = index_b +  subconfig->size + 1;
  subconfig = config_helper("invalid_sub:", "valid:", input, index_c);
  config->invalid_subtypes = subconfig;

  // Parses input for the "valid:" subconfig
  int index_d = index_c +  subconfig->size + 1;
  subconfig = config_helper("valid:", "valid_sub:", input, index_d);
  config->valid_output_types = subconfig;

  // Parses input for the "valid_sub:" subconfig
  int index_e = index_d +  subconfig->size + 1;
  subconfig = config_helper("valid_sub:", "", input, index_e);
  config->valid_subtypes = subconfig;

  // Checks to see if there are any errors
  if (config->all_output_types->size == -1 || config->invalid_output_types->size == -1 ||
      config->invalid_subtypes->size == -1 || config->valid_output_types->size == -1 ||
      config->valid_subtypes->size == -1) {
    // If there are errors, print config message and free initialized variables
    free_string_array(input);
    free_only_config(config);
    return NULL;
  }


  // Only free the StringArray structure and config handlers, we want to keep the data
  free(input->data[index_a]);
  free(input->data[index_b]);
  free(input->data[index_c]);
  free(input->data[index_d]);
  free(input->data[index_e]);
  free(input->data);
  free(input);

  return config;
}

/**
    Helper funtion to free the memory stored in Config. Does not free the data, only the struct.
    @param config The Config struct to free
*/
void free_only_config(Config *config) {
  free(config->all_output_types->data);
  free(config->all_output_types);
  free(config->invalid_output_types->data);
  free(config->invalid_output_types);
  free(config->invalid_subtypes->data);
  free(config->invalid_subtypes);
  free(config->valid_output_types->data);
  free(config->valid_output_types);
  free(config->valid_subtypes->data);
  free(config->valid_subtypes);
  free(config);
}

/**
    Helper funtion to free the memory stored in Config. Frees both the data and struct.
    @param config The Config struct to free
*/
void free_config_and_data(Config *config) {
  free_string_array(config->all_output_types);
  free_string_array(config->invalid_output_types);
  free_string_array(config->invalid_subtypes);
  free_string_array(config->valid_output_types);
  free_string_array(config->valid_subtypes);
  free(config);
}

/**
    Helper function to free the array of character pointers returned by read_delimited_input.
    @param input The memory to free
*/
void free_string_array(StringArray *input) {
  for (int x = 0; x < input->size; x++) {
    free(input->data[x]);
  }
  free(input->data);
  free(input);
}
