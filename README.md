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
Connectors that are not doubles will not work.
e.g. `ls &&& ls` or anything similar will not work like how it does in bash.
Instead it would run like `ls && ls`.

Invalid commands will not display the same message as a normal terminal.

Commands that start with a connector does not work like bash does.
e.g. `$ & ls` or `$ && ls` or `$ ls &` or `$ ls &&` or any of sort of combination
of that will not work like bash does.
Instead those commands will run as `ls`.

Multiple commands do not work
e.g. `ls && pwd || ls` or anything that has multiple uses of `&&` or `||`
will not work like bash does.
Instead only the first command is ran.

`echo "hello"` doesnt output hello. Instead it outputs `"hello"`

`echo > file` doesnt work

# ls
ls is a basic program that mimics the bash ls

##Installation
To install run the below code in your terminal

```
$ git clone  http://github.com/yourusername/rshell.git
$ cd rshell
$ git checkout hw1
$ make
$ bin/ls
```

#Known bugs
An unsupported flag (e.g. not -a, -l -R or any combination of those)
will cause the program to error and exit. This is intentional

Formating for the flags is not perfect. Formating may be unorganized
but still readable.

The block counter total is mispositioned. Normally when running ls -l,
the Total will be displayed at the top, my ls will display it at the
bottom
