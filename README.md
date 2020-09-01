# Shoots
Language Jam C++ Attempt

This is a simple programming language meant to teach how to program
as well as to get people used to programming in lower level languages.

In order to run your file or an example: compile or run the code then type ./main FILENAME
As an example, 
&nbsp&nbsp&nbsp&nbsp    'clang++-7 -pthread -std=c++17 -o main main.cpp'
&nbsp&nbsp&nbsp&nbsp    './main greeting.snr' 
would compile the interpreter then run that example file. You only need to compile the interpreter once

Syntactically and format-wise this is similar to assembly and fortran as in a command followed by its operands or parameters.
In order to make it a lot easier, this uses named and typed variables, and every command is in the format:
&nbsp&nbsp&nbsp&nbsp    COMMAND param1,param2,param3,....
Another way to attempt to make it earier to learn is that all variables are static and scoped to either the main file or the function they are created in.
If the value needs to be modified, it must be deleted first. The main file and all functions have access to a single stack if something more advanced is required.

Every command is 3 letters, and all whitespace is ignored on file read (except in quotes)
So, every command, no matter what it is is 3 letters followed by comma seperated parameters (if any)

Below is a list of available commands and what they do / how to use them

#### Basic Commands:
com (comment):          any text after the com will not be read by the interpreter
int (create integer):   in the format 'int VARNAME' or 'int VARNAME,VALUE', created an integer with name VARNAME and optional value VALUE
dbl (create double):    in the format 'dbl VARNAME' or 'dbl VARNAME,VALUE', created a double with name VARNAME and optional value VALUE
str (create string):    in the format 'str VARNAME' or 'dbl VARNAME,VALUE', created a string with name VARNAME and optional value VALUE
&nbsp&nbsp&nbsp&nbsp    pretty much anything can be put into a string except ". If things like escape characters and spaces are
&nbsp&nbsp&nbsp&nbsp    required, use quotes to surround it as in str testString. "Hello World!\n"
inp (take user input):  in the format 'inp VARNAME', creates a variable with the given user in (characters followed by hitting enter, trims the newline)
&nbsp&nbsp&nbsp&nbsp    will default to string unless VARNAME is already created (ex. with int VARNAME). If the variable is already declared it will
&nbsp&nbsp&nbsp&nbsp    attempt to convert the user input to that type and error if impossible.
out (output):           write list of params to the console, can be strings in quotes or variable names to write the values of. Does not automatically
&nbsp&nbsp&nbsp&nbsp    space or create newlines
add (numerical add):    in the format 'add VARNAME,VALUE,VALUE' where VALUE could also be replaced with another VARNAME. Stores the addition of the
&nbsp&nbsp&nbsp&nbsp    two variables and or values into the first VARNAME listed. If it is already created it will try to use that type, otherwise
&nbsp&nbsp&nbsp&nbsp    by default be a double. 
sub (numerical add):    same as add except subtracts the second variable/value from the first and stores it in VARNAME
mul (multiply):         same as add except multiplies the second variable/value to the first and stores it in VARNAME
div (divide):           same as add except divides the first variable/value by the second and stores it in VARNAME
mod (modulo):           same as add except takes the ramainder of the first variable/value divided by the second and stores it in VARNAME
gth (greater than):     in the format 'gth VARNAME,VARNAME,VALUE' where value could be another numeric variable or a number. Stores whether the second variable
&nbsp&nbsp&nbsp&nbsp    is greater than the third (or value) in the first VARNAME (0 or 1 for false/true). Will create an int by default,
&nbsp&nbsp&nbsp&nbsp    but could also be stored into a double
lth (less than):        same as gth, but checks if the second parameter is less than the third, and stores 0 or 1 in the first one
gte (greater or equal): same as gth, but checks if the second parameter is greater than or equal to the third, and stores 0 or 1 in the first one
lte (greater or equal): same as gth, but checks if the second parameter is less than or equal to the third, and stores 0 or 1 in the first one
cmp (compare values):   in the format 'cmp VARNAME,VARNAME,VALUE' where value could be another variable or a value of a matching type to the second variable
&nbsp&nbsp&nbsp&nbsp    cannot mix strings and numerical values/variables, will error. Stores 1 into the first variable if both others are the same value/string
&nbsp&nbsp&nbsp&nbsp    otherwise will store a 0 (false)
and (logical and):      in the format 'and VARNAME,VARNAME,VARNAME' performs a logical && on the second and third variables (will ignore the third if the second is > 0)
&nbsp&nbsp&nbsp&nbsp    and store the result into the first variable. will default to an int, but could also store into a double. Be careful using this with non 0 or 1
&nbsp&nbsp&nbsp&nbsp    variables, as the answer might also not then be a boolean 0/1. 
orr (logical or):       same as and except performs an || operation. bacause variables are static, to && or || to a constant, just use lth or gth
not (logical not):      in the format 'not VARNAME,VARNAME', performs a ! on the second numeric variable and stores it into the first
del (delete variable):  in the format 'del VARNAME,VARNAME,...' deletes 1 or more variables from the current scope, will not error if they are already not declared
val (variable check):   in the format 'val VARNAME,VARNAME', checks to see if the second variable has a value in the current scope (1 or 0 stored into first VARNAME)
rnd (random int):       in the format 'rnd VARNAME,VALUE,VALUE', will store random int between the values(inclusive) into the named variable. Will be an int by default,
&nbsp&nbsp&nbsp&nbsp    but could also be stored into an empty double. The values could also be replaced with int variables.

#### Advanced commands:
for (for loop):         in the format 'for VALUE,VALUE' where either value could be a numeric variable. The first value is the number of iterations, the second is the
&nbsp&nbsp&nbsp&nbsp    number of lines to include in the loop. This can in effect be used as an if, where number of iterations is a 0 or a 1 (true/false)
fun (declare function): in the format 'fun FUNCNAME,VARNAME,VARNAME,...', where the FUNCNAME given is the name of the function, the first variable given is where it will
&nbsp&nbsp&nbsp&nbsp    pull its return value from, and optional other variable names are the paramaters it takes in. (pass by value)
ret (end the function): put this at the bottom of the function to serve as the bounce back into the line after the run was called.
run (run function):     in the format 'run FUNCNAME,VARNAME,VARNAME,...', where FUNCNAME is the function to run, the first variable is where the returned value will be stored
&nbsp&nbsp&nbsp&nbsp    in the current scope, and any other variables given are the parameters to pass
sec (calendar time):    in the format 'sec VARNAME', stores the current time in seconds since Jan 1st 1970 UTC into the given numeric variable. WIll default to int. 
err (throw error):      in the format 'err "message to print out"', prints out user message then ends the program
idx (index str):        in the format 'idx VARNAME,VARNAME,VALUE', where VALUE could be an int variable. Stores a string into VARNAME of the indexed letter in the second variable (starting at 0)
len (length of str):    in the format 'len VARNAME,VARNAME', stores the length of the second variable into the first. Defaults to int, but could also be declared as a double
put (add to stack):     in the format 'put VARNAME', appends the value of the variable to the stack (preserves type)
pop (pops stack):       in the format 'pop', removes the top value fo the stack
top (reads stack):      in the format 'top VARNAME', reads the top of the stack's value into VARNAME. will default VARNAME to the correct type, otherwise could throw an error if different

#### Future Plans:
add in the ability to import functions from other files
full file read/write
add in string concatenation, probably just called cat (out currently accomplishes this, but doesnt store to a variable)
optimize the speed of the interpreter
add more in-depth error catching
custom code highlighting
command line input (line by line interpreter)
impliment things based on user feedback