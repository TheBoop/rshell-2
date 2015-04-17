#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h> //exit
#include <unistd.h> //execvp, fork, gethostname, getlogin
#include <stdio.h> //perror
#include <errno.h> //perror
#include <sys/types.h> //wait
#include <sys/wait.h> //wait
using namespace std;

int main()
{
	string input;	
	while(1)
	{
		char* login = getlogin();
		//char name[64];
		//int len = 64;
		//int hostname = gethostname(); // how to use this??
		cout << login << "@"  << "$ ";
		getline(cin, input);
		if(input == "exit")
		{
			return 0;
		}
		//covert string to char* maybe; yes the method below seems to have worked
		char* in_cstr = new char[input.length()+1];
		//char* in_cstr[input.length()+1]; doesn't work
		strcpy(in_cstr, input.c_str());
		//char in_cstr[100];
		//cin.getline(in_cstr, 100, '\n');

		//int find = 0;
		//int track = 0;
		//find = in_cstr.find("&&");
		//if(find > 0)
		//{
		//	track = 1;
		//}

		//tokenize words
		char delim[] = "  &&";
		char* token;
		token = strtok(in_cstr, delim); // &save_1);
		//cout << "token: " << token << endl;
		//char* file;
		//cout << "c_string: " << token[0] << endl; testing
		//cout << "c_string: " << token[1] << endl; testing
		char** argv = new char*[input.length()+1];
		int i = 0;
		cout << "Tokens of input:" << endl;
		//int x = strlen(token);
		//cout << "x:" << x;
		//for(int k = 0; k < x; k++)
		//{
			//cout << "k:" << k;
			//cout << token[k] << endl;
			//token = strtok(NULL, delim); //causes problems with loop
			//x = strlen(token);
			//k=0;
		//}
		while(token != NULL)
		{
			cout << token << endl;
			argv[i] = token;
			i++;
			token = strtok(NULL, delim);
		}
		argv[i] = NULL; //fixes ls -a ls bug gotta put null char back
		int pid = fork();
		if(pid == -1)
		{
			perror("fork failed");
			exit(1);
		}
		else if(pid > 0)
		{
			cout << "In parent, going to wait" << endl;
			if(wait(0) == -1)
			{
				perror("wait failed");
			}
		}
		else if(pid == 0)
		{
			cout << "In child" << endl;

			if(execvp(argv[0], argv) == -1)
			{
				perror("execvp failed");
			}
			//else
			//{
			//	execvp(argv[0], argv);
			//}
		}
		delete[] in_cstr;
		delete[] argv;
	}
	//currently if ls -a run then ls, ls -a still runs
	//will give me login (e.g. lzhu012)
	//char* login = getlogin();
	//cout << login << endl;
	return 0;
}
