#include <signal.h>
#include <unistd.h>

#include "../src/user_commands/user_commands.hpp"

int   command  = 0;                                                  //Number of input command.
int   no_print = 0;                                                  //Inform that the error message should not be printed.

int main(int argc, char **argv){
    int     buffer_size       = 0;
    int     accepted_req      = 0;                                   //Total accepted requests.
    int     rejected_req      = 0;                                   //Total rejected requests.
    int     continue_loop     = 1;                                   //Flag used to exit program.
    int     num_of_monitors   = 0;
    int     num_of_subfolders = 0;                                   //Number of subfolders (or countries) in the system.
    char ***pipe_array        = NULL;                                //The array of all the pipes that will be used.
    long    bloom_size        = 0;                                   //Bloom size set by user.

    monitor_db  **monitor_array  = NULL;                             //An array of all the Monitors' information that are needed.
    std::string   folder_name;                                       //The name of the main folder given by the user.
    std::string  *subfolders     = NULL;                             //The subfolder array.
    std::string  *monitor_inputs = NULL;                             //The inputs (countries) that each monitor will receive.

    static struct sigaction travel_signal_action;                    //Create a sigaction structure for the signals that will be handled by travelMonitor.
    travel_signal_action.sa_handler = travel_monitor_signal_handler; //Initialize handler.
    sigemptyset(&(travel_signal_action.sa_mask));

    sigaction(SIGINT,&travel_signal_action,NULL);
    sigaction(SIGQUIT,&travel_signal_action,NULL);
    sigaction(SIGCHLD,&travel_signal_action,NULL);

    if(!init_program(argc,argv,num_of_monitors,buffer_size,bloom_size,folder_name)){                              //Read user input.
        return EXIT_SUCCESS;                                                                                      //If command line imput is not correct exit program.
     }

    std::cout << "\nInitializing...\n" << std::endl;

    init_directories();                                                                                           //Initialize the directories that will be used.

    if(!process_subfolders(num_of_monitors,num_of_subfolders,folder_name,&subfolders,&monitor_inputs)){           //Process all the subfolders (countries) and create the monitor inputs.
        return EXIT_SUCCESS;
     }
    
    create_named_pipes(&pipe_array,num_of_monitors);                                                              //Create the named pipes that will be used.

    if(!init_named_pipes(&pipe_array,num_of_monitors)){                                                           //Initialize the named pipes.
        delete_travel_monitor_structures(num_of_monitors,pipe_array,subfolders,monitor_inputs);
        return EXIT_SUCCESS;
     }

    if(!init_monitors(buffer_size,num_of_monitors,bloom_size,getpid(),pipe_array,monitor_inputs,&monitor_array)){ //Initialize the monitors.
        delete_travel_monitor_structures(num_of_monitors,pipe_array,subfolders,monitor_inputs,monitor_array);
        return EXIT_SUCCESS;
     }

    get_travel_blooms(buffer_size,bloom_size,num_of_monitors,monitor_array);                                      //Get the bloom filters form the Monitors.

    if(receive_ready_check(buffer_size,num_of_monitors,monitor_array)){
        std::cout << "Initialized: Parent Process [" << getpid() << "]\n" << std::endl;
     }
    else{
        terminate(buffer_size,accepted_req,rejected_req,num_of_monitors,num_of_subfolders,subfolders,monitor_array);
        continue_loop = 0;
     }

    welcome();

    while(continue_loop){
        int          command_num  = 0;
        int          num_of_words = 0;    //Number of words of input.
        int          current_request;
        std::string *input_array  = NULL; //The array of the user input "broken" into words

        get_command(command_num,&input_array,num_of_words);

        if(command != 0){                 //If a signal command is given, pass it.
            command_num = command;
         }

        switch(command_num){
            case CHLD_CRASH:              //Handles child crashes.
                sigprocmask(SIG_BLOCK,&travel_signal_action.sa_mask,NULL);
                restart_child(buffer_size,num_of_monitors,pipe_array,bloom_size,monitor_array,monitor_inputs);
                command = 0;
                break;
            case TRVL_RQ:                 //Handles travel requests.
                sigprocmask(SIG_BLOCK,&travel_signal_action.sa_mask,NULL);
                current_request = travel_request_send(buffer_size,num_of_monitors,input_array,monitor_array);
                if(current_request == 1){ //If the request was seen through, record it.
                    accepted_req++;
                 }
                else if(current_request == 0){
                    rejected_req++;
                 }
                delete [] input_array;
                input_array = NULL;
                break;
            case TRVL_STS:                //Handles travel statistics requests.
                sigprocmask(SIG_BLOCK,&travel_signal_action.sa_mask,NULL);
                travel_stats(buffer_size,num_of_words,num_of_monitors,input_array,monitor_array);
                delete [] input_array;
                input_array = NULL;
                break;
            case ADD_VAC_REC:             //Handles new records' additions.
                sigprocmask(SIG_BLOCK,&travel_signal_action.sa_mask,NULL);
                add_vaccination_records(buffer_size,num_of_monitors,pipe_array,bloom_size,input_array,monitor_array);
                delete [] input_array;
                input_array = NULL;
                break;
            case SRCH_VAC_STS:            //Handles ID search.
                sigprocmask(SIG_BLOCK,&travel_signal_action.sa_mask,NULL);
                search_vaccination_status(buffer_size,num_of_monitors,input_array,monitor_array);
                delete [] input_array;
                input_array = NULL;
                break;
            case PRINT_PIDS:              //Prints Monitors' pids.
                sigprocmask(SIG_BLOCK,&travel_signal_action.sa_mask,NULL);
                print_monitor_pids(num_of_monitors,monitor_array);
                delete [] input_array;
                input_array = NULL;
                break;
            case HELP:                    //Prints help message.
                sigprocmask(SIG_BLOCK,&travel_signal_action.sa_mask,NULL);
                help();
                delete [] input_array;
                input_array = NULL;
                break;
            case TERMINATE:               //Terminates program.
                sigprocmask(SIG_BLOCK,&travel_signal_action.sa_mask,NULL);
                terminate(buffer_size,accepted_req,rejected_req,num_of_monitors,num_of_subfolders,subfolders,monitor_array);
                continue_loop = 0;        //break loop.
                delete [] input_array;
                input_array   = NULL;
                break;
            default:
                std::cout << "Error: Wrong format. Please type \"/help\"." << std::endl;
                delete [] input_array;
                input_array = NULL;
                break;
         }
        sigprocmask(SIG_UNBLOCK,&travel_signal_action.sa_mask,NULL);
     }

    delete_travel_monitor_structures(num_of_monitors,pipe_array,subfolders,monitor_inputs,monitor_array);

    return EXIT_SUCCESS;
}