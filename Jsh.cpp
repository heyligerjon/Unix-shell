//cs1730bash

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <string>
#include <stdio.h>
#include <string.h>
#include <cstdlib>

using namespace std;

int main(){

  bool loop = true;

  char buf[PATH_MAX];

  std::string cwd = getcwd(buf, PATH_MAX);

  while(loop){

    char * argv[11];

    cout << "1730sh:" << cwd << "$ ";

    std::string comm = "";
    std::string x = "";

    std::string args;

    getline(cin, args);

    comm = args.substr(0, args.find(" "));

    const char * foo = comm.c_str();

    std::string temp = "";

    for(int i = 0; i < 11; i+=1){

      if ((int)args.find(" ") == -1) {
        temp = args.substr(0, args.find(" "));
        argv[i] = (char*)(malloc(temp.length()));
        strcpy(argv[i], temp.c_str());
        break;
      }

      temp = args.substr(0, args.find(" "));
      argv[i] = (char*)(malloc(temp.length()));
      strcpy(argv[i], temp.c_str());
      args.erase(0, (args.find(" ")+1));

    }

    //int out_backup = dup(STDOUT_FILENO);                  //creating a copy of standard output
    //int in_backup = dup(STDIN_FILENO);                    //creating backup for standard input

    pid_t pid;

    if ((pid=fork()) == -1) { // error
      perror("FORK ERROR");
    }
    else if (pid == 0) {    // in child process

      for (int i = 0; i < 11; i++) {
        if (argv[i] == nullptr) {
          continue;
        }
        if (argv[i] != nullptr && string(argv[i]) == "<") {
          int fd = open(argv[i+1], O_RDONLY | O_CREAT, 0777); //opening the file
          dup2(fd, STDIN_FILENO);                             //redirecting read to file

          argv[i] = nullptr;
          argv[i+1] = nullptr;
        }
        else if(argv[i] != nullptr && string(argv[i]) == ":" && string(argv[i]+1) == ">") {
          int fd = open(argv[i+2], O_WRONLY | O_CREAT, 0777); //opening the file
          ftruncate(fd, 0);                           //redirecting write to file
          close(fd);

          argv[i] = nullptr;
          argv[i+1] = nullptr;
          break;
        }
        else if(argv[i] != nullptr && string(argv[i]) == ">") {
          int fd = open(argv[i+1], O_WRONLY | O_CREAT, 0777); //opening the file
          dup2(fd, STDOUT_FILENO);                           //redirecting write to file

          argv[i] = nullptr;
          argv[i+1] = nullptr;
        }
        else if(argv[i] != nullptr && string(argv[i]) == ">>"){
          int fd = open(argv[i+1], O_WRONLY | O_CREAT, 0777); //opening the file

          lseek(fd, 0, SEEK_END);
          dup2(fd, STDOUT_FILENO);                           //redirecting write to file

          argv[i] = nullptr;
          argv[i+1] = nullptr;
        }
      }
      if(argv[0] != nullptr)
        execvp(foo, argv);
    }
    else {                  // in parent process
      wait(nullptr);
    }

    if (comm == "export"){
      putenv(argv[1]);
    }

    if (comm == "exit") {
      loop = false;
      exit(0);
    }

    for (int i = 0; i < 11; i++) {
      argv[i] = nullptr;
    }

  }//while(loop)


}//main
