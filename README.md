# CppMakeMake
Makefile generator for C++ small projects, coded at university to learn OOP. It aims to generate Makefiles in a simple and fast way.

## Building
Simply run 
 
``` 
make 
```
This will create a `cppmakemake` executable, compiling cpps with g++.

## Usage

To generate a Makefile, move to the repository that contains _all_ `.cpp` and `.h` files, then run the executable. This will generate a Makefile that will compile your project with g++ compiler, using the `-fno-elide-constructors` option. This Makefile will also clean the repository from `.o` files and will run the application, named as `[name-of-the-file-containing-main]_exe`. Example, assuming you have added `cppmakemake` to you path variable (`sudo cp cppmakemake /bin/`):
```
cd /path/to/repo/containing/project
cppmakemake
make
```
This will run your C++ project.
