#include <netdb.h>
#include <unistd.h>

#include "../src/threads/threads.hpp"
#include "../src/user_commands/user_commands.hpp"

int          cyclic_buffer_size   = 0;
bool         write_bool           = false;                                   //Used to signal that the thread_writer(i.e. the main thread) is filling the buffer.
bool         flag_main_lock       = false;
bool         continue_loop_thread = true;                                    //Is used to "shut down" (when = false) the threads.
long         bloom_size           = 0;

citizen        *Sentinel_Dummy    = new citizen;                             //A "dummy" citizen with ID -1 to act as the citizen of all sentinels.
data_base      *citizen_info      = new data_base();                         //Initialize Data Base.
skip_list    ***vaccine_skip_l    = NULL;                                    //The Monitor skip lists.
skip_list      *information_check = NULL;                                    //Used to confirm that the new citizen's info are in accordance to the info that are already in the system, if there are any.
std::string    *cyclic_buffer;
bloom_filter  **vacced_bloom      = NULL;

pthread_cond_t cond_lock  = PTHREAD_COND_INITIALIZER;                        //Used in order to ensure that the first file (via the threads) will be initialized alone (only one thread will run/ please see ReadMe for more info).
pthread_cond_t cond_read  = PTHREAD_COND_INITIALIZER;                        //Used to signal that the readers can read.
pthread_cond_t main_lock  = PTHREAD_COND_INITIALIZER;                        //Initiate a lock on the main thread in order to wait for all the other threads to finish updating the structures.
pthread_cond_t cond_write = PTHREAD_COND_INITIALIZER;                        //Used to signal that the writer must write (because of the empty buffer).

pthread_mutex_t mutex_threads                  = PTHREAD_MUTEX_INITIALIZER;  //The (only) mutex used by threads.
pthread_mutex_t main_lock_mutex                = PTHREAD_MUTEX_INITIALIZER;  //Used to lock the main thread.
pthread_mutex_t insert_citizen_mutex           = PTHREAD_MUTEX_INITIALIZER;  //Used in init_monitor_structures.cpp
pthread_mutex_t insert_citizen_new_virus_mutex = PTHREAD_MUTEX_INITIALIZER;  //Used in init_monitor_structures.cpp

int main(int argc, char **argv){
    srand(time(0));

    int   port               = -1; //The port(s) that the server will use (port and (port + 1)).
    int  *sockets;                 //The array for storing the values of the sockets (for send and recv).
    int  *socket_id;               //The ids of socket() that listen will be done upon.
    int   continue_loop      =  1; //Flag used to exit program.
    int   num_of_folders     =  0; //Number of folders (countries) that the Monitor will control.
    int   num_of_threads     =  0;
    int   socket_buffer_size =  0;

    std::string         folder_names =  "";
    struct sockaddr_in *server       = new struct sockaddr_in[2]; //The structues that will be used for initializing the sockets with the correct information.

    init_socket_arrays(&sockets,&socket_id);

    if(!init_monitor_server(argc,argv,port,num_of_threads,socket_buffer_size,cyclic_buffer_size,bloom_size,folder_names)){ //Read user input.
        initialization_delete(Sentinel_Dummy,citizen_info,server);
        return EXIT_SUCCESS;
     }

    if(!init_tcp(SERVER_R,port,&sockets,&socket_id,&server) || !init_tcp(SERVER_W,(port+1),&sockets,&socket_id,&server)){  //Initialize both TCP connections.
        initialization_delete(Sentinel_Dummy,citizen_info,server);
        return EXIT_SUCCESS;
     }

    std::string      *folder_array      = split_to_words(folder_names,num_of_folders); //The folder array contains all the folders (countries) that the Monitor will control.
    folder_db        *monitor_folders   = new folder_db(num_of_folders,folder_array);  //The data base of the Monitor's folders.
    country_records **monitor_countries = new country_records*[num_of_folders];        //The monitor's countries data base.
    pthread_t        *threads           = new pthread_t[num_of_threads];

    for(int i=0; i<num_of_folders; i++){                                               //Initialize the monitor's countries' data bases.
        monitor_countries[i] = new country_records(get_folder_country(folder_array[i]));
     }

    Sentinel_Dummy->set_ID(-1);                               //Initialize Sentinel Dummy.
    information_check = new skip_list(0,NULL,Sentinel_Dummy); //Used to confirm that the new citizen's info are in accordance to the info that are already in the system, if there are any.
    cyclic_buffer     = new std::string[cyclic_buffer_size];

    for(int i=0; i<cyclic_buffer_size; i++){
        cyclic_buffer[i] = "";
     }

    init_cond_mutex_variables();                              //Initialize all the mutexes and cond variables that will be used.

    mutex_lock(&main_lock_mutex);                             //Needs to be locked to be used with cond variables.

    for(int i=0; i<num_of_threads; i++){                      //Initialize the threads
        thread_init(&(threads[i]),&thread_read);
     }

    thread_write(num_of_folders,&monitor_folders);            //Begin the thread_writer (from the main thread).

    while(flag_main_lock){                                    //Wait for the signal that the cyclic buffer is empty.
        cond_wait(&main_lock,&main_lock_mutex);
     }

    write_bool = true;                                        //Block the threads (not busy).

    send_bloom_filter(socket_buffer_size,bloom_size,citizen_info->get_vrs_num(),sockets[SERVER_W],sockets[SERVER_R],vacced_bloom);               //write the monitor's bloom filters.

    if(send_ready_check(socket_buffer_size,continue_loop,sockets[SERVER_W],sockets[SERVER_R])){
        std::cout << "Connection established with: Server [" << getpid() << "]" << std::endl;
     }
    else{
        std::cout << "~> Error: Cannot connect to Server " << getpid() << ". The process is now exiting." << std::endl;
        return EXIT_SUCCESS;
     }

    while(continue_loop){
        int         command     = 0;
        std::string str_command = "";

        receive_message(socket_buffer_size,sockets[SERVER_W],sockets[SERVER_R],str_command);
        command = to_int(str_command);

        switch (command){
            case TRVL_RQ:      //Handles outgoing travel requests.
                travel_request_receive(socket_buffer_size,sockets[SERVER_W],sockets[SERVER_R],vaccine_skip_l,information_check,citizen_info);
                break;
            case COUNTRY_TO_M: //Handles incoming travel requests.
                add_to_destination(socket_buffer_size,num_of_folders,sockets[SERVER_W],sockets[SERVER_R],citizen_info->get_vrs_ptr(),monitor_countries);
                break;
            case TRVL_STS:     //Handles travel statistics requests.
                travel_stats_monitor(socket_buffer_size,num_of_folders,sockets[SERVER_W],sockets[SERVER_R],monitor_countries);
                break;
            case ADD_VAC_REC:  //Handles new record additions.
                add_vaccination_records_monitor(socket_buffer_size,sockets[SERVER_W],sockets[SERVER_R],num_of_folders,&monitor_folders);
                break;
            case SRCH_VAC_STS: //Handles ID search.
                search_vaccination_status_monitor(socket_buffer_size,citizen_info->get_vrs_num(),sockets[SERVER_W],sockets[SERVER_R],vaccine_skip_l,information_check);
                break;
            case EXPORT_LOG:   //Handles Monitor log_file exporting.
                export_server_log(num_of_folders,folder_array,monitor_countries);
                break;
            case TERMINATE:    //Terminates Monitor.
                export_server_log(num_of_folders,folder_array,monitor_countries);
                continue_loop = 0;
                break;
            default:
                std::cout << "Error: Wrong format. Please type \"/help\"." << std::endl;
                break;
         }
     }

    delete_monitor_structures(num_of_folders,sockets,socket_id,monitor_folders,folder_array,monitor_countries,server);
    delete_monitor_citizen_structures(citizen_info,vaccine_skip_l,vacced_bloom,information_check,Sentinel_Dummy);
    delete_threads(num_of_threads,threads,cyclic_buffer);

    return EXIT_SUCCESS;
}
