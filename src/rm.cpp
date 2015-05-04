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

void rm_dir(vector<string> paths, bool rec){
    DIR *dirp;
    struct dirent *direntp;
        
    //LOOPS THROUGH ALL THE PATHS GIVEN
    for(unsigned int i = 0; i<paths.size();++i){
        struct stat s;
        if(stat(paths[i].c_str(),&s) < 0){
            perror("Stat");
            exit(1);
        }
        if(!rec){
            if(S_ISDIR(s.st_mode)){
                cout << "Cannot delete directories" << endl;
                return;
            }else if(S_ISREG(s.st_mode)){
                if(unlink(paths[i].c_str())<0){
                    perror("Unlink");
                    exit(1);
                }
            }else{
                cout << "Unable to remove non-regular fil" << endl;
                return;
            }
        }else{
            if(S_ISDIR(s.st_mode)){
                if((dirp = opendir(paths[i].c_str())) == NULL){
                    perror("OPEN_DIR");
                    exit(1);
                }
                
                errno = 0;
                while((direntp = readdir(dirp)) != NULL){
                    string file = direntp->d_name;
                    if(file != "." && file != ".."){
                        string temp(paths[i] + "/" + direntp->d_name);
                        vector<string> fp;
                        fp.push_back(temp);
                        if(stat(fp[0].c_str(),&s)<0){
                            perror("Dir Stat");
                            exit(1);
                        }
                        if(S_ISDIR(s.st_mode)){
                            rm_dir(fp,true);
                        }else if(unlink(fp[0].c_str())<0){
                            perror("Unlink Dir");
                            exit(1);
                        }
                    }
                }
                if(errno!=0)
                {
                    perror("readdir() failed");
                    exit(1);
                }
                if(rmdir(paths[i].c_str())<0){
                    perror("Remov Dir");
                    exit(1);
                }
            }else{
                if(unlink(paths[i].c_str())<0){
                    perror("Unlink");
                    exit(1);
                }
            }
        }
    }
}

int main(int argc, char** argv){
    vector<string> paths;
    bool rec =false;

    if(argc < 1){
        cout << "Not enough arguments" << endl;
        return 1;
    }

    for(int i=1; i<argc; ++i){
        if(argv[i][0] == '-'){
            if(argv[i][1] == '\0'){
                cout << "No flag input after - " << endl;
                return -1;
            }else if(argv[i][1] == 'r') {
                rec = true;
            }else{
                cout << "Flag Error" << endl;
                return -1;
            }
        }else{
            paths.push_back(argv[i]);
        }
    }
    
    /*for(unsigned int i =0; i<paths.size(); ++i){
        cout << "Path: " << paths[i] << endl;
    }*/

    rm_dir(paths, rec);

    return 0;
}
