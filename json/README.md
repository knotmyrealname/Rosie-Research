# User Guide for rosiejson_processor
The purpose of rosiejson_processor is to demonstrate methods of which users can utilize rosie json to process data, and to provide a visualization of the json structure for debugging. This can aid in further research into the applications of rosie, especially into the exploration of it's uses to aid in data processing, such as transaction distinction.

## Building in Debian with `gcc`
### Prerequisites
To begin, you will need to clone the Rosie-Research repository (in the future, a seperate repository may be created specifically for rosiejson_processor). This repository can be cloned by running `git clone https://github.com/knotmyrealname/Rosie-Research.git` in the terminal. 

Before you can compile the program, you need to know that rosiejson_processor utilizes the `json_c` library, found at https://github.com/json-c/json-c. This library must be installed in order for rosiejson_processor to compile and run. Installation instructions can be found at the json-c github page.

Additionally, rosiejson_processor utilizes `gcc` (c99) for compilation. If `gcc` is not installed on your system, you can install it by running  
```
sudo apt update
sudo apt install build-essential
```
### Compilation
To compile rosiejson_processor, you will first need to go into the json folder in the Rosie-Research library. Afterwards, to compile you will need to run `make` in the terminal. This should produce an executable rosiejson_processor, after which, compilation should be finished. 

In order to remove compilation output, you can run `make clean` to get rid of all output and executable files.
### Building on non-Debian systems
Support for non-Debian systems has not been tested, however, there is interest in potentially testing on other Operating Systems. There are no guarantees on functionality, but as rosiejson_processor is written mostly with standard C99 functions, it should hopefully work on most other systems, given that the json-c library was downloaded properly.

## Running the program
There are several modes in which you can run rosiejson_processor:

`./rosiejson_processor -a <config-file> <json-file>`
Outputs all of the json data types, showing the depth of each item, as well as the start and end values. This references the `all:` config type, appending colored data values to the end of every matching type.

`./rosiejson_processor -i <config-file> <json-file>`
Outputs all of the matches deemed invalid, for either further processing or debugging. Invalid matches are determined by the `invalid:` config type, with `invalid_sub:` determining which submatches of the invalid match should also be output. 

`./rosiejson_processor -v <config-file> <json-file>`
Outputs all of the matches deemed valid, with a newline to separate different whole valid matches. Valid matches are determined by the `valid:` config type, with `valid_sub:` determining which submatches of a valid match should be part of the valid match. Config match types defined in `valid:` should not be redefined in `valid_sub:` as items identified as `valid_sub:` are not recursed through. 

`./rosiejson_processor -c`
Outputs config file information and instructions

### Config File
The config file provides for the easy defining of output for the rosiejson_processor program. A proper config file should have the bare minimum of 
```
all:
invalid:
invalid_sub:
valid:
valid_sub:
```
Although this is valid, it would lead to no output from finding invalid or valid matches, as nothing was specified to be output. 
Both spaces and newlines are considered valid delimitors between values, meaning that
```
all: <data-type> invalid: <data-type> invalid_sub: <data-type> <data-type> valid: <data-type>   valid_sub:<data-type>
```
and 
```
all: <data-type>
<data-type>
<data-type>

invalid: <data-type> invalid_sub: <data-type>
<data-type> valid: <data-type> valid_sub: <data-type>
```
are all valid config file formats.
## Issues
Although rosiejson_processor was thoroughly tested for errors and memory leaks, it is still possible that some slipped through preliminary testing. If any errors are discovered, an issue report can be posted to https://github.com/knotmyrealname/Rosie-Research/issues, and it will be investigated. 

Additionally, if you have an idea of what can be done to improve the functionality or usability of rosiejson_processor, such ideas can also be posted to the issues tab in Github. 
