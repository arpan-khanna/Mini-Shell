# Mini-Shell

------------------------------------------------------------------------------------------

Programming language used – C
Operating Systems the file will work on – Linux

------------------------------------------------------------------------------------------

For running the file:
1. Compile the file by typing gcc main.c
2. Then execute it by typing ./a.out
------------------------------------------------------------------------------------------
FEATURES


1) We will give the input and it will take the input till the newline. Spaces and tabs
are used as a delimiter i.e it parse the commands separated by spaces and tabs. If the
input is null, then it will do nothing.History is stored in history.txt file. 

Note- If we just give spacebars and tabs as an input and no character then it will
cause segmentation to break. In case it happens then we have to launch our shell
by ./a.out.

2) When we type ./a.out then our shell will start running and we can give input.
If the input is not null, then it will be stored in history.txt file in tmp folder. To
check the history we can anytime type history and it will show us the history.

3) To print environment variable, type echo $env where env is the name of the
environment variable. If the environment variable was defined then it will show the
value of env variable else a newline.

4) To set the value of environment variable, type setenv var=value where var is the
name of the environment variable and value is its value. In this command please
follow the above syntax strictly i.e no sapces in assigning the value and one space
after setenv. If the value cannot be set, then it will show us an error message. We can
check if value is assigned to it by following point 3.

5) If we type just cd then directory will be changed to home/user . If we want to
change the direcory to a specific directory then type cd path_of_directory Here you
can either specify full path or relative path or just ../ all will be working.
Note- The name of directory should not contain spaces as spaces and tabs are used as
a delimiter(parser).

6) If we want to know the location of current directory type pwd
If we want to put something on the prompt type echo sometext
We can type help to see the overview of this shell.
We can clear screen by typing clear
Below is a just a small list of the commands which the shell can execute
man, chsh, whereis, passwd, date, cal, clear, sleep, alias, history, apropos, exit, logout,
shutdown, ls, cat, more, less, touch, cp, mv, rm, script, mkdir, cd, pwd, rmdir, chmod,
grep,sort,awk
where the above commands can be executed just by following the syntax of the above
commands i.e command_name [options] attributes

7) Not only the above commands this shell executes all the linux external commands
by forking and using exec family call. Hence we can execute all the linux external by
following the syntax. If a command is not found or it is not properly executed then it
will gernerate an error message.
Note -In these commands there is no problem on the spaces and the user can use
spaces and tabs as per him/her as the parser in these commands is very well set. And
not only in this, the user can use spaces as per convenience in almost all the
commands except a few and the commands which will have restriction on spaces will
be explicitly mentioned in the readme file.

8) The shell checks the the presence of single pipe and does the execution properly.
The output of the first command is fed as an input in the second command.
The syntax of piping is command1 | command2 where command1 and command2
are the two commands.
Few examples- ls | wc , ls | grep a , ls -l | more , sort record.txt | uniq ,
cat input | grep arpan , cat input | head -7 .... and numerous more

Imp note- If command1 and command2 are valid commands then it will work
properly and will show the desirable results and if there will be an error it will
generate an error message. But if they are some random gibberish stuff then the
program will not work properly. In this case we have to we have to use key
combination ctrl+C and exit the program and then rerun the program by again
typing ./a.out. Hence it is advised not to use any random stuff only while piping.

9) The program works for input redirection. To use input redirection type
command < filename It will take the input from the input file.
Few examples – cat < input, sort < input , awk awkcommand < inputfile ,
./executable < input , grep a < input ... and numerous more

10) The program works for output and append redirection. The difference between
output and append redirection is that output redirection open the file as a fresh new
file and then put the contents whereas append redirection appends the output at the
end of the file. Both the programs creates the file if the file dont exist.
Output redirection (>) command > outputfile
Append redirection(>>) command >> outputfile
Few examples echo hello world > filename , ping -c 5 www.google.com > outputfile ,
./executable > outputfile, ls > output file and same for append and many more
examples

11) Special Case- We can both use input and output redirection in the same command
Below is the syntax. Please follow the syntax exactly and number of spaces as used.
Syntax command < inputfile > outputfile
the syntax is command space ‘<’ space inputfilename space ‘>’ space outputfilename
Few examples- sort < input > output , ./executable < input > output ,
grep < input > output ...and many more examples

12) To exit the shell type exit It will remove the history and exits the program.
Imp note – Sometimes if we type exit then it just deletes the history file and didn’t
exit the program. Hence for exiting the shell we have to type exit twice or thrice.
