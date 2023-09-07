#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <input_filename> <output_filename>"
    exit 1
fi

./main "$1" "$2"
