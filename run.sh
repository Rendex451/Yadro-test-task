#!/bin/sh

cd bin

writer_output=$(./writer)
echo "$writer_output"

key=$(echo "$writer_output" | grep -oP 'Shared Memory Key: \K\d+')
./reader "$key"