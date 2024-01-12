Using the C programming language and System V semaphores, write a Linux program that runs in two processes.
One process reads line by line from the file first.txt, and the other from the file second.txt.
These two processes alternately write the read lines to a text file named sum.txt, so that all odd lines come from the first.txt file, and even lines come from the second.txt file.
