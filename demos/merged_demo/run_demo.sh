#!/usr/bin/env bash
set -o errexit
set -o nounset
set -o pipefail

echo "Generating Demo Files..."

echo "Text file (t), Standard Output (s), JSON (j), or Clean (c):" 
read output_format

function demorosie (){
  rosie -f demo.rpl grep -w -o $1 $2 $input_file
}

if [[ $output_format == "t" || $output_format == "T" ]]
then
  echo "Input file:"
  read input_file
  echo "Running Demo 1... (A pattern that matches lines)"
  demorosie color line >> output_1.txt
  echo "Running Demo 2... (A pattern that matches simple transactions)"
  demorosie color simple_tx >> output_2.txt
  echo "Running Demo 3... (A pattern that matches nested transactions)"
  demorosie color nested_tx >> output_3.txt
  echo "Running Demo 4... (A pattern that matches nested, incomplete, and invalid transactions)"
  demorosie color nested_all_tx >> output_4.txt

elif [[ $output_format == "s" || $output_format == "S" ]]
then
  echo "Input file:"
  read input_file
  echo "Running Demo 1... (A pattern that matches lines)"
  demorosie color line
  echo "Hit {Enter} to continue"
  read entry
  echo "Running Demo 2... (A pattern that matches simple transactions)"
  demorosie color simple_tx
  echo "Hit {Enter} to continue"
  read entry
  echo "Running Demo 3... (A pattern that matches nested transactions)"
  demorosie color nested_tx
  echo "Hit {Enter} to continue"
  read entry
  echo "Running Demo 4... (A pattern that matches nested, incomplete, and invalid transactions)"
  demorosie color nested_all_tx

elif [[ $output_format == "j" || $output_format == "J" ]]
then
  echo "Input file:"
  read input_file
  echo "Running Demo 1... (A pattern that matches lines)"
  demorosie jsonpp line >> json_1.txt
  echo "Running Demo 2... (A pattern that matches simple transactions)"
  demorosie jsonpp simple_tx >> json_2.txt
  echo "Running Demo 3... (A pattern that matches nested transactions)"
  demorosie jsonpp nested_tx >> json_3.txt
  echo "Running Demo 4... (A pattern that matches nested, incomplete, and invalid transactions)"
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
