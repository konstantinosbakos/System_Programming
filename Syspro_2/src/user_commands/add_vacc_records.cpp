#include <signal.h>
#include <unistd.h>

#include "user_commands.hpp"

void add_vaccination_records(int buffer_size, int num_of_monitors, char ***pipe_array, long bloom_size, std::string *input_array, monitor_db **monitor_array){
    //A function used by travelMonitor in order to signal a specific monitor that there are new records to be read.
    monitor_db  *monitor_with_country = NULL;
    std::string  proceed = "";

    for(int i=0; i<num_of_monitors; i++){                                //Find the monitor that has the specific country.
        int          num_of_monitor_countries = 0;
        std::string *monitor_countries        = monitor_array[i]->get_countries(num_of_monitor_countries);

        for(int j=0; j<num_of_monitor_countries; j++){
            if(input_array[1] == monitor_countries[j]){
                monitor_with_country = monitor_array[i];
             }
         }
     }

    if(!monitor_with_country){                                           //If country does not exist, print error and return.
        std::cout << "~> Error: Country does not exist." << std::endl;
        return;
     }

    kill(monitor_with_country->get_monitor_pid(),SIGUSR1);               //Send the signal.
    sleep(1);

    receive_message(buffer_size,monitor_with_country->get_pipe_write(),monitor_with_country->get_pipe_read(),proceed);//Receive a message that extra files do exist.

    if(proceed == "PROCEED"){                                            //If new files do exist, retrieve the updated bloom filters.
        update_travel_blooms(buffer_size,bloom_size,monitor_with_country);
        std::cout << "Updated." << std::endl;
     }
    else{                                                                //Else print error.
        std::cout << "~> Error: No new records for the country given." << std::endl;
     }
}

void add_vaccination_records_monitor(int buffer_size, int *pipe, folder_db *monitor_folders, long bloom_size, skip_list ***vaccine_skip_l, bloom_filter **vacced_bloom, skip_list *information_check, data_base *citizen_info, citizen *Sentinel_Dummy){
    //A function used by Monitor in order to receive and handle a signal for extra files.
    int             flag        = 0;
    folder_records *temp_fd_rec = monitor_folders->get_folder_list();

    while(temp_fd_rec){                                                  //Find if new files do exist.
        if(temp_fd_rec->update_file_array()){                            //If they do, update the current file array of the specific country.
            int          num_of_not_reg       = 0;
            std::string *not_registered_files = temp_fd_rec->get_not_registered(num_of_not_reg);

            flag = 1;
            send_message(buffer_size,pipe[MON_W],pipe[MON_R],"PROCEED"); //Send a message that files were found.

            temp_fd_rec->set_all_registered();
            //Add the new records to the structures.
            add_new_records(num_of_not_reg,temp_fd_rec->get_folder(),not_registered_files,bloom_size,vaccine_skip_l,vacced_bloom,information_check,citizen_info,Sentinel_Dummy);
            //Send the updated bloom_filters to the travelMonitor.
            send_bloom_filter(buffer_size,bloom_size,citizen_info->get_vrs_num(),pipe[MON_R],pipe[MON_W],getpid(),vacced_bloom);
         }
        temp_fd_rec = temp_fd_rec->get_next();
     }

    if(!flag){                                                           //If no new files were found, return false.
        send_message(buffer_size,pipe[MON_W],pipe[MON_R],"DO NOT PROCEED");
     }
}