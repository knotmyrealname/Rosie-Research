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

/** The minimum word count for a config file */
#define MIN_CONFIG_SIZE 5

/**
    A struct containing an array of char pointers and a size value coordinating to the number of char pointers
*/
typedef struct {
  /** The array of character pointers, storing the data */
  char **data;
  /** The size of the data array, ie how many char pointers are being stored */
  int size;
} StringArray;

/**
    A struct containing the essential structure of a config file
*/
typedef struct {
  /** The array of (all) output types for the program - used by generate_all_output*/
  StringArray *all_output_types;
  /** The array of invalid output types for the program - used by generate_invalid_output*/
  StringArray *invalid_output_types;
  /** The array of invalid subtypes for the program - used by generate_invalid_output*/
  StringArray *invalid_subtypes;
  /** The array of valid output types for the program - used by generate_valid_output */
  StringArray *valid_output_types;
  /** The array of valid subtypes for the program - used by generate_valid_output */
  StringArray *valid_subtypes;
} Config;

/**
    Reads input from a file pointer and returns it as a (potentially very long) string
    The user can free the returned memory by simply calling free() on the object.
    @param fp The file pointer to read from
    @return A dynamically allocatedstring containing all the data inside the file pointer
*/
char *read_continous_input(FILE *fp);

/**
    Reads input from a file pointer and returns it as a StringArray, with a '\0'. The input is deliminated by
    the specified delimiter, as well as a default '\n'.
    The user can free the returned memory by calling the provided free_delimited_input function on the object.
    @param fp The file pointer to read from
    @param delimiter The character to deliminate input by.
    @return A StringArray struct containing an array of char pointers (to strings) and the number of strings read
*/
StringArray *read_delimited_input(FILE *fp, char delimiter);

/**
    Gets input and, if it's the correct format, parses it as a Config struct
    @param fp The file pointer to read from
    @return A config struct containing StringArrays of every config subtype
*/
Config *getConfig(FILE *fp);

/**
    Helper funtion to free the memory stored in Config. Does not free the data, only the struct.
    @param config The Config struct to free
*/
void free_only_config(Config *config);

/**
    Helper funtion to free the memory stored in Config. Frees both the data and struct.
    @param config The Config struct to free
*/
void free_config_and_data(Config *config);

/**
    Helper function to free the array of character pointers returned by read_delimited_input.
    @param input The memory to free
*/
void free_string_array(StringArray *input);
