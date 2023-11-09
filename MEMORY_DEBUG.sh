#!/bin/bash

# Check if Valgrind is installed
if ! command -v valgrind &> /dev/null
then
    echo "Valgrind is not installed. Installing now..."
    sudo apt-get update
    sudo apt-get install valgrind
fi

# Find the compiled file in the current directory
executable_file=$(find . -maxdepth 1 -type f -executable -exec file {} \; | grep "ELF" | awk -F: '{print $1}')

# Check if an executable file was found
if [ -n "$executable_file" ]
then
    echo "Executable file found: $executable_file"
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes "$executable_file"
else
    echo "No executable file found in the current directory."
fi
