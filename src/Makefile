CC=g++
OPT=
NAME=main

MAIN_PROCEDURE_EXE : parte main makemaker 
	$(CC) $(OPT) -o cppmakemake parte.o main.o makemaker.o 
	rm *.o

parte : parte.cpp parte.h 
	$(CC) $(OPT) -c parte.cpp -o parte.o

main : main.cpp makemaker.h 
	$(CC) $(OPT) -c main.cpp -o main.o

makemaker : makemaker.cpp parte.h makemaker.h 
	$(CC) $(OPT) -c makemaker.cpp -o makemaker.o

