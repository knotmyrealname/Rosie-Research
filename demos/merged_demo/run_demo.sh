#!/usr/bin/env bash
# For Color Coding to work correctly, the included .rosierc file must be in the home directory
set -o errexit
set -o nounset
set -o pipefail

echo "Generating Demo Files..."

echo "Text file (t), Standard Output (s), JSON (j), or Clean (c):" 
read output_format

echo "Input file (if Clean, just press {Enter}):"
read input_file

colors="end_line=blue:tx_begin_line=red:tx_end_line=blue:data_line=green:invalid_line=yellow:incomplete_tx_begin_line=cyan"

function demorosie (){
  rosie --colors $colors -f demo.rpl grep -w -o $1 $2 $input_file
}

function space_output () {
  echo "Hit {Enter} to continue"
  read entry
}

declare -a demostatus=('Running Demo 1... (A pattern that matches lines)'
                       'Running Demo 2... (A pattern that matches simple transactions)'
                       'Running Demo 3... (A pattern that matches nested transactions)'
                       'Running Demo 4... (A pattern that matches nested, incomplete, and invalid transactions)')

if [[ $output_format == "t" || $output_format == "T" ]]
then
  echo ${demostatus[0]}
  demorosie color line >> output_1.txt
  echo ${demostatus[1]}
  demorosie color simple_tx >> output_2.txt
  echo ${demostatus[2]}
  demorosie color nested_tx >> output_3.txt
  echo ${demostatus[3]}
  demorosie color nested_all_tx >> output_4.txt

elif [[ $output_format == "s" || $output_format == "S" ]]
then
  echo ${demostatus[0]}
  demorosie color line
  space_output
  echo ${demostatus[1]}
  demorosie color simple_tx
  space_output
  echo ${demostatus[2]}
  demorosie color nested_tx
  space_output
  echo ${demostatus[3]}
  demorosie color nested_all_tx

elif [[ $output_format == "j" || $output_format == "J" ]]
then
  echo ${demostatus[0]}
  demorosie jsonpp line >> json_1.txt
  echo ${demostatus[1]}
  demorosie jsonpp simple_tx >> json_2.txt
  echo ${demostatus[2]}
  demorosie jsonpp nested_tx >> json_3.txt
  echo ${demostatus[3]}
  demorosie jsonpp nested_all_tx >> json_4.txt

elif [[ $output_format == "c" || $output_format == "C" ]]
then
  echo "Cleaning up output"
  declare -a files=('output_1.txt' 'output_2.txt' 'output_3.txt' 'output_4.txt'
                    'json_1.txt' 'json_2.txt' 'json_3.txt' 'json_4.txt') 
  for f in "${files[@]}";
  do
    if [ -f "$f" ]
    then
      rm "$f"
    fi
  done

else
echo "Wrong input" >&2
fi

echo "Done"
