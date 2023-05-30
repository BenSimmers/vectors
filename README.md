# Vectors

## Description
An implementation of a vectors in C to store data.
Kind of like a list, but with a fixed size. So an array :D a cool array.

## Usage
Include the header file in project
```c
#include "vector.h"
```

Run the makefile to compile the library
```bash
make
# not running make will only recompile the modified lines
```
**Note:** The makefile will compile the lib to a `.so` file since it was compiled on a Mac. if on Windows, change the the makefile to compile to a `.dll` file, or just compile it manually 💀 or search google for an answer 🫠.

If you wanna clear all compiled files
```bash
make clean
# remember to recompile the lib 
```

# Python Interopt
Since Python was built off the back of C, made sense to shoot myself in the foot and try to make a Python binding for this library.
```bash
python3 main.py
```
