#!/bin/bash
set -e

# Get the absolute path to the directory containing the script. Fixed to Team20
SCRIPT_DIR=$(cd "$(dirname "\$0")" && pwd)

# Base directory where to start searching for autotester.exe
AUTOTESTER_BASE_DIR="$SCRIPT_DIR/Team20/Code20/"

# Set the default mode to DEBUG
MODE="Debug"

file_ext=""
# Parse the command-line arguments

# -R: Release or Debug mode
# -W: Windows mode
# -M: Mac mode
# FIXME: both -M and -W cannot be used together
while getopts "RW:M" opt; do
    case ${opt} in
        R )
            MODE="Release"
            ;;
        W )
            OS="Windows"
            file_ext=".exe"
            ;;
        M )
            OS="Mac"
            ;;
        \? )
            echo "Usage: ./autotester.sh [-R for Release mode, -W for Windows, -M for Mac, no flag for Debug mode]"
            exit 1
            ;;
    esac
done

if [ $OPTIND -eq 1 ]; then
    echo "No input provided. Please specify an option."
    echo "Usage: ./autotester.sh [-R for Release mode, -W for Windows, -M for Mac, no flag for Debug mode]"
    exit 1
fi

# Find the autotester executable within the base directory
# FIXME: add release or debug mode
AUTOTESTER_PATH=$(find "$AUTOTESTER_BASE_DIR" -name autotester$file_ext -type f | head -n 1)

# Check if the autotester executable was found
if [[ -z "$AUTOTESTER_PATH" ]]; then
    echo "Error: autotester executable not found in $AUTOTESTER_BASE_DIR"
    exit 1
fi

# Base directory
base_dir="$(pwd)/Team20/Tests20"

# Find all directories within Tests20 and loop over them
find "$base_dir" -mindepth 1 -type d | while IFS= read -r dir; do
    # Construct the absolute path to the files
    queries_file="$dir/queries.txt"
    source_file="$dir/source.txt"
    
    # Check if both files exist in the directory
    if [[ -f "$queries_file" && -f "$source_file" ]]; then
        # Navigate to the directory
        if cd "$dir"; then
            # Run the autotester using the absolute paths to the files
            if ! "$AUTOTESTER_PATH" "$source_file" "$queries_file" out.xml; then
                echo "Autotester failed in directory $dir"
            fi
            # Navigate back to the previous directory
            cd - > /dev/null || echo "Warning: Failed to return to previous directory from $dir"
        else
            echo "Failed to enter directory $dir"
        fi
    else
        echo "The required files are not found in $dir."
    fi
done
