echo "Generating Demo Files..."

echo "Text file (t), Standard Output (s), or Clean (c):"
read output_format


if [[ $output_format == "t" || $output_format == "T" ]]
then
echo "Running Demo 1... (A pattern that matches lines)"
rosie --colors "tx_begin_line=red:tx_end_line=blue:data_line=green" -f demo_1.rpl grep -w -o color 'findall: line' input-1.txt >> demo_1_output.txt
echo "Running Demo 2... (A pattern that matches simple transactions)"
rosie --colors "tx_begin_line=red:tx_end_line=blue:data_line=green" -f demo_2.rpl grep -w -o color simple_tx input-1.txt >> demo_2_output.txt
echo "Running Demo 3... (A pattern that matches nested transactions)"
rosie --colors "tx_begin_line=red:tx_end_line=blue:data_line=green" -f demo_3.rpl grep -w -o color nested_tx input-1.txt >> demo_3_output.txt
echo "Running Demo 4... (A pattern that matches nested transactions)"


elif [[ $output_format == "s" || $output_format == "S" ]]
then
echo "Running Demo 1... (A pattern that matches lines)"
rosie --colors "tx_begin_line=red:tx_end_line=blue:data_line=green" -f demo_1.rpl grep -w -o color 'findall: line' input-1.txt
echo "Hit {Enter} to continue"
read entry
echo "Running Demo 2... (A pattern that matches simple transactions)"
rosie --colors "tx_begin_line=red:tx_end_line=blue:data_line=green" -f demo_2.rpl grep -w -o color simple_tx input-1.txt
echo "Hit {Enter} to continue"
read entry
echo "Running Demo 3... (A pattern that matches nested transactions)"
rosie --colors "tx_begin_line=red:tx_end_line=blue:data_line=green" -f demo_3.rpl grep -w -o color nested_tx input-1.txt

elif [[ $output_format == "c" || $output_format == "C" ]]
then
echo "Cleaning up output"
rm demo_1_output.txt demo_2_output.txt demo_3_output.txt

else
echo "Wrong input"
fi

echo "Done"
