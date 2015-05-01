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
#include <sys/ioctl.h> //formatting winsize

using namespace std;

//Checks if flags were passed and returns a vector storing that information
vector<int> check(char argv[], vector<int>& flags, vector<string>& directory)
{
	int a_param = 0;
	int l_param = 0;
	int R_param = 0;
	if(argv[0] == '-')
	{
		if(argv[1] == '\0')
		{
			cout << "Error: Unsupported flag" << endl;
		}
		for(int i = 1; argv[i] != '\0'; i++)
		{
			if(argv[i] == 'a')
			{
				a_param = 1;
			}
			else if(argv[i] == 'l')
			{
				l_param = 1;
			}
			else if(argv[i] == 'R')
			{
				R_param = 1;
			}
			else
			{
				cout << "Error: Unsupported flag" << endl;
				exit(1);
			}
		}
	}
	else
	{
		//if argv is a path instead of a flag
		string s_dir = argv;
		directory.push_back(argv);
		//cout << "argv= " << argv << " " << "path.at(0) = " << path.at(0) << endl;
	}
	flags.at(0) = a_param;
	flags.at(1) = l_param;
	flags.at(2) = R_param;
	return flags;
}

//compares letters in a file, used for sorting
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

//Checks the file permissions
void permissions(struct stat p)
{
	(S_ISDIR(p.st_mode)) ? cout << "d" : 
	(S_ISCHR(p.st_mode)) ? cout << "c" :
	(S_ISBLK(p.st_mode)) ? cout << "b" : 
	(S_ISLNK(p.st_mode)) ? cout << "l" : cout << "-";

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

//gets the max of file lengths
unsigned maxlen(vector<string> file)
{
	unsigned max_len = 0;
	for(unsigned i = 0; i < file.size(); i++)
	{
		if(file.at(i).size() > max_len)
		{
			max_len = file.at(i).size();
		}
	}
	return max_len;
}

void ls(vector<string> file, vector<int> flags, vector<string>& rev_dir, string rev_sdir)
{
	//cout << "d: "  << direntp->d_name[0] << " " << endl;
	struct stat fstat;
	int totalblocks = 0;
	int max_len = maxlen(file)+1;
	int cur_len = 0;
	vector<string> output;
	struct winsize w;
	ioctl(STDOUT_FILENO,TIOCGWINSZ, &w);
	int windowsz = w.ws_col;
	//for(unsigned i = 0; i < file.size(); i++)
	//{
	//	if(file.at(i).at(0) != '.' || flags.at(0) == 1)
	//	{
	//		if(flags.at(1) == 1)
	//		{
	//			if(stat(file.at(i).c_str(), &fstat) == -1)
	//			{
	//				perror("Cannot access file1");
	//				exit(1);
	//			}
	//			totalblocks += fstat.st_blocks;
	//		}
	//	}
	//}
	//if(flags.at(1) == 1)
	//{
	//	cout << "Total " << totalblocks/2 << endl;
	//}
	for(unsigned i = 0; i < file.size(); i++)
	{
		// -a
		//cout << "file.at(i) = " << file.at(i) << " " << file[i].at(0) << endl;
		if(file.at(i).at(0) != '.' || flags.at(0) == 1)
		{
			//-l
			string nrev_sdir = rev_sdir;
			nrev_sdir.append("/");
			nrev_sdir.append(file.at(i));
			if(stat(nrev_sdir.c_str(), &fstat) == -1)
			{
				perror("Error: stat()");
				exit(1);
			}

			if(flags.at(2) == 1 && S_ISDIR(fstat.st_mode) && file.at(i) != "." && file.at(i) != "..")
			{
				//cout << rev_sdir << ": " << endl;
				rev_dir.push_back(nrev_sdir);
			}
			if(flags.at(1) == 1)
			{
				//permissions
				permissions(fstat);
				cout << " ";

				//hard link count
				cout << fstat.st_nlink << " ";
				totalblocks += fstat.st_blocks;

				//user info
				struct passwd *usrinfo;
				if((usrinfo = getpwuid(fstat.st_uid)) == NULL)
				{
					perror("Error: getpwuid()");
					exit(1);
				}
				cout << usrinfo->pw_name << " ";

				//group info
				struct group *grpinfo;
				if((grpinfo = getgrgid(fstat.st_gid)) == NULL)
				{
					perror("Error: getgrgid()");
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
					perror("Error: strftime()");
					exit(1);
				}
			}
			if(flags.at(1) == 1)
			{
				cout << file.at(i) << endl;
			}
			//cout << "size: " << file.at(i).size() << " ";
			//cur_len = file.at(i).size();
			//max_len = max(cur_len, max_len);
			//cout << "max: " << max_len;
			else
			{
				//cout.width(max_len); cout << left << file.at(i);
				if(cur_len + max_len > windowsz)
				{
					cout << endl;
					cur_len = 0;
				}
				else
				{
					cur_len += max_len;
				}
				cout.width(max_len); cout << left << file.at(i);
			}
			//output.push_back(file.at(i));
			//output.push_back("\n");
		}
	}
	if(flags.at(1) != 1)
	{
		cout << endl;
	}
	if(flags.at(1) == 1)
	{
		cout << "Total " << totalblocks/2 << endl;
	}
	//int formatcol = w.ws_col / max_len;
	//cout << "num: " << formatcol << endl;
	//int col_counter = 0;
	//for(unsigned k = 0; k < output.size(); k++)
	//{
	//	cout.width(formatcol); cout << left << output.at(k);
	//	col_counter++;
	//	if(col_counter == formatcol)
	//	{
	//		cout << endl;
	//	}
	//}
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
	vector<string> rev_dir;
	string rev_sdir;
	for(int i = directory.size() - 1; i >= 0; i--)
	{
		rev_dir.push_back(directory.at(i));
	}
	DIR *dirp;
	dirent *direntp;
	string dname;
	while(!rev_dir.empty())
	{
		rev_sdir = rev_dir.back();
		rev_dir.pop_back();
		if(NULL == (dirp = opendir(rev_sdir.c_str())))
		{
			perror("Error: opendir()");
			exit(1);
		}
		errno = 0;
		while( NULL != (direntp = readdir(dirp)) )
		{
			dname = direntp->d_name;
			file.push_back(dname);
		}
		sort(file.begin(), file.end(), my_compare);
		ls(file, flags, rev_dir, rev_sdir);
		if(errno != 0)
		{
			perror("Error: readdir()");
			exit(1);
		}
		if(-1 == closedir(dirp))
		{
			perror("Error: closedir()");
			exit(1);
		}
		file.clear();
		if(!rev_dir.empty())
		{
			cout << endl;
		}
	}
	return 0;
}
