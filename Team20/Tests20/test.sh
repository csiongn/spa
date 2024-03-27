#!/bin/bash

shopt -s expand_aliases  # Enable alias expansion

source ~/.bash_aliases

# Find autotester in Code20 folder
AUTOTESTER_PATH=$(find ../Code20 -name autotester.exe -type f | head -n 1)

echo $AUTOTESTER_PATH

if [[ -f "../res.txt" ]]; then
    # File exists, delete it
    rm "../res.txt"
    echo "Existing res.txt found, removing it."
fi

# Iterate through each directory in the current directory
for dir in */; do
    dir=${dir%*/}  # Remove trailing slash
    echo "Processing directory: $dir"
    
    # Change into the directory
    cd "$dir" || { echo "Failed to change directory to $dir"; exit 1; }

    # Run the command, pipe to res.txt in the Test20 folder
    st3 source.txt queries.txt out.xml >> ../res.txt

    # Change back to the original directory
    cd - > /dev/null
done

echo "Results stored in res.txt"