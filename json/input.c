/**
    @file input.c
    @author Daniel Dong
    Parses FILE pointers into easily readable character arrays
*/

#include <stdlib.h>
#include <stdio.h>

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
DelimitedInput *read_delimited_input(FILE *fp, char delimiter) {
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

  DelimitedInput *input_struct = (DelimitedInput *) malloc(sizeof(DelimitedInput));
  input_struct->data = input;
  input_struct->size = input_size;

  return input_struct;
}

/**
    Helper function to free the array of character pointers returned by read_delimited_input.
    @param input The memory to free
*/
void free_delimited_input(DelimitedInput *input) {
  for (int x = 0; x < input->size; x++) {
    free(input->data[x]);
  }
  free(input->data);
  free(input);
}
