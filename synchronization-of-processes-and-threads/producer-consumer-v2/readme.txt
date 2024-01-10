Using the C programming language, write a Linux program that simulates communication between threads using integer buffers.
Thread N0 generates integer data and writes it to buffers B1, B2, and B3. When writing data, thread N0 first checks if there are free positions in buffer B3, and if so, it writes the new data to buffer B3. If buffer B3 is full, it checks if there are free positions in buffer B1. If there are, producer N0 writes the generated data to buffer B1. If buffer B1 is also full, the new data is written to buffer B2 if it has free positions.
If there are no free positions in any buffer, thread N0 waits until a position is freed up in one of the buffers to write a new element.
Threads N1, N2, N3 read data from buffers B1, B2, B3 and display them on the standard output.
Use POSIX semaphores for synchronization between threads.
