# Third Project

## Running the Project

### Makefile

In `Syspro_3/bin/` run `make` to create the executable.

### Running the executable

In `Syspro_3/bin/` run:
`./travelMonitorClient –m numMonitors -b socketBufferSize -c cyclicBufferSize -s sizeOfBloom -i input_dir -t numThreads`
- numMonitors = Number of child processes.
- socketBufferSize = The size of the buffer used by the sockets.
- cyclicBufferSize = The size of the buffer used by the threads.
- sizeOfBloom = The number of the bloom filter in bytes.
- input_dir = The name of the folder the script from project 2 created.
- numThreads = The number of threads.
 

## Previous projects
 This project uses almost all the code from the previous projects. The similarity of the concept
 of sockets to pipes in trasmitting messages, even allowed for the send and receive functions to be
 used identically (to the ones that were used with the pipes). This time, instead of read/write,
 send/recv is used.

## Classes
   No new classes were needed for this project, only adjustments.

### Sockets
 The sockets where implemented according to the instructions given. The monitors, are the servers and the travelMonitorClient, is the client. This means that the monitors bind the sockets and the
 travelMonitorClient connects to them. The initial socket is set at 8000. The connection is doubly as
 it was in the previous project. This way, the old functions (from project 2) can be used again with
 slight changes.
 The way the IP is found and the sockets are initialized and connected is based on the code and the slides that were provided by the professors.

### Threads
 For the thread implimentation the logic of *Readers-Writer* from the advanced threads lecture was implemented. The main thread is the writer (via using the function thread_writer) and the threads are the readers. 

Firstly all the threads are initiated and then the writer begins. The way the citizen structures are initialized, demands that there be only one thread running at the begining. This means that the first file is initialized alone, while all the other threads are blocked. This does not break the parallel execution of the threads, as it is compulsary (because of the implementation of the previous projects) and thus, unnavoidable. 

After the first file is passed to the structures and the latter are initialized, all the threads work simultaneously, untill no more files are left.

The cyclic_buffer, is indeed cyclic, because each thread takes the next not empty cell of the buffer.

 When all cells of the cyclic_buffer are empty the writer fills them again.

 After there are no more files to be passed to the cyclic_buffer by the writer, the writer exits.

 When the writer exits, two flags are activated, "local_lock" and "flag_main_lock". These flags are
 responsible to freeze the main thread (without busy waiting), untill all the files are processed
 (= empty cyclic_buffer). When the condition is met, the main thread is able to continue.

After the initialization, the threads freeze and they do not exit. If there are new files to be processed,
 the threads are awaken and used again. After that, they freeze again (all this without busy waiting).
 
 At the end, when the program exits, the threads, mutexes and cond variables are all destroyed correctly.

## User Commands
- `/travelRequest [citizenID] [date] [countryFrom] [countryTo] [virusName]`: Same function as in project 2.
 
- `/travelStats [virusName] [date_1] [date_2] [country]`: Same function as in project 2.

- `/addVaccinationRecords [country]`: After the user has placed files in the folder `input_dir/country`, the parent process informs the child process that will be handling the `country`, via the sockets. If new files do exist, the process updates its database and sends through the corresponding named pipes the new bloom filters to the parent process. 
 
- `/searchVaccinationStatus [citizenID]`: Same function as in project 2.
 
- `/printPids`: Prints the Pids of the existing processes.

- `/help`: To print a box with the available commands to the user.

- `/exit` : To exit the program.

## Misc
-  The user may run `./travelMonitorClient -h` for a help box to appear.
 - sleep() is used once for output reasons (it helps prevent the welcome() message of travelMonitor
 not to be printed before or between the "monitor connected" messages).
 - The program was tested on the university systems as well as on Ubuntu 20.04.
 g++ (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0.

