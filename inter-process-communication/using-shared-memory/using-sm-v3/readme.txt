Using the C programming language, create a program that copies a specified file.
The first command line argument is the name of the original file, and the second argument is the name of the new file, which will be a copy of the first one.
The copying is done by dividing the program into two processes. The first process reads a certain number of bytes from the file and sends them to the second process.
The second process writes the bytes from shared memory into the new file.
