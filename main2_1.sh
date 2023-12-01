#!/bin/bash

# Function to create files in a directory
create_files() {
  local directory=$1
  local file_size=$2
  local num_files=$3

  mkdir -p "$directory"
  
  for ((i = 1; i <= num_files; i++)); do
    dd if=/dev/urandom of="$directory/file$i" bs="$file_size" count=1 status=none
  done
}

# Case 1: 100 files of 50MB each
time create_files "dir1" "128M" 100

clear_space() {
  local directory=$1
  
  # Remove all files in the directory
  rm -f "$directory"/*

  # Remove empty subdirectories
  find "$directory" -type d -empty -delete
}

# Clear space for Case 1
clear_space "dir1"