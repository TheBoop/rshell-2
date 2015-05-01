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

void del_incstr(char* in_cstr)
{
	//cout << "deleting in_cstr" << endl;
	delete[] in_cstr;
}

void del_argv(char** argv, int argv_sz)
{
	//cout << "deleting argv " << endl;
	for(int j = 0; j < argv_sz+1; j++)
	{
		delete[] argv[j];
	}
	delete [] argv;
	//cout << endl;
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
		//if(input == "exit")
		//{
		//	return 0;
		//}

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
		// dont need to track ; since anything after a connector is treated as new input

		//tokenize words
		char delim[] = "&|;";
		char* token;
		char* token2;
		char* save_1;
		char* save_2;

		//Gets the first command in the string
		//e.g. Input = ls -a && ls
		//token = ls -a;
		token = strtok_r(in_cstr, delim, &save_1);
		
		while(token != NULL)
		{
			//cout << "Top of lopp" << endl;
			//cout << "token: " << token << endl;
			//Take out any whitespace
			//e.g. token2 = ls
			token2 = strtok_r(token, " ", &save_2);
			//cout << "token2: " << token2 << endl;
			
			//Puts token2 into a vector for temporary storage
			//Gets the rests of token if there are any
			//e.g. token2 = -a
			//temp[0] = ls
			//temp[1] = -a
			vector<string> temp;
			while(token2 != NULL)
			{
				temp.push_back(token2);
				token2 = strtok_r(NULL, " ", &save_2);
			}
			//cout << "contents of temp vector: " << temp.size() << " ";
			//for(unsigned t = 0; t < temp.size(); t++)
			//{
			//	cout << temp.at(t) << " ";
			//}
			//cout << endl;

			//Puts everything from storage into a char**
			//Since execvp takes char** as a parameter
			//Also checks if input was exit, if so we need to exit prematurely
			//e.g. argv[0] = ls
			//argv[1] = -a
			//argv[2] = \0
			char** argv = new char*[temp.size()+1];
			for(unsigned i = 0; i < temp.size(); i++)
			{
				if(temp.at(i) == "exit")
				{
					delete[] argv;
					del_incstr(in_cstr);
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
				int sz = temp.size();
				del_argv(argv, sz);
				del_incstr(in_cstr);
				exit(1);
			}
			else if(pid == 0)
			{
				//cout << "in child" << endl;
				//if argv[0] was some invalid input
				if(execvp(argv[0], argv) == -1)
				{
					perror("execvp failed");
					//takes care of the false || true case
					if(or_cnt == 1)
					{
						status = 3;
						int sz = temp.size();
						del_argv(argv, sz);
						del_incstr(in_cstr);
						_exit(1);
					}
					//takes care of the false && true case
					if(and_cnt == 1)
					{
						status = 3;
						int sz = temp.size();
						del_argv(argv, sz);
						del_incstr(in_cstr);
						_exit(1);
					}
					int sz = temp.size();
					del_argv(argv, sz);
					del_incstr(in_cstr);
					_exit(1);
				}
				else
				{
					//takes care of the true || true or false case
					if(or_cnt == 1)
					{
						status = 3;
						int sz = temp.size();
						del_argv(argv, sz);
						break;
					}
				}
			}
			else
			{
				//cout << "in parent" << endl;
				if(wait(&status) == -1)
				{
					perror("Child still running");
				}
				else
				{
					//cout << "in status check" << endl;
					if(WIFEXITED(status))
					{
						if(WEXITSTATUS(status) == 0 && or_cnt == 1)
						{
							//e.g. true || true/false 
							//since arg1 returned true we dont need to run the second arg
							//break out of loop and return
							//cout << "exit status: " << WEXITSTATUS(status) << endl;
							int sz = temp.size();
							del_argv(argv, sz);
							break;
						}
						if(WEXITSTATUS(status) != 0 && and_cnt == 1)
						{
							//e.g. false && true
							//since arg1 is false, dont do second arg
							//execvp will fail and child will have exited with 1
							//In that case go in here
							//break out of loop and return
							//cout << "exit status: " << WEXITSTATUS(status) << endl;
							int sz = temp.size();
							del_argv(argv, sz);
							break;
						}
					}
					//Take in the next argument if there is any
					////Go back to the top and repeat the process
					//e.g. token = ls
					token = strtok_r(NULL, delim, &save_1);
				}
			}
			int sz = temp.size();
			del_argv(argv, sz);
		}
		del_incstr(in_cstr);
	}
	return 0;
}
