Using the C programming language, write a Linux program that contains a buffer capable of holding two integer numbers.
A separate thread periodically generates two numbers (within the range of 1 to 10) and writes them into this buffer.
When a new pair of numbers is found in the buffer, the main thread (main function) should determine their sum and print it to the standard output.
The time interval between two consecutive writes to the buffer is a random value between 0 and 5 seconds.
