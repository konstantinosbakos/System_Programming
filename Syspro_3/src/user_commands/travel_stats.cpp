#include "user_commands.hpp"

void travel_stats(int socket_buffer_size, int num_of_words, int num_of_monitors, std::string *input_array, monitor_db **monitor_array){
    //A function used by travelMonitor in order to print travel statistics.
    int          dummy_num      = 0;
    int          virus_exists   = 0;
    std::string  command        = std::to_string(TRVL_STS);
    std::string  message        = "";
    std::string  response       = "";
    std::string *response_array = NULL;

    for(int i=0; i<num_of_monitors; i++){                                                //Confirm that virus does exist.
        if(monitor_array[i]->has_virus(input_array[1])){
            virus_exists = 1;
         }
     }

    if(!virus_exists){                                                                   //If virus does not exist return error.
        std::cout << "~> Error: Virus does not exist in the system." << std::endl;
        return;
    }
    if(!check_date_format(input_array[2]) || !check_date_format(input_array[3])){        //Confirm that the dates' format is correct.
        std::cout << "~> Error: Please enter a valid date format." << std::endl;
        return;
     }
    if(!is_valid_date(input_array[2]) || !is_valid_date(input_array[3])){                //Confirm that the dates are within the expected boundaries.
        std::cout << "~> Error: Please enter a valid date." << std::endl;
        return;
     }


    if(num_of_words == 4){                                                               //If the global statistics are required.
        int accepted_req = 0;
        int rejected_req = 0;

        message = "ALL " + input_array[1] + " " + input_array[2] + " " + input_array[3]; //Construct the message for the Monitors.
        
        for(int i=0; i<num_of_monitors; i++){                                            //write the message to all the monitors.
            send_message(socket_buffer_size,monitor_array[i]->get_socket_write(),monitor_array[i]->get_socket_read(),command);
            send_message(socket_buffer_size,monitor_array[i]->get_socket_write(),monitor_array[i]->get_socket_read(),message);
            
            for(int j=0; j<monitor_array[i]->get_num_of_countries(); j++){               //Receive each Monitor's statistics.
                receive_message(socket_buffer_size,monitor_array[i]->get_socket_write(),monitor_array[i]->get_socket_read(),response);

                response_array = split_to_words(response,dummy_num);

                accepted_req += to_int(response_array[0]);                            //Add all the results together.
                rejected_req += to_int(response_array[1]);

                delete [] response_array;
                response_array = NULL;

                response = "";
             }
         }

        std::cout << "~> Global statistics:    " << std::endl;                           //Print the statistics.
        std::cout << "      Total    Requests: " << accepted_req + rejected_req << "." << std::endl;
        std::cout << "      Accepted Requests: " << accepted_req << "." << std::endl;
        std::cout << "      Rejected Requests: " << rejected_req << "." << std::endl;
    }
    else if(num_of_words == 5){                                                          //If a country is specified.
        monitor_db *monitor_with_country = NULL;
       
        for(int i=0; i<num_of_monitors; i++){                                            //Find the monitor that controls the specific country.
            if(monitor_array[i]->has_country(input_array[4])){
                monitor_with_country = monitor_array[i];
                break;
             }
         }

        if(!monitor_with_country){                                                       //If the country does not exist return error.
            std::cout << "~> Error: Country does not exist." << std::endl;
            return;
         }

        message = "SINGLE " + input_array[1] + " " + input_array[2] + " " + input_array[3] + " " + input_array[4]; //Inform the Monitor to return the statistics for a single country.

        send_message   (socket_buffer_size,monitor_with_country->get_socket_write(),monitor_with_country->get_socket_read(),command);
        send_message   (socket_buffer_size,monitor_with_country->get_socket_write(),monitor_with_country->get_socket_read(),message);
        receive_message(socket_buffer_size,monitor_with_country->get_socket_write(),monitor_with_country->get_socket_read(),response);

        response_array = split_to_words(response,dummy_num);                             //Break the response to an array.

        std::cout << "~> " << input_array[4]     << " statistics: " << std::endl;        //Print the statistics.
        std::cout << "      Total    Requests: " << (to_int(response_array[0]) + to_int(response_array[1])) << "." << std::endl;
        std::cout << "      Accepted Requests: " << response_array[0] << "." << std::endl;
        std::cout << "      Rejected Requests: " << response_array[1] << "." << std::endl;

        delete [] response_array;
        response_array = NULL;
     }
    else{ //Else return error.
        std::cout << "~> Error: Travel Stats." << std::endl;
     }
}

void travel_stats_monitor(int socket_buffer_size, int num_of_folders, int socket_write, int socket_read, country_records **monitor_countries){
    //A function used by Monitor in order to handle travel statistics requests.
    int         dummy_num = 0;
    int         accepted  = 0;
    int         rejected  = 0;
    std::string message   = "";

    receive_message(socket_buffer_size,socket_write,socket_read,message); //Receive the message from travelMonitor.

    std::string *message_array = split_to_words(message,dummy_num);       //Break the message to an array.

    if(message_array[0] == "ALL"){                                        //If all countries are requested, return the statistixs for all of them.
        for(int i=0; i<num_of_folders; i++){
            monitor_countries[i]->get_period_stats(accepted,rejected,message_array[1],message_array[2],message_array[3]);
            
            message = std::to_string(accepted) + " " + std::to_string(rejected);
            send_message(socket_buffer_size,socket_write,socket_read,message);
            message = "";
         }
     }
    else if(message_array[0] == "SINGLE"){                                //If a single country is requested, find its statistics and return them.
        country_records *requested_country = NULL;

        for(int i=0; i<num_of_folders; i++){
            if(monitor_countries[i]->get_country() == message_array[4]){
                requested_country = monitor_countries[i];
                break;
             }
         }

        requested_country->get_period_stats(accepted,rejected,message_array[1],message_array[2],message_array[3]);

        message = std::to_string(accepted) + " " + std::to_string(rejected);
        send_message(socket_buffer_size,socket_write,socket_read,message);
     }
    delete [] message_array;
    message_array = NULL;
 }
