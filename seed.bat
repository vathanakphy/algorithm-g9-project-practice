@echo off
REM Creates the 'out' directory if it doesn't exist.
if not exist out mkdir out

echo Compiling files for seeder...

REM --- Compile Dependencies ---
REM These object files are needed by the seeder.
gcc -c ./db/sqlite3.c -o ./out/sqlite3.o
g++ -c ./utils/db_utils.cpp -o ./out/db_utils.o

REM --- Compile Seeder ---
g++ -c seed.cpp -o ./out/seed.o

echo Linking seeder executable...
REM --- Link Seeder ---
g++ ./out/seed.o ./out/db_utils.o ./out/sqlite3.o -o ./out/seed.exe

echo Running database seeder...
REM --- Run Seeder ---
out\seed.exe

echo.
echo Seeding complete. The database is ready.
exit
pause
