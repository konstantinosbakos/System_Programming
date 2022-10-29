#include <signal.h>
#include <unistd.h>

#include "../src/user_commands/user_commands.hpp"

int command  = -1;                                                                     //Number of input command.
int no_print =  0;                                                                     //Inform that the error message should not be printed.

int main(int argc, char **argv){
    srand(time(0));

    int         *pipe           = new int[3];                                          //The array where the pipe file descriptors will be stored.
    int          buffer_size    = 0;
    int          continue_loop  = 1;                                                   //Flag used to exit program.
    int          num_of_folders = 0;                                                   //Number of folders (countries) that the Monitor will control.
    char        *pipe_names[]   = {argv[1],argv[2]};                                   //The pipe names.
    long         bloom_size     = 0;
    std::string  folder_names   = "";

    static struct sigaction monitor_signal_action;                                     //Create a sigaction structure for the signals that will be handled by Monitor.
    monitor_signal_action.sa_handler = monitor_signal_handler;                         //Initialize handler.
    sigfillset(&(monitor_signal_action.sa_mask));

    sigaction(SIGINT,&monitor_signal_action,NULL);
    sigaction(SIGQUIT,&monitor_signal_action,NULL);
    sigaction(SIGUSR1,&monitor_signal_action,NULL);

    init_monitor_basics(buffer_size,bloom_size,&pipe,pipe_names,folder_names);         //Initialize the basic monitor structures.

    std::string      *folder_array      = split_to_words(folder_names,num_of_folders); //The folder array contains all the folders (countries) that the Monitor will control.
    folder_db        *monitor_folders   = new folder_db(num_of_folders,folder_array);  //The data base of the Monitor's folders.
    country_records **monitor_countries = new country_records*[num_of_folders];        //The monitor's countries data base.

    for(int i=0; i<num_of_folders; i++){                                               //Initialize the monitor's countries' data bases.
        monitor_countries[i] = new country_records(get_folder_country(folder_array[i]));
     }

    citizen        *Sentinel_Dummy      = new citizen;                                 //A "dummy" citizen with ID -1 to act as the citizen of all sentinels.
                    Sentinel_Dummy->set_ID(-1);                                        //Initialize Sentinel Dummy.
    data_base      *citizen_info        = new data_base();                             //Initialize Data Base.
    skip_list    ***vaccine_skip_l      = NULL;                                        //The Monitor skip lists.
    skip_list      *information_check   = new skip_list(0,NULL,Sentinel_Dummy);        //Used to confirm that the new citizen's info are in accordance to the info that are already in the system, if there are any.
    bloom_filter  **vacced_bloom        = NULL;

    init_monitor_citizen_structures(bloom_size,&vaccine_skip_l,&vacced_bloom,information_check,citizen_info,Sentinel_Dummy,monitor_folders); //Initialize all the structures that store the information regarding the citizens.

    send_bloom_filter(buffer_size,bloom_size,citizen_info->get_vrs_num(),pipe[MON_R],pipe[MON_W],long(getpid()),vacced_bloom);               //Send the monitor's bloom filters.

    if(send_ready_check(buffer_size,continue_loop,pipe[MON_W],pipe[MON_R])){
        std::cout << "Initialized: Child  Process [" << getpid() << "]" << std::endl;
     }
    else{
        std::cout << "~> Error: Cannot initialize process " << getpid() << ". The process is now exiting." << std::endl;
        return EXIT_SUCCESS;
     }

    while(continue_loop){
        if(command == -1){     //If no signal command is given.
            std::string str_command = "";
            receive_message(buffer_size,pipe[MON_W],pipe[MON_R],str_command);

            if(str_command != ""){
                command = std::stoi(str_command);
             }
         }

        switch (command){
            case TRVL_RQ:      //Handles outgoing travel requests.
                travel_request_receive(buffer_size,pipe[MON_W],pipe[MON_R],vaccine_skip_l,information_check,citizen_info);
                break;
            case COUNTRY_TO_M: //Handles incoming travel requests.
                add_to_destination(buffer_size,num_of_folders,pipe[MON_W],pipe[MON_R],citizen_info->get_vrs_ptr(),monitor_countries);
                break;
            case TRVL_STS:     //Handles travel statistics requests.
                travel_stats_monitor(buffer_size,num_of_folders,pipe[MON_W],pipe[MON_R],monitor_countries);
                break;
            case ADD_VAC_REC:
                clear_pipe(pipe[MON_CLR],buffer_size);
                add_vaccination_records_monitor(buffer_size,pipe,monitor_folders,bloom_size,vaccine_skip_l,vacced_bloom,information_check,citizen_info,Sentinel_Dummy);
                break;
            case SRCH_VAC_STS: //Handles ID search.
                search_vaccination_status_monitor(buffer_size,citizen_info->get_vrs_num(),pipe[MON_W],pipe[MON_R],vaccine_skip_l,information_check);
                break;
            case EXPORT_LOG:   //Handles Monitor log_file exporting.
                clear_pipe(pipe[MON_CLR],buffer_size);
                export_monitor_log(num_of_folders,folder_array,monitor_countries);
                break;
            case TERMINATE:    //Terminates Monitor.
                export_monitor_log(num_of_folders,folder_array,monitor_countries);
                continue_loop = 0;
                break;
            default:
                std::cout << "Error: Wrong format. Please type \"/help\"." << std::endl;
                break;
         }
        command = -1;
     }

    delete_monitor_structures(num_of_folders,pipe,monitor_folders,folder_array,monitor_countries);
    delete_monitor_citizen_structures(citizen_info,vaccine_skip_l,vacced_bloom,information_check,Sentinel_Dummy);

    return EXIT_SUCCESS;
}