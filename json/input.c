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
    Reads input from a file pointer and returns it as an array of character pointers, with a '\0'
    to mark the end. The input is deliminated by the specified delimiter, as well as a default '\n'.
    The user can free the returned memory by calling the provided free_delimited_input function on the object.
    @param fp The file pointer to read from
    @param delimiter The character to deliminate input by.
    @return A dynamically array of character pointers (ie strings)
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
  char prev = '\n';

  // Iterate until the end of input
  while (next != EOF) {
    if ((next == delimiter || next == '\n')) {
      // If we get a duplicate delimiter match, we ignore it
      // May be undesireable for certain forms of input, but it's better for our use case
      if (prev != next) {
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
      }
    }
    else {
      // Normal Character, pass to word array
      word[word_size++] = next;
      if (word_size >= word_capacity) {
        word_capacity = word_size * ARRAY_REALLOC_FACTOR;
        word = (char *) realloc(word, word_capacity * sizeof(char));
      }
    }
    prev = next;
    next = fgetc(fp);
  }
  free(word);
  input[input_size] = '\0';

  StringArray *input_struct = (StringArray *) malloc(sizeof(StringArray));
  input_struct->data = input;
  input_struct->size = input_size;

  return input_struct;
}

StringArray *config_helper(char *config_name, char *next_config_name, StringArray *input, int index) {
  StringArray *subconfig = (StringArray *) malloc(sizeof(StringArray));
  int capacity = ARRAY_START_SIZE;
  int size = 0;
  char **data = (char **) malloc(capacity * sizeof(char *));

  char *next_string = input->data[index + size];
  size++;
  if (strcmp(config_name, next_string) != 0) {
    subconfig->size = -1;
    return subconfig;
  }
  while (size + index < input->size && strcmp(next_config_name, next_string) != 0) {
    data[size - 1] = input->data[index + size];
    next_string = input->data[index + size];
    size++;
    if (size >= capacity) {
      capacity = size * ARRAY_REALLOC_FACTOR;
      data = (char **) realloc(input, capacity * sizeof(char *));
    }
  }
  subconfig->size = size - 2;
  subconfig->data = data;
  return subconfig;
}

/**
    Gets input and, if it's the correct format, parses it as a Config struct
    @param fp The file pointer to read from
    @return A config struct containing StringArrays of every config subtype
*/
Config *getConfig(FILE *fp) {
  StringArray *input = read_delimited_input(fp, ' ');
  Config *config = (Config *) malloc(sizeof(Config));
  if (input->size < MIN_CONFIG_SIZE)
    config_msg();

  int index_a = 0;
  StringArray *subconfig = config_helper("all:", "invalid:", input, index_a);
  config->all_output_types = subconfig;


  int index_b = index_a + subconfig->size + 1;
  subconfig = config_helper("invalid:", "invalid_sub:", input, index_b);
  config->invalid_output_types = subconfig;

  int index_c = index_b +  subconfig->size + 1;
  subconfig = config_helper("invalid_sub:", "valid:", input, index_c);
  config->invalid_subtypes = subconfig;

  int index_d = index_c +  subconfig->size + 1;
  subconfig = config_helper("valid:", "valid_sub:", input, index_d);
  config->valid_output_types = subconfig;

  int index_e = index_d +  subconfig->size + 1;
  subconfig = config_helper("valid_sub:", "", input, index_e);
  config->valid_subtypes = subconfig;

  if (config->all_output_types->size == -1 || config->invalid_output_types->size == -1 ||
      config->invalid_subtypes->size == -1 || config->valid_output_types->size == -1 ||
      config->valid_subtypes->size == -1) {
    config_msg();
    free_string_array(input);
    free_only_config(config);
    fclose(fp);
    exit(EXIT_FAILURE);
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
