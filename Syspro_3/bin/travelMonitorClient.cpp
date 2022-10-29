#include "../src/user_commands/user_commands.hpp"

int main(int argc, char **argv){
    int  accepted_req       =  0;        //Total accepted requests.
    int  rejected_req       =  0;        //Total rejected requests.
    int  continue_loop      =  1;        //Flag used to exit program.
    int  num_of_threads     =  0;
    int  num_of_monitors    =  0;
    int  num_of_subfolders  =  0;        //Number of subfolders (or countries) in the system.
    int  cyclic_buffer_size =  0;
    int  socket_buffer_size =  0;
    long bloom_size         =  0; 

    monitor_db  **monitor_array  = NULL; //An array of all the Monitors' information that are needed.
    std::string  *subfolders     = NULL; //The subfolder array.
    std::string   folder_name;           //The name of the main folder given by the user.
    std::string  *monitor_inputs = NULL; //The inputs (countries) that each monitor will receive.

    if(!init_travel_monitor_client(argc,argv,num_of_monitors,socket_buffer_size,cyclic_buffer_size,bloom_size,folder_name,num_of_threads)){ //Read user input.
        return EXIT_SUCCESS;                                                                                      //If command line imput is not correct exit program.
     }

    std::cout << "\nConnecting...\n" << std::endl;

    init_directories();                                                                                           //Initialize the directories that will be used.

    if(!process_subfolders(num_of_monitors,num_of_subfolders,folder_name,&subfolders,&monitor_inputs)){           //Process all the subfolders (countries) and create the monitor inputs.
        return EXIT_SUCCESS;
     }

    if(!init_monitors(num_of_threads,num_of_monitors,socket_buffer_size,cyclic_buffer_size,bloom_size,monitor_inputs,&monitor_array)){ //Initialize the monitors.
        delete_travel_monitor_structures(num_of_monitors,subfolders,monitor_inputs,monitor_array);
        return EXIT_SUCCESS;
     }

    get_travel_blooms(socket_buffer_size,bloom_size,num_of_monitors,monitor_array);                               //Get the bloom filters form the Monitors.

    if(!receive_ready_check(socket_buffer_size,num_of_monitors,monitor_array)){
        terminate(socket_buffer_size,accepted_req,rejected_req,num_of_monitors,num_of_subfolders,subfolders,monitor_array);
        continue_loop = 0;
     }

    welcome();

    while(continue_loop){
        int          command_num  = 0;
        int          num_of_words = 0;    //Number of words of input.
        int          current_request;
        std::string *input_array  = NULL; //The array of the user input "broken" into words

        get_command(command_num,&input_array,num_of_words);

        switch(command_num){
            case TRVL_RQ:                 //Handles travel requests.
                current_request = travel_request_send(socket_buffer_size,num_of_monitors,input_array,monitor_array);
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
                travel_stats(socket_buffer_size,num_of_words,num_of_monitors,input_array,monitor_array);
                delete [] input_array;
                input_array = NULL;
                break;
            case ADD_VAC_REC:             //Handles new records' additions.
                add_vaccination_records(socket_buffer_size,num_of_monitors,bloom_size,input_array,monitor_array);
                delete [] input_array;
                input_array = NULL;
                break;
            case SRCH_VAC_STS:            //Handles ID search.
                search_vaccination_status(socket_buffer_size,num_of_monitors,input_array,monitor_array);
                delete [] input_array;
                input_array = NULL;
                break;
            case HELP:                    //Prints help message.
                help();
                delete [] input_array;
                input_array = NULL;
                break;
            case TERMINATE:               //Terminates program.
                terminate(socket_buffer_size,accepted_req,rejected_req,num_of_monitors,num_of_subfolders,subfolders,monitor_array);
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
     }

    delete_travel_monitor_structures(num_of_monitors,subfolders,monitor_inputs,monitor_array);

    return EXIT_SUCCESS;
}