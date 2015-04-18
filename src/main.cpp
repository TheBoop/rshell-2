#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <stdlib.h> //exit
#include <unistd.h> //execvp, fork, gethostname, getlogin
#include <stdio.h> //perror
#include <errno.h> //perror
#include <sys/types.h> //wait
#include <sys/wait.h> //wait
using namespace std;

void comment(string& input)
{
	size_t com = input.find("#");
	if(com != string::npos)
	{
		input.erase(com);
	}
}


int main()
{
	while(1)
	{
		//Intial terminal output with login/hostname
		string login;
		if(getlogin() == NULL)
		{
			perror("getlogin() failed");
		}
		else
		{
			login = getlogin();
		}
		char name[64];
		if(gethostname(name, sizeof(name)) == -1)
		{
			perror("gethostname() failed");
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

		//Remove comments
		comment(input);

		//covert string to char*
		char* in_cstr = new char[input.length()+1];
		strcpy(in_cstr, input.c_str());

		//Tracking connectors
		int and_cnt = 0;
		int or_cnt = 0;
		for(unsigned k = 0; k < strlen(in_cstr); k++)
		{
			if(in_cstr[k] == '&' && k+1 < strlen(in_cstr) && in_cstr[k+1] == '&')
			{
				and_cnt = 1;
			}
			if(in_cstr[k] == '|' && k+1 < strlen(in_cstr) && in_cstr[k+1] == '|')
			{
				or_cnt = 1;
			}
		}

		//tokenize words
		char delim[] = "&|;";
		char* token;
		char* token2;
		char* save_1;
		char* save_2;
		token = strtok_r(in_cstr, delim, &save_1);
		//cout << "token: " << token << endl;
		//char* file;
		//cout << "c_string: " << token[0] << endl; testing
		//cout << "c_string: " << token[1] << endl; testing
		//
		//char** argv = new char*[input.length()+1];
		//int i = 0;
		//cout << "Tokens of input:" << endl;
		//
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
			char* in_str2 = new char[strlen(token)+1];
			in_str2 = token;
			token2 = strtok_r(in_str2, " ", &save_2);
			vector<string> temp;
			while(token2 != NULL)
			{
				temp.push_back(token2);
				token2 = strtok_r(NULL, " ", &save_2);
			}

			char **argv = new char*[temp.size()+1];
			for(unsigned i = 0; i < temp.size(); i++)
			{
				if(temp.at(i) == "exit")
				{
					exit(1);
				}
				argv[i] = new char[temp.at(i).size()+1];
				strcpy(argv[i], temp.at(i).c_str());
				argv1[temp.size()] = 0;
			}

			int pid = fork();
			int status = 0;
			if(pid == -1)
			{
				perror("fork failed");
				exit(1);
			}
			else if(pid == 0)
			{
				if(execvp(argv[0], argv) != -1 && or_cnt == 1)
				{
					status = 10;
					exit(1);
				}
				else if(execvp(argv[0], argv) == -1)
				{
					perror("execvp failed");
					if(and_cnt == 1)
					{
						status = 10;
						exit(1);
					}
					else if(or_cnt == 1)
					{
						cout << "or failed" << endl;
						status = 1;
						cout << status << endl;
						exit(1);
					}
					exit(1);
				}
			}
			else
			{
				if(wait(&status) == -1)
				{
					perror("wait failed");
				}
				else
				{
					if(status > 0 && and_cnt == 1)
					{
						break;
					}
					if(WEXITSTATUS(status) == 0 && or_cnt == 1)
					{
						cout << status << " or worked " << endl;
						break;
					}
					else
					{
						token = strtok_r(NULL, delim, &save_1);
					}
				}
			}
		}
		//delete[] in_cstr;
		//delete[] in_str2;
	}
}
//nohlsearch
//bug: if execvp fails, exit has to be ran twice to work for some reason
//bug: &&& doesnt give error
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
