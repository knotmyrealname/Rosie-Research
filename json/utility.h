/**
    @file utility.h
    @author Daniel Dong
    The header file for utility.c, contains all of the public function declarations
*/

/**
    Prints out a usage message and exit with error status.
*/
void usage();

/**
    Prints out an error message and exits with error status.
*/
void error(char const *msg);

/**
    Prints the Config file format message
    @param exit_status The exit status for the config message
*/
void config_msg(int exit_status);

/**
    Generates the spacing for the output, based on the depth of the data
    @param depth The depth of the entry
    @return The spacing for the entry
*/
char *spacing(int depth);
