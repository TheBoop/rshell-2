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

int main(int argc, char* argv[])
{
	const char *dir = ".";
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
		cout << direntp->d_name << endl;
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
