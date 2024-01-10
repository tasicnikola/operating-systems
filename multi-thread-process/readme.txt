Using the C programming language, create a multi-threaded Linux process. Within the process, three threads (N1, N2, and N3) are executed, accessing a shared resource.
The shared resource is represented by an integer array of 20 elements.
The threads access the shared resource in the following manner:

Thread N1, every 2 seconds, writes a random value to a random position in the first half of the array (positions 0 to 9).
Thread N2, every 4 seconds, writes a random value to a random position in the second half of the array (positions 10 to 19).
Thread N3, every 8 seconds, writes a random value to a random position in the array (positions 0 to 19).
Using a known mechanism for synchronization and mutual exclusion between threads, ensure that thread N3 can access the shared resource only if, at a given moment, neither thread N1 nor thread N2 is accessing the shared resource.
