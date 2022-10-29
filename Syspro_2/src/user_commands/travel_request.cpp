#include "user_commands.hpp"

int travel_request_send(int buffer_size, int num_of_monitors, std::string *input_array, monitor_db **monitor_array){
    //A function used by travelMonitor in order to handle travel requests.
    monitor_db *countryTo_monitor   = get_country_monitor(num_of_monitors,input_array[4],monitor_array);
    monitor_db *countryFrom_monitor = get_country_monitor(num_of_monitors,input_array[3],monitor_array);

    if(!is_number(input_array[1])){                                                   //If the ID is not a number return false.
        return -1;
     }
    if(std::stoi(input_array[1]) > 9999 || std::stoi(input_array[1]) < 0){            //If the ID is not in the boundaries return error.
        std::cout << "~> Error: ID must be between 0 and 9999 but it is (" << input_array[1] << ")." << std::endl;
        std::cout << "   Please enter a valid ID." << std::endl;
        return -1;
     }
    if(!check_date_format(input_array[2])){                                           //Check if the date format is correct.
        std::cout << "~> Error: Please enter a valid date format." << std::endl;
        return -1;
     }
    if(!is_valid_date(input_array[2])){                                               //Check if the date is within accepted boundaries.
        std::cout << "~> Error: Please enter a valid date." << std::endl;
        return -1;
     }
    if(!countryTo_monitor || !countryFrom_monitor){                                   //Check that both countries do exist.
        std::cout << "~> Error: Country does not exist in the system." << std::endl;
        return -1;
     }

    if(countryFrom_monitor->search_bloom_filter(input_array[1],input_array[5]) != 0){ //Search if ID exists.
        std::string command = std::to_string(TRVL_RQ);
        //If it does, send the information to the respective Monitor.
        send_message(buffer_size,countryFrom_monitor->get_pipe_write(),countryFrom_monitor->get_pipe_read(),command);
        send_message(buffer_size,countryFrom_monitor->get_pipe_write(),countryFrom_monitor->get_pipe_read(),input_array[1]);
        send_message(buffer_size,countryFrom_monitor->get_pipe_write(),countryFrom_monitor->get_pipe_read(),input_array[5]);

        std::string response = "";
        //Receive the response.
        receive_message(buffer_size,countryFrom_monitor->get_pipe_write(),countryFrom_monitor->get_pipe_read(),response);

        //According to the response, print the right output.
        if(response == "NO"){
            std::cout << "~> Request Rejected: You are not vaccinated." << std::endl;
            send_to_countryTo(buffer_size,input_array[2],input_array[4],input_array[5],countryTo_monitor,REJECTED_REQ);

            return 0;
         }
        else if(response == "NO VIRUS"){ //If the virus does not exist for the monitor, return error.
            std::cout << "~> Error: Virus does not exist for the monitor of the citizen given." << std::endl;
            return -1;
         }
        else{
            int  dummy_num;
            std::string *response_array = split_to_words(response,dummy_num);

            if(compare_dates(response_array[1],OLDEST_DATE,input_array[2])){
                int *input_date   = date_to_int(input_array[2]);
                int *vaccine_date = date_to_int(response_array[1]);

                if((input_date[2] - vaccine_date[2]) >= 2){
                    std::cout << "~> Request accepted: Happy Travels!" << std::endl;
                    send_to_countryTo(buffer_size,input_array[2],input_array[4],input_array[5],countryTo_monitor,ACCEPTED_REQ);

                    delete_TRQ_dates(response_array,input_date,vaccine_date);
                    return 1;
                 }
                else if(input_date[1] > 6){
                    std::string least_date(std::to_string(input_date[0]) + "-" + std::to_string(input_date[1] - 6) + "-" + std::to_string(input_date[2]));
                    
                    if(compare_dates(response_array[1],OLDEST_DATE,least_date)){
                        std::cout << "~> Request accepted: Happy Travels!" << std::endl;  
                        send_to_countryTo(buffer_size,input_array[2],input_array[4],input_array[5],countryTo_monitor,ACCEPTED_REQ);
                        delete_TRQ_dates(response_array,input_date,vaccine_date);
                        
                        return 1;  
                     }
                    else{
                        std::cout << "~> Request Rejected: You will need another vaccination before travel date." << std::endl;
                        send_to_countryTo(buffer_size,input_array[2],input_array[4],input_array[5],countryTo_monitor,REJECTED_REQ);
                        delete_TRQ_dates(response_array,input_date,vaccine_date);
                        
                        return 0;
                     }
                 }
                else{
                    std::string least_month(std::to_string(input_date[1] + 6));
                    std::string least_year (std::to_string(input_date[2] - 1));
                    std::string least_date (std::to_string(input_date[0]) + "-" + least_month + "-" + least_year);

                    if(compare_dates(response_array[1],OLDEST_DATE,least_date)){
                        std::cout << "~> Request accepted: Happy Travels!" << std::endl; 
                        send_to_countryTo(buffer_size,input_array[2],input_array[4],input_array[5],countryTo_monitor,ACCEPTED_REQ);
                        delete_TRQ_dates(response_array,input_date,vaccine_date);
                        
                        return 1;
                     }
                    else{
                        std::cout << "~> Request Rejected: You will need another vaccination before travel date." << std::endl;
                        send_to_countryTo(buffer_size,input_array[2],input_array[4],input_array[5],countryTo_monitor,REJECTED_REQ);
                        delete_TRQ_dates(response_array,input_date,vaccine_date);
                      
                        return 0;
                     }
                 }
             }
            else{
                std::cout << "~> Request Rejected: The date is before the vaccination date in the system (" << response_array[1] << ")" << std::endl;
                send_to_countryTo(buffer_size,input_array[2],input_array[4],input_array[5],countryTo_monitor,REJECTED_REQ);
                delete_TRQ_dates(response_array);
                
                return 0;
             }
         }
     }
    else{
        std::cout << "~> Request Rejected: You are not vaccinated." << std::endl;
        send_to_countryTo(buffer_size,input_array[2],input_array[4],input_array[5],countryTo_monitor,REJECTED_REQ);
        
        return 0;
     }
}

void add_to_destination(int buffer_size, int num_of_folders, int pipe_write, int pipe_read, virus *vrs_list, country_records **monitor_countries){
    //Used by Monitor to inform a country of destination about a travel request.
    int          dummy_num = 0;                                                     //A number that wont be used.
    virus       *req_vrs   = vrs_list;
    std::string  date      = "";
    std::string  message   = "";
    
    receive_message(buffer_size,pipe_write,pipe_read,message);//
    receive_message(buffer_size,pipe_write,pipe_read,date);

    std::string     *message_array = split_to_words(message,dummy_num);
    country_records *country_r     = get_country_record(message_array[1],num_of_folders,monitor_countries);

    while(req_vrs->get_virus() != message_array[2]){req_vrs = req_vrs->get_next();} //Find the specific virus.

    if(message_array[0] == "YES"){                                                  //Record the travel request along with its date.
        country_r->update_date_stats(ACCEPTED_REQ,date,req_vrs);
     }
    else{
        country_r->update_date_stats(REJECTED_REQ,date,req_vrs);
     }
    delete [] message_array;
    message_array = NULL;
}

void travel_request_receive(int buffer_size, int pipe_write, int pipe_read, skip_list ***vaccine_skip_l, skip_list *information_check, data_base *citizen_info){
    //A function used by Monitor in order to handle travel requests.
    std::string ID        = "";
    std::string str_virus = "";

    //Receive citizen information.
    receive_message(buffer_size,pipe_write,pipe_read,ID);
    receive_message(buffer_size,pipe_write,pipe_read,str_virus);

    if(!is_virus(str_virus,citizen_info->get_vrs_ptr(),true)){     //Search for the virus.
        send_message(buffer_size,pipe_write,pipe_read,"NO VIRUS"); //If it does not exist, inform travelMonitor and return.
        return;
     }

    skip_list *vac_skip_l = get_skip_list(citizen_info->get_vrs_num(),str_virus,vaccine_skip_l,VACCINATED);//Search for citizen.
    block     *ctzn_vac   = vac_skip_l->search(std::stoi(ID));

    if(ctzn_vac){                                                  //If citizen is vaccinated, return the vaccination date.
        send_message(buffer_size,pipe_write,pipe_read,("YES " + ctzn_vac->get_citizen()->get_date()));
     }
    else{
        send_message(buffer_size,pipe_write,pipe_read,"NO");
     }
}
