#!/bin/bash

# Get the path to the autotester

AUTOTESTER_PATH="../../Code20/cmake-build-debug/src/autotester/autotester"
# Check if the autotester exists
# Loop over all directories
for dir in $(find . -type d); do
    # Check if both files exist in the directory
    if [[ -f "$dir/queries.txt" && -f "$dir/source.txt" ]]; then
        # Navigate to the directory
        cd $dir
        # Run the autotester
        $AUTOTESTER_PATH source.txt queries.txt out.xml
        # Navigate back
        cd -
    fi
done