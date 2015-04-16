#include <iostream>
#include <string>
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
		//tokenize words
		char delim[] = " ";
		char *token;
		char *save_1;
		token = strtok_r(input, delim, &save_1);
		cout << "token: " << token << endl;
		//for(int i = 0; token != NULL; i++)
		//{
		//	token = strtok_r(
		//}
		//vector<string> words; //not sure if i need to do this
	}

	//will give me login (e.g. lzhu012)
	//char* login = getlogin();
	//cout << login << endl;
	return 0;
}
