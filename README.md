214-pa1
=======

Assignment 1: Tokenizer

###Version
1.0

###Introduction
In this assignment, you will practice programming with C pointers. Much of the pointer manipulation
will come in the form of operating on C strings, although you will be dealing with some
pointers to structs as well.
Your task is to write a type and a set of functions, in essence, the equivalent of a Java class that
implements a tokenizer. The tokenizer should accept two strings as command-line arguments, the
first of which will contain a set of separator characters while the second will contain a set of tokens
separated by one or more separator characters. The tokenizer should return the tokens in the
second string one token at a time, hence your program is called a tokenizer

###Invocation
gcc tokenizer tokenizer.c
./tokenizer <tokens> <string>
