#include "user_commands.hpp"
#include "../threads/threads.hpp"

void add_vaccination_records_monitor(int socket_buffer_size, int socket_write, int socket_read, int num_of_folders, folder_db **monitor_folders){
    //A function used by Monitor in order to receive and handle a signal for extra files.
    int             flag        = 0;
    std::string     country     = "";
    folder_records *temp_fd_rec = (*monitor_folders)->get_folder_list();

    receive_message(socket_buffer_size,socket_write,socket_read,country);

    while(temp_fd_rec){                            //Find if new files do exist.
        if(temp_fd_rec->get_country() == country){ //If they do, update the current file array of the specific country.
            if(temp_fd_rec->update_file_array()){
                flag = 1;
                send_message(socket_buffer_size,socket_write,socket_read,"PROCEED"); //Write a message that files were found.

                write_bool = false;                //Τo unblock the threads.
                cond_broadcast(&cond_read);        //To wake up the threads.
                thread_write(num_of_folders,monitor_folders);

                while(flag_main_lock){             //Wait for the signal that it is okay to proceed.
                    cond_wait(&main_lock,&main_lock_mutex);
                }
                write_bool = true;                 //Τo block the threads again.
                
                //Write the updated bloom_filters to the travelMonitor.
                send_bloom_filter(socket_buffer_size,bloom_size,citizen_info->get_vrs_num(),socket_write,socket_read,vacced_bloom);
             }
         }
        temp_fd_rec = temp_fd_rec->get_next();
     }

    if(!flag){  //If no new files were found, return false.
        send_message(socket_buffer_size,socket_write,socket_read,"DO NOT PROCEED");
     }
}