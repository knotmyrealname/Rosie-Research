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
    @param exit_status The exit status for the config message
*/
void config_msg(int exit_status) {
  printf("%s%s%s%s%s%s%s%s",
         "The configuration file expects a list of match types, each with submatch types delimited by whitespace-separated ",
         "strings. Extraneous whitespace (spaces and new-lines) is ignored. An example of a valid config file is:\n\n",
         "  all:\n  end_line\n  invalid_line\n  data_line\n  tx_begin_line\n  tx_end_line\n  incomplete_tx_begin_line\n\n",
         "  invalid:\n  incomplete_tx_begin_line invalid_line\n\n",
         "  invalid_sub: data_line invalid_line\n",
         "  valid: nested_all_tx.complete_tx valid_sub: data_line tx_begin_line tx_end_line\n\n",
         "- Note that the data within cannot have the same name as any of the config-type labels (e.g. 'all:')\n",
         "- It is ill-advised to have duplicate data-labels between valid: and valid_sub: due to the nature of -v output\n");
  exit(exit_status);
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
