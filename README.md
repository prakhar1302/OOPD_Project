## Shell Implementation README
 
  - This repository contains an implementation of a basic shell in C++, along with scripts and programs related to profiling and multi-threaded  implementations.

## Prerequisites

- C++ compiler (GCC or Clang)
  Any IDE(Vscode , Codeblock)
  linux (Any distribution)


Files Included
# main1.cpp
Implements a basic shell that accepts commands such as cd, mv, rm, ls, and cp.
Supports additional options for each command, including -i for interactive mode and -r for recursive actions.
Utilizes classes and objects, supports wildcard characters, and incorporates file manipulation functionalities.

## main2_1.sh, main2_2.sh, main2_3.sh
Bash scripts to create directories with varying file sizes and quantities.
Profiles the time taken by different file creation scenarios using the time command.

## main3.cpp
A multi-threaded version of the shell implementation for commands involving recursion.
Utilizes C++ threads for recursive file operations like moving, copying, listing, and removing files.

# main4_1.cpp
An enhanced version of the shell with additional functionalities and help options for commands.
Provides an improved user interface with explanations for each command and its options.
Usage

# main4_2.cpp
An enhanced version of the shell with additional functionalities  like mulithreading  and help options for commands.
Provides an improved user interface with explanations for each command and its options.


# Usage

Compile each C++ file using a C++ compiler (g++, for example) and execute the generated binary.
Run the Bash scripts using the bash command to create directories with specified file sizes and quantities.
Refer to each file's comments and the README for specific usage instructions and functionalities.
Instructions
Building and Running

Compile the C++ files: g++ main1.cpp -o main1 (replace main1 with the respective file names)
Execute the generated binary: ./main1
Running Bash Scripts

Execute the Bash scripts: bash main2_1.sh (replace main2_1.sh with the respective script names)
Navigating the Shell

Use standard shell commands like cd, ls, mv, rm, and cp with added options -i and -r.
Refer to the built-in help for command usage: <command> -help.
Multi-threaded Operations

Run the multi-threaded shell implementation to experience improved performance for recursive commands.


## Installation and Usage

1. Clone this repository:

    in base
    go to your directory where you clone the project using the "cd" command
    git clone https://github.com/prakhar1302/OOPD_Project.git
    cd Oppd_Project
    Navigate into the project directory: "Oppd_Project"

2. Compile and run:

   Use your best suitable compiler and run the code.
   Run the code and use it .
   use makefile for debugging and optimization.

## Creators
1. Prakhar Sharma (MT23060)
2. Akash Sharma (MT23012)