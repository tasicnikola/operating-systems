Using the C programming language, write a Linux program that creates two additional processes.
The parent process and these two newly created child processes are connected by pipes. The parent process accepts sentences entered by the user from the keyboard, and using pipes, it sends each sentence to the first child process.
This process checks whether the first character in the sentence is an uppercase letter, and if it is not, it converts it to the corresponding uppercase letter.
The modified sentence is then sent to the second child process using a pipe. The next process checks if the last character in the sentence is a period (.). If it is not, it adds a period and prints the sentence to the standard output.

Modify the solution from the task above so that the three processes are connected in a circular manner using pipes.
The second child process no longer prints the sentence but instead sends it via a pipe to the parent process, which then prints it to the standard output without any modification.
