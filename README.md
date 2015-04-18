# rshell
rshell is a basic program that mimics some features of the bash terminal. Like the
bash terminal, rshell accepts input from the user and tries to run them as they would
in a normal bash terminal. Currently, many features do not work. At the moment some
commands such as ls, mkdir, pwd, etc with connectors work to some extent.

##Installation
To install run the below code in your terminal

```
$ git clone  https://github.com/zhuleon/rshell.git
$ cd rshell
$ git checkout hw0
$ make
$ bin/rshell
```

##Known Bugs
Connectors that are not doubles, e.g. && or || will not work properly

Invalid syscalls are not handles and will error

invalid executables are not handled


