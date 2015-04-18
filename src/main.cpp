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

int connector(string& input)
{
	size_t com = input.find("#");
	if(com != string::npos)
	{
		input.erase(com);
		return 1;
	}
}


int main()
{
	while(1)
	{
		//Intial terminal output with login/hostname
		if(getlogin() == NULL)
		{
			perror("getlogin() failed");
		}
		else
		{
			string login = getlogin();
		}
		char name[64];
		if(gethostname(name, sizeof(name)) == -1)
		{
			perror("gethostname() failed")
		}
		else if(getlogin() != NULL)
		{
			cout << login << "@" << name  << "$ ";
		}

		//Getting input
		string input;
		getline(cin, input);
		
		//This wont work if its with connectors and other stuff
		if(input == "exit")
		{
			return 0;
		}
		
		//covert string to char* maybe; yes the method below seems to have worked
		char* in_cstr = new char[input.length()+1];
		strcpy(in_cstr, input.c_str());

		//tokenize words
		char delim[] = "  &|;";
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

		//Calling execvp
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
	return 0;
}
//nohlsearch
//bug: if execvp fails, exit has to be ran twice to work for some reason
//IDEA
//STEP 1:
//LETS SAY WE HAVE     ls && ls
//Tokenize with delimintors && ; ||
//THEN WE HAVE TOKENS
//     ls 
// ls
// STORE the first in one argv1
// then second in another argv2
// recurssviely do this until no delims are left
// then tokenize AGAIN with " "
// so for first argv1 we have ls
// the second also ls
// then we execvp firs the first argv
// then we call execvp vp on the second but this time dependent on the first execvp
// do this until all argvs are passed through
// how to code this i have no idea, will it work i have no idea
//
// char* token;
// token = strtok(in_cstr, "&")
// token[0] = '    ls '
// token[1] = ' ls'
// each one of these tokens will become its own cstring
// token1 = strtok(token, " ")
// if(execvp(token1) == -1)
// 	perrror("failed")
// 	exit
// else(execvp(token2) == -1)
// recurrively call (token2, token1)
// 
