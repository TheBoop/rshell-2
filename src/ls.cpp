#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <cstdlib>
#include <sys/stat.h> //stat
#include <sys/types.h> //stat, opendir, readdir, closedir
#include <unistd.h> //stat
#include <errno.h> //perror
#include <stdio.h> //perror
#include <dirent.h> //readdir, closedir
#include <fcntl.h> //perror
#include <pwd.h> //getpwuid
#include <grp.h> //getgrgid
#include <time.h> //gettime

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

void permissions(struct stat p)
{
	(S_IFDIR & p.st_mode) ? cout << "d" : 
	(S_IFCHR & p.st_mode) ? cout << "c" :
	(S_IFBLK & p.st_mode) ? cout << "b" : 
	(S_IFLNK & p.st_mode) ? cout << "l" : cout << "-";
	
	(S_IRUSR & p.st_mode) ? cout << "r" : cout << "-";
	(S_IWUSR & p.st_mode) ? cout << "w" : cout << "-";
	(S_IXUSR & p.st_mode) ? cout << "x" : cout << "-";

	(S_IRGRP & p.st_mode) ? cout << "r" : cout << "-";
	(S_IWGRP & p.st_mode) ? cout << "w" : cout << "-";
	(S_IXGRP & p.st_mode) ? cout << "x" : cout << "-";

	(S_IROTH & p.st_mode) ? cout << "r" : cout << "-";
	(S_IWOTH & p.st_mode) ? cout << "w" : cout << "-";
	(S_IXOTH & p.st_mode) ? cout << "x" : cout << "-";
}

void ls(dirent *direntp, vector<int> flags)
{
	//cout << "d: "  << direntp->d_name[0] << " " << endl;
	// -a
	if(direntp->d_name[0] != '.' || flags.at(0) == 1)
	{
		//-l
		if(flags.at(1) == 1)
		{
			struct stat fstat;
			if(stat(direntp->d_name, &fstat) == -1)
			{
				perror("????");
				exit(1);
			}
			
			//permissions
			permissions(fstat);
			cout << "\t";

			//user info
			struct passwd *usrinfo;
			if((usrinfo = getpwuid(fstat.st_uid)) == NULL)
			{
				perror("getpwuid() failed");
				exit(1);
			}
			cout << usrinfo->pw_name << "\t";

			//group info
			struct group *grpinfo;
			if((grpinfo = getgrgid(fstat.st_gid)) == NULL)
			{
				perror("getgrgid() failed");
				exit(1);
			}
			cout << grpinfo->gr_name << "\t";

			//size info
			cout << fstat.st_size << "\t";

			////time info
			//struct tm* tminfo;
			//char timestring[256];
			//tm = localtime(&fstat.st_mtime);
			//if(strftime(timestring, sizeof(timestring), "%b %d %H:%M:", tminfo) != 0)
			//{
			//	cout << timestring << "\t";
			//}
			//else
			//{
			//	perror("cant get time");
			//	exit(1);
			//}
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
