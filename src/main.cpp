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
	size_t And = input.find("&");
	And = input.find("&", And+1);
	if(And != string::npos)
	{
		return 2;
	}
	//if(And != string::npos && input.at(And+1) == '&' && input.at(And+2) != '&')
	//{
	//	return 2;
	//}
	size_t Or = input.find('|');
	if(Or != string::npos && input.at(Or+1) == '|' && input.at(Or+2) != '|')
	{
		return 3;
	}
	size_t End = input.find(";");
	if(End != string::npos && input.at(End+1) == ';' && input.at(End+2) != ';')
	{
		return 4;
	}
	else
	{
		return -1;
	}
	//careful this only works for one connector at a time due to how its returned
	//seems to work so far but why chars instead of "" i have no clue
}


int main()
{
	while(1)
	{
		//Intial terminal output with login/hostname
		char* login = getlogin();
		char name[64];
		//int len = 64;
		gethostname(name, sizeof(name)); // how to use this??
		cout << login << "@" << name  << "$ ";
		string input;
		getline(cin, input);

		//dealing with connectors;
		int what = connector(input);
		if(what != -1)
		{
			cout << "connector checking: " << input << endl;
		}
		else
		{
			cout << "no connectors found: " << endl;
		}


		if(input == "exit")
		{
			return 0;
		}
		
		//covert string to char* maybe; yes the method below seems to have worked
		char* in_cstr = new char[input.length()+1];
		//char* in_cstr[input.length()+1]; doesn't work
		strcpy(in_cstr, input.c_str());

		//int find = 0;
		//int track = 0;
		//find = in_cstr.find("&&");
		//if(find > 0)
		//{
		//	track = 1;
		//}

		//tokenize words
		char delim[] = "  &&||;";
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
//bug: if execvp fails, exit has to be ran twice to work for some reason
