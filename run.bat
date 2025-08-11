@echo off
REM Creates the 'out' directory if it doesn't exist.
if not exist out mkdir out

echo Compiling files for main program...

REM --- Compile Dependencies ---
REM These object files are needed by the main program.
gcc -c ./db/sqlite3.c -o ./out/sqlite3.o
g++ -c ./utils/db_utils.cpp -o ./out/db_utils.o

REM --- Compile Main Program ---
g++ -c main.cpp -o ./out/main.o

echo Linking main executable...
REM --- Link Main Program ---
g++ ./out/main.o ./out/db_utils.o ./out/sqlite3.o -o ./out/my_program.exe

echo Running main program...
REM --- Run Main Program ---
out\my_program.exe

pause
