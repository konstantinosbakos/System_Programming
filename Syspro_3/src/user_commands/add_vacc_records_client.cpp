#include "user_commands.hpp"

void add_vaccination_records(int socket_buffer_size, int num_of_monitors, long bloom_size, std::string *input_array, monitor_db **monitor_array){
    //A function used by travelMonitor in order to signal a specific monitor that there are new records to be read.
    monitor_db  *monitor_with_country = NULL;
    std::string  proceed              = "";
    std::string  command              = std::to_string(ADD_VAC_REC);

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

    send_message(socket_buffer_size,monitor_with_country->get_socket_write(),monitor_with_country->get_socket_read(),command);
    send_message(socket_buffer_size,monitor_with_country->get_socket_write(),monitor_with_country->get_socket_read(),input_array[1]); //Send the country the user gave.
    receive_message(socket_buffer_size,monitor_with_country->get_socket_write(),monitor_with_country->get_socket_read(),proceed);     //Receive a message that extra files do exist.

    if(proceed == "PROCEED"){                                            //If new files do exist, get the updated bloom filters.
        update_travel_blooms(socket_buffer_size,bloom_size,monitor_with_country);
        std::cout << "Updated." << std::endl;
     }
    else{                                                                //Else print error.
        std::cout << "~> Error: No new records for the country given." << std::endl;
     }
}
