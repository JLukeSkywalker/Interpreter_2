# Interpreter_2
Language Jam C++ Attempt

This is a simple programming language meant to teach how to program
as well as to get people used to programming in lower level languages.

In order to run your file or an example, compile or run the code, then type ./main FILENAME
As an example, 
'clang++-7 -pthread -std=c++17 -o main main.cpp'
'./main greeting.snr' 
would compile the interpreter then run that example file.

Syntactically and format-wise this is similar to assembly and fortran as in a command followed by its operands or parameters.
In order to make it a lot easier, this uses named and typed variables, and every command is in the format:
COMMAND param1,param2,param3,....

Every command is 3 letters, and all whitespace is ignored on file read (except in quotes)
So, every command, no matter what it is is 3 letters followed by comma seperated parameters (if any)

Below is a list of available commands and what they do / how to use them

com (comment):          any text after the com will not be read by the interpreter
int (create integer):   in the format 'int NAME' or 'int NAME,VALUE', created an integer with name NAME and optional value VALUE
dbl (create double):    in the format 'dbl NAME' or 'dbl NAME,VALUE', created a double with name NAME and optional value VALUE
str (create string):    in the format 'str NAME' or 'dbl NAME,VALUE', created a string with name NAME and optional value VALUE
                        pretty much anything can be put into a string except ". If things like escape characters and spaces are
                        required, use quotes to surround it as in str testString. "Hello World!\n"
