/**
    @file output.h
    @author Daniel Dong
    Contains output functions for all, invalid, and valid inputs
*/

#include <stdio.h>
#include <json_object.h>
#include <json_tokener.h>

/** The number of times output related to the config file was printed */
extern int output_printed_count;

/**
    Checks the json object type, and if it matches an object in output_types, the data inside the data object is pri>
    @param data The json object containing the data to potentially print
    @param type The json object containing the type of the data
    @param output_types The list of types for which data should be output
    @paarm types_count The number of types in the output_types array
*/
bool contains(StringArray *types, struct json_object *type);

/**
    Recursively generates output based on the json that was parsed into a more readable format.
    Theoretically, this should work with any rosie json output.
    @param parsed_input The json input to parse
    @param depth The depth of the input being analyzed
    @return A value based on the success of the function; true when successful and false when unsuccessful
*/
bool generate_all_output(struct json_object *parsed_input, StringArray *output_types, int depth);

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
                             StringArray *subtypes, bool in_invalid_tx);

/**
    Recursively generates output based on the json that was parsed, only printing out segments that are
    part of an valid transaction.
    This may have the potential to be updated to be more flexible with other rosie patterns, but that will
    have to come in a future update.
    @param parsed_input The json input to parse
    @param in_invalid_tx Whether the parent was an invalid transaction
    @return A value based on the success of the function; true when successful and false when unsuccessful
*/
bool generate_valid_output(struct json_object *parsed_input, StringArray *valid_types, StringArray *subtypes);
