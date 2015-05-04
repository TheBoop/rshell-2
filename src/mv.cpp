#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <errno.h>
#include <dirent.h>
#include <cstring>

using namespace std;

int main(int argc, char** argv){
    vector<string> paths;
    
    if(argc < 2){
        cout << "Not enough arguments" << endl;
        return 1;
    }

    struct stat s;
    struct stat d;

//===========CHECK FILE 1======================
    if(stat(argv[1],&s) < 0){
        perror("File DNE");
        exit(1);
    }
    if(!S_ISREG(s.st_mode)){
        cout << "Unable to change non-regular files" << endl;
        return 1;
    }

//==============CHECK FILE 2=================
    if(stat(argv[2],&d) < 0){
        if(link(argv[1],argv[2])<0){
            perror("Link");
            exit(1);
        }
        if(unlink(argv[1])<0){
            perror("Unlink");
            exit(1);
        }
    }else if(S_ISDIR(d.st_mode)){
        string file(argv[2]);
        file.append("/");
        file.append(argv[1]);
        if(link(argv[1],file.c_str())<0){
            perror("Link");
            exit(1);
        }
        if(unlink(argv[1])<0){
            perror("Unlink");
            exit(1);
        }
    }else{
        cout << "Second File Exists" << endl;
    }

    return 0;
}
