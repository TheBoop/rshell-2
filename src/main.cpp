#include <iostream>
#include <string.h>
#include <unistd.h> //execvp
#include <stdio.h> //perror
#include <errno.h> //perror
#include <vector>
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
		cout << "Tokens of input:" << endl;
		while(token != NULL)
		{
			cout << token << endl;
			token = strtok(NULL, delim);
		}
		//vector<string> words; //not sure if i need to do this
	}

	//will give me login (e.g. lzhu012)
	//char* login = getlogin();
	//cout << login << endl;
	return 0;
}
