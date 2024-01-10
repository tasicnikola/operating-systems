Access to the database is performed for writing and reading by multiple processes.
At any given moment, there may be multiple processes reading the content from the database using the read_database() procedure.
However, if one process is writing content to the database using the write_database() procedure, no other process is allowed access to the database for writing or reading.
Processes that read content take precedence, so as long as there are processes reading from the database, the process that needs to write data must wait.
Using the C programming language, write a Linux program that simulates the previously described algorithm using processes and known IPC mechanisms. (Synchronization problem Readers-Writers).
