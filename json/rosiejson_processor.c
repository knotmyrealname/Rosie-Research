/**
    @file rosiejson.c
    @author Daniel Dong
    Reads in Rosie outputted json alongside a config file to output more readable data, based on the command line arguments
    Uses the json-c library for parsing
*/

#include <stdlib.h>
#include <stdio.h>
#include <json_object.h>
#include <json_tokener.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "input.h"
#include "utility.h"
#include "output.h"

/** Symbolizes the command line argument -a for (all) */
#define _A 1

/** Symbolizes the command line argument -i for (invalid) */
#define _I 2

/** Symbolizes the command line argument -v for (valid) */
#define _V 3

/** Symbolizes the argument number for the option flag */
#define OPTION 1

/** Symbolizes the argument number for the config file */
#define CONFIG_FILE 2

/** Symbolizes the argument number for the json file */
#define JSON_FILE 3

/** The number of lines printed by program execution, used to print out a helpful message when nothing else
    would otherwise be printed */
int output_printed_count = 0;

/**
    The main method of rosiejson
    @param argc The length of the argument array
    @param argv The array of command line arguments
    @return The exit status of the program
*/
int main(int argc, char *argv[]) {
  // Ensures that there is at least one command line argument
  if (argc <= 1)
    usage();

  // Sets the state of the program, ie what action should be performed
  int state = 0;

  // Checks the option argument and sets the state accordingly
  if (strcmp("-a", argv[OPTION]) == 0) {
    state = _A;
  }
  else if (strcmp("-i", argv[OPTION]) == 0) {
    state = _I;
  }
  else if (strcmp("-v", argv[OPTION]) == 0) {
    state = _V;
  }
  else if (strcmp("-c", argv[OPTION]) == 0) {
    config_msg(EXIT_SUCCESS);
  }
  else {
    // If a valid option was not recognized, a usage message is printed
    usage();
  }

  // Now there has to be enough arguments for there to be a data and config file as data processing starts
  if (argc != JSON_FILE + 1)
    usage();

  // Gets the config file, erroring if it was unable to be read
  FILE *config_fp;
  struct stat st;
  if (stat(argv[CONFIG_FILE], &st) != 0 || (config_fp = fopen(argv[CONFIG_FILE], "r")) == NULL) {
    error("Cannot open config file");
  }

  // Reads the config file into a Config struct
  Config *config = getConfig(config_fp);
  fclose(config_fp);
  if (config == NULL) {
    config_msg(EXIT_FAILURE);
  }

  // Opens the json file, erroring and freeing data if it was unable to be read
  FILE *json_fp;
  if (stat(argv[JSON_FILE], &st) != 0 || (json_fp = fopen(argv[JSON_FILE], "r")) == NULL) {
    free_config_and_data(config);
    error("Cannot open input file");
  }

  // Reads in the json input
  char *json_input = read_continous_input(json_fp);
  fclose(json_fp);

  // Generate the json objects and print out formatted data
  struct json_object *parsed_input = json_tokener_parse(json_input);
  if (parsed_input == NULL) {
    free_config_and_data(config);
    free(json_input);
    error("Unable to parse input for json content");
  }

  // Run different methods based on the previously determined state
  switch (state) {
    case _A:
      // Generates all output and frees memory if it is unsuccessful
      if (!generate_all_output(parsed_input, config->all_output_types,  0)) {
        free_config_and_data(config);
        free(json_input);
        while(json_object_put(parsed_input) != 1) {}
        error("Invalid json data");
      }
      break;
    case _I:
      // Generates invalid output and frees memory if it is unsuccessful
      if (!generate_invalid_output(parsed_input, config->invalid_output_types, config->invalid_subtypes, false)) {
        free_config_and_data(config);
        free(json_input);
        while(json_object_put(parsed_input) != 1) {}
        error("Invalid json data");
      }
      break;
    case _V:
      // Generates valid output and frees memory if it is unsuccessful
      if (!generate_valid_output(parsed_input, config->valid_output_types, config->valid_subtypes)) {
        free_config_and_data(config);
        free(json_input);
        while(json_object_put(parsed_input) != 1) {}
        error("Invalid json data");
      }
      break;
    default:
      // This should not happen if the options were read in correctly
      free_config_and_data(config);
      free(json_input);
      while(json_object_put(parsed_input) != 1) {}
      error("Unexpected Option in Program, should not be reachable, contact developer");
  }

  // If nothing was printed out, let the user know everything worked correctly, just that there was nothing to
  // print because of the config file configuration
  if (output_printed_count == 0) {
    printf("%s%s",
      "Program was successful, but nothing was printed, due to either a lack of data, or because of a ",
      "misconfigured or (partially) empty configuration file\n");
  }

  // Free the memory
  while(json_object_put(parsed_input) != 1) {}
  free(json_input);
  free_config_and_data(config);

  return(EXIT_SUCCESS);
}
