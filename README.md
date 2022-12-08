# Rosie-Research
This repository is a collection of notes, code, and data that was compiled through research into the Rosie Pattern Language (RPL). 
## demos
A collection of demos to demonstrate the functionality of the RPL. There are two demo types, the discrete demo, and the merged demo.

The discrete demo folder contains separate demo files for each of the 4 cases that were considered for transaction processing, simple line matching, simple transaction matching, nested transaction matching, and nested and invalid transaction matching. To run the demo, there is a bash script of which can be run through the command line, providing input instructions and output, either in the form of command line output or text file output, based on the specified output format

The merged demo folder contains a unified demo file for all of the cases developed for the discrete demo, in order to demonstrate the composability of the RPL. Support was added in the provided bash script for specifying an input file and json output. The demo script was also streamlined, and script failure states were considered and accounted for.

## json
A C program to process rosie json output, and return more processed, more human readable output. It's usage and purpose is explained further in the README.md file inside the json folder

## notes
Some of the notes that were taken during the meetings, discussing future steps and ideas. Each file is timestamped with the meeting date in the format `notes-MM-DD-YY.txt`

## original_testing
The first attempt to parse transactions using RPL. Contains the original rpl code, input data, and output data, including a log of command line inputs.

## issues.txt
This is a collection of issues that were documented about the usability and functionality of RPL, as part of the research. 
