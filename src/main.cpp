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
		
		while(token != NULL)
		{
			//cout << "Top of lopp" << endl;
			//cout << "toke: " << token << endl;
			char* in_str2 = new char[strlen(token)+1];
			in_str2 = token;
			token2 = strtok_r(in_str2, " ", &save_2);
			//cout << "in_str2: " << in_str2 << endl;
			//cout << "token2: " << token2 << endl;

			vector<string> temp;
			while(token2 != NULL)
			{
				temp.push_back(token2);
				token2 = strtok_r(NULL, " ", &save_2);
			}
			//cout << "contents of temp vector: ";
			//for(unsigned t = 0; t < temp.size(); t++)
			//{
			//	cout << temp[t] << " ";
			//}
			//cout << endl;

			char **argv = new char*[temp.size()+1];
			for(unsigned i = 0; i < temp.size(); i++)
			{
				if(temp.at(i) == "exit")
				{
					exit(1);
				}
				argv[i] = new char[temp.at(i).size()+1];
				strcpy(argv[i], temp.at(i).c_str());
				argv[temp.size()] = NULL;
			}

			//cout << "contents of argv: ";
			//for(unsigned k = 0; k < temp.size(); k++)
			//{
			//	cout << argv[k] << " ";
			//}
			//cout << endl;

			int pid = fork();
			int status = 0;
			if(pid == -1)
			{
				perror("fork failed");
				exit(1);
			}
			else if(pid == 0)
			{
				//cout << "in child" << endl;
				if(execvp(argv[0], argv) == -1)
				{
					perror("execvp failed");
					if(or_cnt == 1)
					{
						status = 5;
						exit(1);
					}
					if(and_cnt == 1)
					{
						status = 5;
						exit(1);
					}
				}
				else
				{
					if(or_cnt == 1)
					{
						status = 5;
						break;
					}
				}
			}
			else
			{
				//cout << "in parent" << endl;
				if(wait(&status) == -1)
				{
					perror("wait failed");
				}
				else
				{
					if(WEXITSTATUS(status) != 0 && and_cnt == 1)
					{
						break;
					}
					if(WEXITSTATUS(status) == 0 && or_cnt == 1)
					{
						break;
					}
					else
					{
						//if wait is sucessful, and no and is found no or is found
						token = strtok_r(NULL, delim, &save_1);
					}
				}
			}
		}
		delete[] in_cstr;
		//delete[] in_str2;
	}
	return 0;
}
//nohlsearch
//bug: if execvp fails, exit has to be ran twice to work for some reason
//bug: &&& doesnt give error
