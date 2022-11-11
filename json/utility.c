/**
    @file usage.c
    @author Daniel Dong
    Contains utility functions for rosiejson
*/

#include <stdlib.h>
#include <stdio.h>

/**
    Prints out a usage message and exit with error status.
*/
void usage() {
  printf("%s%s%s%s%s%s",
         "Usage:  rosiejson <option> <config-file> <json-file>\n",
         "  Valid <option> values are one of these:\n",
         "    -a  Prints out all Json Objects\n",
         "    -i  Prints out all Invalid/Incomplete Json Objects\n",
         "    -v  Prints out all Valid Json Objects\n",
         "  Use -c to see valid <config-file> formats\n");
  exit( EXIT_FAILURE );
}

/**
    Prints out an error message and exits with error status.
*/
void error(char const *msg) {
  fprintf(stderr, "%s\n", msg);
  exit(EXIT_FAILURE);
}

/**
    Prints the Config file format message
*/
void config_msg() {
  printf("%s%s%s%s",
         "The configuration file expects a list of submatch types that should output data as a list of",
         "whitespace-separated strings. Ex:\n",
         "  submatch_type1 submatch_type2 submatch_type3\n",
         "  submatch_type4        submatch_type_5\n");
}

/**
    Generates the spacing for the output, based on the depth of the data
    @param depth The depth of the entry
    @return The spacing for the entry
*/
char *spacing(int depth) {
  int spaces_count = depth * 2;
  char *spaces = (char *) malloc((spaces_count + 1) * sizeof(char));
  for (int x = 0; x < depth; x++) {
    spaces[x * 2] = '|';
    spaces[x * 2 + 1] = ' ';
  }
  spaces[spaces_count] = '\0';
  return spaces;
}
