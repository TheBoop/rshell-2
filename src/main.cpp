#include <iostream>
#include <string>
#include <string.h>
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
		cout << "$ ";
		getline(cin, input);
		if(input == "exit")
		{
			return 0;
		}
		//covert string to char* maybe; yes the method below seems to have worked
		char* in_cstr = new char[input.length()+1];
		//char* in_cstr[input.length()+1]; doesn't work
		strcpy(in_cstr, input.c_str());
		
		//tokenize words
		char delim[] = " ";
		//char input_cstr[] = "ls"; delete me
		char* token;
		//char* save_1;
		//strcpy(input_cstr, input.c_str());
		token = strtok(in_cstr, delim); // &save_1);
		//cout << "token: " << token << endl;
		//char* path;
		//cout << "c_string: " << token[0] << endl; testing
		//cout << "c_string: " << token[1] << endl; testing
		char* argv[1024];
		int i = 0;
		cout << "Tokens of input:" << endl;
		//int x = strlen(token);
		//cout << "x:" << x;
		//for(int k = 0; k < x; k++)
		//{
			//cout << "k:" << k;
			//cout << token[k] << endl;
			//token = strtok(NULL, delim); //causes problems with loop
		//}
		while(token != NULL)
		{
			cout << token << endl;
			argv[i] = token;
			i++;
			token = strtok(NULL, delim);
		}
		int pid = fork();
		if(pid == -1)
		{
			perror("fork failed");
		}
		else if(pid != 0)
		{
			cout << "In parent, going to wait" << endl;
			wait(0);
		}
		else if(pid == 0)
		{
			cout << "In child" << endl;
			if(execvp(argv[0], argv) != 0)
			{
				perror("execvp failed");
			}
			else
			{
				execvp(argv[0], argv);
			}
		}
	}
	//currently if ls -a run then ls, ls -a still runs
	//will give me login (e.g. lzhu012)
	//char* login = getlogin();
	//cout << login << endl;
	return 0;
}
