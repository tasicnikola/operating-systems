Using the C programming language, create three Linux processes that utilize shared memory of size 1024 bytes for mutual communication.
The first process fills the first 512 bytes of shared memory with randomly selected letters (in the range a-z).
Subsequently, the second process fills the last 512 bytes of shared memory with arbitrarily chosen digits.
Once the second process completes the data generation, the third process writes the entire content of the shared memory into a file.
This sequence of actions repeats periodically every 15 seconds.
