Using the C programming language, write a UNIX/Linux program that simulates the producer/consumer problem using message queues.
The main program is divided into two processes.
The first process (producer) generates N random positive integers and sends them to the second process.
The value of N is determined during execution, also as a random positive integer. After sending the numbers, the first process sends -1 as an end code.
The second process (consumer) retrieves the sent numbers from the messages and prints them to the standard output.
