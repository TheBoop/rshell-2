all: rshell ls

rshell:
	mkdir -p bin
	g++ -Wall -Werror -ansi -pedantic src/main.cpp -o bin/rshell
ls:
	mkdir -p bin
	g++ -Wall -Werror -ansi -pedantic src/ls.cpp -o bin/ls
