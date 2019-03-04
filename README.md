#### Program Usage:

_exit_ - exit the program

_ctrl-C_ - kill the child process

setenv [_var_] [_value_] - If environment variable var does not exist, then kapish should create it. kapish
                       should set the value of var to value, or to the empty string if value is omitted.
                       Note: Initially, kapish inherits environment variables from its parent. kapish
                       should be able to modify the value of an existing environment variable or create
                       a new environment variable via the setenv command. kapish should be able to
                       set the value of any environment variable; but the only environment variables
                       that it explicitly uses are HOME and PATH.
                       
unsetenv [_var_] - kapish should destroy the environment variable var.

cd [_dir_] - kapish should change kapish's working directory to dir, or to the HOME
           directory if dir is omitted.


#### Setting Program Debug Level
To set the program debug level, change the _debug_ variable in kapish.c to:

**0** - No console debug logging

**1** - Full console debug logging

#### Credits

_split_line.h_ - Help from: https://brennan.io/2015/01/16/write-a-shell-in-c/

_sigintHandler_ - Help from: https://www.geeksforgeeks.org/write-a-c-program-that-doesnt-terminate-when-ctrlc-is-pressed/
