# Second Project

## Running the Project

### Bash Script 
In `Syspro_2/bin/` run: 
`./create_infiles.sh inputFile input_dir numFilesPerDirectory`
- inputFile = The file with the records.
- input_dir = Name of the main file.
- numFilesPerDirectory = How many files each sub-directory (country) will have.

### Makefile
In `Syspro_2/bin/` run `make` to create the executable.

### Running the executable
In `Syspro_2/bin/` run: 
`./travelMonitor –m numMonitors -b bufferSize -s sizeOfBloom -i input_dir`
- numMonitors = Number of child processes to be created.
- bufferSize = The size of the buffer used by named pipes.
- sizeOfBloom = The size of the bloom filter in bytes.
- input_dir = The name of the folder the script created.

## Project 1
 Almost all the code from the first project has been utilized in order to implement many of the
 similar concepts that were required for the second project. To store the citizens, skip lists
 were used.

## Classes
 In the second project there were some class additions, that can be seen in "src/process".
 Specifically:
### monitor_db
 This class is used by travelMonitor in order to store all the important data about each
 monitor. It stores information like the monitor pids, their pipes etc.
### folder_records
 This class is designed to keep the files that have been read and registered to the
 Monitor structures. It is used in the process of checking for new files.
### folder_db
 This class is designed to keep a list of folder_records. This is usefull when a monitor
 operates on more than one countries.
### date_list
 This is a list that stores all the dates, for which travel requests have been made. It
 stores each date with its virus(the virus of the request) and if it was accepted or
 denied. If a date-virus combination exists already, it is found and the new request is
 registered.
### country_records
 This class is used by Monitors in order to store the date lists and match them with a
 country.

## Signals
- `SIGCHLD`: If a child process suddenly terminates, the parent process recognises this signal and it restarts it.
- `SIGINT` and `SIGQUIT`: When the parent process receives this signal, it must finish executing the current work-load and then send a `SIGKILL` signal to all the child processes to terminate them. When a child process terminates it outputs a log-file named `log_file.x` where x = pid. The log-file contains all the travels that were requested at the specific child process, the countries it contains and how many of them were accepted and rejected. 

## Functions
All  the functions from the first project are in src/functions. The new functions  that control the communication with  the pipes are in the  src/process_functions.  The signal functions  are located in that folder as well.

## Misc
 - sleep() is used twice for output reasons and only once for synchonization. The reason it is
 used like that is because the pipe need to be "cleaned" in the Monitor process before it gets
 accesed by the travelMonitor process.
 - The program was tested on the university systems as well as on Ubuntu 20.04.
 g++ (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0.
 - If the command line input is wrong, the user can type "./travelMonitor -h" and a help
 box will appear.
