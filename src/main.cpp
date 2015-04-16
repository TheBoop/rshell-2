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
		char* argv[1024];
		int i = 0;
		cout << "Tokens of input:" << endl;
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
			perror("fork");
		}
		else if(pid != 0)
		{
			cout << "In parent, going to wait" << endl;
			wait(0);
		}
		else if(pid == 0)
		{
			cout << "In child" << endl;
			execvp(argv[0], argv);

		}
		//vector<string> words; //not sure if i need to do this
	}

	//will give me login (e.g. lzhu012)
	//char* login = getlogin();
	//cout << login << endl;
	return 0;
}
