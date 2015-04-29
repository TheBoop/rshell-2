#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h> //stat
#include <sys/types.h> //stat, opendir, readdir, closedir
#include <errno.h> //perror
#include <stdio.h> //perror
#include <dirent.h> //readdir, closedir
#include <unistd.h> //stat
#include <fcntl.h> //perror

using namespace std;

/*
void paramcheck(int argc, char* argv[])
{
	vector<string> param;
	vector<string> arg;
	//int a_param = 0;
	//int l_param = 0;
	//int R_param = 0;
	for(int i = 1; i < argc; i++)
	{
		param.push_back(argv[i]);
	}
	for(unsigned i = 1; i < param.size(); i++)
	{
		if(param.at(i).at(0) == '-')
		{
			arg.push_back(param.at(i));
		}
	}
	for(unsigned i = 0; i < param.size(); i++)
	{
		cout << "i:" << i << " " << param.at(i) << endl;
	}
	for(unsigned i = 0; i < arg.size(); i++)
	{
		if(arg.at(i).find('l') != string::npos)
		{
			cout << "found l" << endl;
		}
	}
}
*/

vector<int> check(char argv[], vector<int>& flags)
{
	int a_param = 0;
	int l_param = 0;
	int R_param = 0;
	if(argv[0] == '-')
	{
		for(int i = 1; argv[i] != '\0'; i++)
		{
			if(argv[i] == 'a')
			{
				a_param = 1;
			}
			if(argv[i] == 'l')
			{
				l_param = 1;
			}
			if(argv[i] == 'R')
			{
				R_param = 1;
			}
		}
	}
	flags.at(0) = a_param;
	flags.at(1) = l_param;
	flags.at(2) = R_param;
	//display flags check
	//for(unsigned j = 0; j < flags.size(); j++)
	//{
	//	cout << "j:" << j << " "  << flags.at(j) << endl;
	//}
	return flags;
}

void ls(dirent *direntp, vector<int> flags)
{
	//cout << "d: "  << direntp->d_name[0] << " " << endl;
	if(direntp->d_name[0] != '.' || flags.at(0) == 1)
	{
		if(flags.at(1) == 1)
		{
			cout << "-l detected" << endl;
		}
		cout << direntp->d_name << endl;
	}
	//if(direntp->d_name[0] == '.' && flags.at(0) == 1)
	//{
	//	cout << direntp->d_name << endl;
	//}
}

int main(int argc, char* argv[])
{
	//paramcheck(argc, argv);
	vector<int> flags(3, 0);
	for(int i = 1; i < argc; i++)
	{
		check(argv[i], flags);
	}

	//flags check
	for(unsigned j = 0; j < flags.size(); j++)
	{
		cout << "j:" << j << " "  << flags.at(j) << endl;
	}

	char dir[] = ".";
	DIR *dirp;
	if(NULL == (dirp = opendir(dir)))
	{
		perror("There was an error with opendir().");
		exit(1);
	}
	dirent *direntp;
	errno = 0;
	while( NULL != (direntp = readdir(dirp)) )
	{
		ls(direntp, flags);
	}
	if(errno != 0)
	{
		perror("There was an error with readdir().");
		exit(1);
	}
	if(-1 == closedir(dirp))
	{
		perror("There was an error with closedir().");
		exit(1);
	}
	return 0;
}
