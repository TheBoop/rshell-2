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
#include <time.h> //gettime strftime
#include <algorithm> //sort

using namespace std;

vector<int> check(char argv[], vector<int>& flags, vector<string>& path)
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
	else
	{
		//if argv is a path instead of a flag
		string s_path = argv;
		path.push_back(s_path);
		//cout << "argv= " << argv << " " << "path.at(0) = " << path.at(0) << endl;
	}
	flags.at(0) = a_param;
	flags.at(1) = l_param;
	flags.at(2) = R_param;
	return flags;
}

bool my_compare(string a, string b)
{
	for(unsigned i = 0; i < a.size(); i++)
	{
		a.at(i) = tolower(a.at(i));
	}
	for(unsigned i = 0; i < b.size(); i++)
	{
		b.at(i) = tolower(b.at(i));
	}
	return a < b;
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

void ls(vector<string> file, vector<int> flags)
{
	//cout << "d: "  << direntp->d_name[0] << " " << endl;
	struct stat fstat;
	int totalblocks = 0;
	for(unsigned i = 0; i < file.size(); i++)
	{
		if(file.at(i).at(0) != '.' || flags.at(0) == 1)
		{
			if(flags.at(1) == 1)
			{
				if(stat(file.at(i).c_str(), &fstat) == -1)
				{
					perror("Cannot access");
					exit(1);
				}
				totalblocks += fstat.st_blocks;
			}
		}
	}
	if(flags.at(1) == 1)
	{
		cout << "Total " << totalblocks/2 << endl;
	}
	for(unsigned i = 0; i < file.size(); i++)
	{
		// -a
		//cout << "file.at(i) = " << file.at(i) << " " << file[i].at(0) << endl;
		if(file.at(i).at(0) != '.' || flags.at(0) == 1)
		{
			//-l
			//if(stat(file.at(i).c_str(), &fstat) == -1)
			//{
			//	perror("Cannot access");
			//	exit(1);
			//}
			if(flags.at(1) == 1)
			{
				if(stat(file.at(i).c_str(), &fstat) == -1)
				{
					perror("Cannot access");
					exit(1);
				}
				//permissions
				permissions(fstat);
				cout << " ";

				//hard link count
				cout << fstat.st_nlink << " ";

				//user info
				struct passwd *usrinfo;
				if((usrinfo = getpwuid(fstat.st_uid)) == NULL)
				{
					perror("getpwuid() failed");
					exit(1);
				}
				cout << usrinfo->pw_name << " ";

				//group info
				struct group *grpinfo;
				if((grpinfo = getgrgid(fstat.st_gid)) == NULL)
				{
					perror("getgrgid() failed");
					exit(1);
				}
				cout << grpinfo->gr_name << " ";

				//size info
				cout.width(7); cout << right << fstat.st_size << " ";

				//time info
				struct tm *tminfo;
				char timebuf[256];
				tminfo = localtime(&fstat.st_mtime);
				if(strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", tminfo) != 0)
				{
					cout << timebuf << " ";
				}
				else
				{
					perror("cant get time");
					exit(1);
				}
			}
			cout << file.at(i) << endl;
		}
	}
}

int main(int argc, char* argv[])
{
	//Tracks flags
	vector<int> flags(3, 0);

	//contains directory names
	vector<string> directory;
	
	//Contains file names
	vector<string> file;

	for(int i = 1; i < argc; i++)
	{
		check(argv[i], flags, directory);
	}

	//flags check
	//for(unsigned j = 0; j < flags.size(); j++)
	//{
	//	cout << "j:" << j << " "  << flags.at(j) << endl;
	//}

	//char dir[] = ".";
	//string dirname = ".";
	if(directory.size() == 0)
	{
		directory.push_back(".");
	}
	DIR *dirp;
	dirent *direntp;
	string dname;
	for(unsigned i = 0; i < directory.size(); i++)
	{
		if(NULL == (dirp = opendir(directory.at(i).c_str())))
		{
			perror("There was an error with opendir().");
			exit(1);
		}
		errno = 0;
		while( NULL != (direntp = readdir(dirp)) )
		{
			//ls(direntp, flags);
			dname = direntp->d_name;
			file.push_back(dname);
		}
		sort(file.begin(), file.end(), my_compare);
		ls(file, flags);
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
		if(i+1 != directory.size())
		{
			cout << endl;
		}
	}
	return 0;
}
