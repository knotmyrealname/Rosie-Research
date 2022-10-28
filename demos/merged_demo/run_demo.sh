echo "Generating Demo Files..."

echo "Text file (t), Standard Output (s), or Clean (c):" 
read output_format


if [[ $output_format == "t" || $output_format == "T" ]]
then
  echo "Running Demo 1... (A pattern that matches lines)"
  rosie --colors "tx_begin_line=red:end_line=blue:data_line=green" -f demo.rpl grep -w -o color line input-1.txt >> output_1.txt
  echo "Running Demo 2... (A pattern that matches simple transactions)"
  rosie --colors "tx_begin_line=red:tx_end_line=blue:data_line=green" -f demo.rpl grep -w -o color simple_tx input-1.txt >> output_2.txt
  echo "Running Demo 3... (A pattern that matches nested transactions)"
  rosie --colors "tx_begin_line=red:tx_end_line=blue:data_line=green" -f demo.rpl grep -w -o color nested_tx input-1.txt >> output_3.txt
  echo "Running Demo 4... (A pattern that matches nested, incomplete, and invalid transactions)"
  rosie --colors "tx_begin_line=red:tx_end_line=blue:data_line=green:invalid_line=yellow:incomplete_tx_begin_line=cyan" -f demo.rpl grep -w -o color nested_all_tx input-1.txt >> output_4.txt

elif [[ $output_format == "s" || $output_format == "S" ]]
then
  echo "Running Demo 1... (A pattern that matches lines)"
  rosie --colors "tx_begin_line=red:end_line=blue:data_line=green" -f demo.rpl grep -w -o color line input-1.txt
  echo "Hit {Enter} to continue"
  read entry
  echo "Running Demo 2... (A pattern that matches simple transactions)"
  rosie --colors "tx_begin_line=red:tx_end_line=blue:data_line=green" -f demo.rpl grep -w -o color simple_tx input-1.txt
  echo "Hit {Enter} to continue"
  read entry
  echo "Running Demo 3... (A pattern that matches nested transactions)"
  rosie --colors "tx_begin_line=red:tx_end_line=blue:data_line=green" -f demo.rpl grep -w -o color nested_tx input-1.txt
  echo "Hit {Enter} to continue"
  read entry
  echo "Running Demo 4... (A pattern that matches nested, incomplete, and invalid transactions)"
  rosie --colors "tx_begin_line=red:tx_end_line=blue:data_line=green:invalid_line=yellow:incomplete_tx_begin_line=cyan" -f demo.rpl grep -w -o color nested_all_tx input-1.txt

elif [[ $output_format == "c" || $output_format == "C" ]]
then
  echo "Cleaning up output"
  declare -a files=('output_1.txt' 'output_2.txt' 'output_3.txt' 'output_4.txt') 
  for f in "${files[@]}";
  do
    if [ -f "$f" ]
    then
      rm "$f"
    fi
  done

else
echo "Wrong input"
fi

echo "Done"
