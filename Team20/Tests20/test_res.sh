#!/bin/bash

# Parse the input files
source_file=$1

# Check if the file exists
if [ ! -f "$source_file" ]; then
    echo "Error: $source_file does not exist."
    exit 1
fi

# Initialize counters
declare -A succeeded_testcases
declare -A failed_testcases

current_folder=""

# Read the source file line by line
while IFS= read -r line; do
    if [[ $line == *"Parameters"* ]]; then
        read -r line # Skip Parameters : source.txt queries.txt out.xml
    fi

    # Echo the third line of each block of test results and reset counters
    if [[ $line == *"Beginning to parse"* ]]; then
        while IFS= read -r line && ! [[ $line == *"End"* ]]; do
            # Keep reading until we find the comment
            :
        done
        # Now we are at "Beginning to evaluate Query File."
        # Read the next line which we want to echo
        IFS= read -r line
        current_file_name=$(echo "$line" | tr -d '\r')
        echo "Testing folder $current_file_name"
        current_folder=$current_file_name
        succeeded_testcases[$current_folder]=0
        failed_testcases[$current_folder]=0
    fi

    # Skip lines that include "is an Unnamedvar"
    if [[ $line == *"is an Unnamedvar"* ]]; then
        read -r line
    fi

    # Check if the line contains the query evaluation
    if [[ $line == *"- "* ]]; then
        query_num=$(echo "$line" | cut -d'-' -f1 | xargs)
        
        # Skip lines until we reach the answers
        while IFS= read -r line && ! [[ $line == "Your answer:"* ]]; do
            read -r line
        done
        
        # Extract the answers
        your_answer=$(echo "$line" | cut -d':' -f2 | xargs)
        read -r line
        correct_answer=$(echo "$line" | cut -d':' -f2 | xargs)
        
        # Convert answers to arrays
        IFS=' ' read -ra your_answer_array <<< "$your_answer"
        IFS=' ' read -ra correct_answer_array <<< "$correct_answer"
        
        # Compare answers
        if [ "${#your_answer_array[@]}" -eq "${#correct_answer_array[@]}" ]; then
            match=true
            for elem in "${your_answer_array[@]}"; do
                if [[ ! " ${correct_answer_array[*]} " =~ " $elem " ]]; then
                    match=false
                    break
                fi
            done
            
            if [ "$match" = true ]; then
                # Ensure current_folder is not empty
                if [ -n "$current_folder" ]; then
                    ((succeeded_testcases["$current_folder"]++))
                fi
            else
                echo "Testcase failed: $current_file_name, $query_num"
                # Ensure current_folder is not empty
                if [ -n "$current_folder" ]; then
                    ((failed_testcases["$current_folder"]++))
                fi
            fi
        else
            echo "Testcase failed: $current_file_name, $query_num"
            # Ensure current_folder is not empty
            if [ -n "$current_folder" ]; then
                ((failed_testcases["$current_folder"]++))
            fi
        fi
    fi

    # At the end of each folder, output the results
    if [[ $line == *"Completed"* ]]; then
        echo "Folder $current_folder: ${succeeded_testcases[$current_folder]} succeeded, ${failed_testcases[$current_folder]} failed"
        read -r line # Skip Autotester completed
    fi

done < "$source_file"
