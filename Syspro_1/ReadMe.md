	                                __________________________________
                                   |-First Project System Programming-|
                                   |----------------------------------|
                                   |--------Konstantinos Bakos--------|
                                   |------------07-04-2021------------|
                                    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Running the Project

### Bash Script

In `Syspro_1/bin/` run:
`./testFile.sh viruses.txt countries.txt X Y`
- X = Number of lines
- Y = Allow duplicate citizens (0 or 1)
Producing more than 10.000 lines without  duplication is not  allowed, because 0 <= Citizen_ID <= 9999.

### Makefile

In `Syspro_1/bin/` run `make` to create the executable.

### Running the executable

In `Syspro_1/bin/` run:
`./vaccineMonitor -c input.txt -b sizeOfBloom`
- sizeOfBloom = the size of the bloom filter in bytes.

## Bloom Filter
The bloom filter was implemented as instructed, via using the hashing functions provided. It is located in /src/bloom_filter. The way it works is the following:
 
It consists of one class, bloom_filer, where the array size and the bloom filter array are stored,
as well as the virus that the patients have been vaccinated for. The user inserts the size of the bloom filte in bytes. That size then becomes the size of the bloom filter array. Each cell in this array is considered to be a byte. At the initialization, everything becomes 0. After that, the two functions look_array() and update_array() are responsible for passing or retrieving information from the array. They are used from bloom_filter_search() and from bloom_filter_insert(), respectively. Both of latter the functions pass and retrieve K(=16) bits. 

## Skip List
 The skip list was implemented based on the video and the information provided. The skip list 
 implementation is located in src/skip_list.The skip list is implemented using two classes, block 
 and skip_list.
### Class : block
 A block is essentially a block of information. It contains the number of levels that it participates 
 in the skip list, an array for the pointer of each level and the citizen pointer.
### Class : skip_list
 A skip list contains many blocks. It arrangers the way the blocks are connected and how many levels
 the structure should have based on the number of the blocks. For achieving the best efficiency, a
 skip list changes levels when enough elements are added or substracted. The number of levels is
 decided by the number int(ceil(log2(block_num))) which is a round up to the powers of 2 (eg. if block_num = 20, int(cel(log2(block_num))) = ceil(4.3) = 5, so the skip list will have 5 levels.) The ground level (0) is not counted as a level (so in the example above, there would be the zero level and 5 more levels).
 Inserting and searching in the list are two functions that are both implemented at O(log2(n))
 time.
 There is also one more basic function; delete. It is implemented in O(log2(n)) time as well.
 All of the above can be found in src/skip_list/skip_list_public.cpp

## Data Base
 Data_base is implemented using 6 classes; name, surname, virus, country, citizen and data_base. These classes can be found in /src/data_base.
 ### Class virus/country/name/surname
 These functions are used in order to avoid data duplication. If a citizen with an existing
 virus/country/name/surname on the list is created, he is appointed the node of the list with the 
 existing name, otherwise, a new node is created with the new name.
### Class citizen
 This class cointains all the information regarding a citizen. The ID and age are implemented as an
 int because there is no point to have a list of IDs or ages (int = 8 bytes, int* = 8 bytes), the date is   stored as a char* array, rathen than a string because it is more memory efficient. The other information about the citizen are stored as pointers to the above class nodes.
 ### Class data_base:
 This is essentially the data_base of the program. All the citizen information are stored here, as
 well as the citizens themselves. There is a list for all of the above mentioned classes. The citizen
 list is a doubly linked list, in order to delete citizens in O(1) time. If it was not a doubly
 linked list it could have a significant impact on the initialization time of the program, as well as
 on other functions.
 
## User Commands
    
 /src/user_commands
 
- `/vaccineStatusBloom [citizenID] [virusName]`: To check if the  citizen with `citizenID` is vaccinated against the virus `virusName`, via the bloom filter (returns `Maybe` or `not vaccinated`).

- `/vaccineStatus [citizenID] [virusName]`:  To check if the  citizen with `citizenID` is
   vaccinated against the virus `virusName`, via the skip list (returns `Maybe` or `not vaccinated`).

- `/vaccineStatus [citizenID]`: To check for which viruses a citizen is vaccinated.

- `/populationStatus [country] [virusName] [date_1] [date_2]`: To check the number of citizens that were vaccinated in the time window given. If the `country` is not provided, statistics for every country will be printed.  If `date_1` is provided, `date_2` must be provided as well. 

- `/popStatusByAge [country] [virusName] [date_1] [date_2]`: To check the number of citizens vaccinated per age group (0-20, 21-40, 41-60, 60+). If the `country` is not provided, statistics for every country will be printed. If `date_1` is provided, `date_2` must be provided as well. 

- `/insertCitizenRecord [citizenID] [firstName] [lastName] [country] [age] [virusName] [YES/NO] [date]`: To insert a new citizen and/or to update their vaccine status about `virusName`. If the `date` is not specified, it is assumed that the vaccination took place today.

- `/vaccinateNow [citizenID] [firstName] [lastName] [country] [age] [virusName]`: To update the vaccine status of `citizenID` about the virus `virusName`. If the citizen is already vaccinated return the vaccination date.

- `/list-nonVaccinated-Persons [virusName]`: To check who are the citizens not vaccinated against  `virusName`.

- `/system-viruses`: To show the user what viruses are in the database.

- `/help`: To print a box with the available commands to the user.

- `/exit` : To exit the program.

## Functions
 /src/functions
 
 This is a general collection of functions that are used by all the structures in order to create both a
 level of abstractness as well as a better understandment of the way the program functions. They are
 divided in bool functions, void functions and functions (that have a return type other than bool).

## Misc
 • The initialization errors are stored in Initialization_errors.txt which can be accessed right after
 the initialization. If the file exists already, it is deleted when the program runs again.
 • There may be many citizens with the same ID in the data base citizen list, but each citizen refers to
 a different virus and/or a different state (YES/NO).
 • information_check is a skip list tha contains all the citizens in every other skip list, at least
 once. Its responsibility is to identify and disallow any new citizen with mismatching information
 from being inserted in the list. After every deletion of a citizen from the data base, it is
 important to delete the citizen with that ID from the information_check skip list as well, in
 case it is the one that is being deleted at the data base. After that an existing citizen with the
 same ID will be inserted in the information_check skip list.
 • To delete all .o files, input.txt and Initialization_errors.txt, type "make clean".
 • The program was tested on the university systems as well as on Ubuntu 20.04.
 g++ (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0.
 • If the command line input is wrong, the user can type "./vaccineMonitor -h" and a help box will
 appear.
 • If the command line imput is wrong for the bash scrip, again, a help box will appear.
