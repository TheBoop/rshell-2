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
Connectors that are not doubles, e.g. &&& or ||| will not work properly

Invalid commands will not display the same message as a normal terminal

Commands that start with a connector does not work like bash does.
e.g. '$ & ls' or '$ && ls' or '$ ls &' or '$ ls &&' or any of sort of combination
of that will not work like bash does.
Instead those commands will run ls normally.

echo "hello" doesnt output hello. Instead it outputs "hello"

echo > file doesnt work
