# User Guide for rosiejson_processor
The purpose of rosiejson_processor is to demonstrate methods of which users can utilize rosie json to process data, and to provide a visualization of the json structure for debugging. This can aid in further research into the applications of rosie, especially into the exploration of it's uses to aid in data processing, such as transaction distinction.

## Building in Unix with `gcc`
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

## Running the program
There are several modes in which you can run rosiejson_processor:

`./rosiejson_processor -a <config-file> <json-file>`
Outputs all 

`./rosiejson_processor -i <config-file> <json-file>`


`./rosiejson_processor -v <config-file> <json-file>`


`./rosiejson_processor -c`
outputs 
