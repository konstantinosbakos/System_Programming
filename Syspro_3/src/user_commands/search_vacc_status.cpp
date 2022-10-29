#include "user_commands.hpp"

void search_vaccination_status(int socket_buffer_size, int num_of_monitors, std::string *input_array, monitor_db **monitor_array){
    //A function used by travelMonitor to search and print the vaccination status of a specific citizen.
    monitor_db  *monitor_with_id = NULL;
    std::string  command         = std::to_string(SRCH_VAC_STS);

    if(!is_number(input_array[1])){                                           //If the ID is not a number, return error.
        return;
     }
    if(to_int(input_array[1]) > 9999 || to_int(input_array[1]) < 0){    //If the ID is not in the boundaries return error.
        std::cout << "~> Error: ID must be between 0 and 9999 but it is (" << input_array[1] << ")." << std::endl;
        std::cout << "   Please enter a valid ID." << std::endl;
        return;
     }

    for(int i=0; i<num_of_monitors; i++){                                     //Communicate with the monitors to find the one that has the ID.
        std::string response = "";

        send_message(socket_buffer_size,monitor_array[i]->get_socket_write(),monitor_array[i]->get_socket_read(),command);
        send_message(socket_buffer_size,monitor_array[i]->get_socket_write(),monitor_array[i]->get_socket_read(),input_array[1]);
        receive_message(socket_buffer_size,monitor_array[i]->get_socket_write(),monitor_array[i]->get_socket_read(),response);
        if(response == "YES"){
            monitor_with_id = monitor_array[i];
            break;
         }
     }

    if(!monitor_with_id){                                                     //If the ID is not found, return error.
        std::cout << "~> Error: ID does not exist in the system." << std::endl;
        return;
     }
    else{                                                                     //If the ID is found.
        std::string citizen_details = "";
        std::string citizen_viruses = "";

        //receive message with the detais of the citizen and the details for the viruses he is recorded for.
        receive_message(socket_buffer_size,monitor_with_id->get_socket_write(),monitor_with_id->get_socket_read(),citizen_details);
        receive_message(socket_buffer_size,monitor_with_id->get_socket_write(),monitor_with_id->get_socket_read(),citizen_viruses);

        int          max                   = 0;
        int          num_of_details        = 0;
        int          num_of_vrs_details    = 0;
        std::string *citizen_details_array = split_to_words(citizen_details,num_of_details);
        std::string *citizen_viruses_array = split_to_words(citizen_viruses,num_of_vrs_details);

        std::cout << "~> ID     : " << input_array[1]           << std::endl; //Print citizen details.
        std::cout << "~> Name   : " << citizen_details_array[0] << std::endl;
        std::cout << "~> Surname: " << citizen_details_array[1] << std::endl;
        std::cout << "~> Age    : " << citizen_details_array[2] << std::endl;
        std::cout << "~> Country: " << citizen_details_array[3] << std::endl;
        std::cout << "~> Viruses: " << std::endl;

        for(int i=0; i<num_of_vrs_details; i++){
            if(int(citizen_viruses_array[i].length()) > max){
                max = int(citizen_viruses_array[i].length());
             }
         }

        for(int i=0; i<num_of_vrs_details; i++){                              //Print citizen virus details.
            if(citizen_viruses_array[i] != "YES" && citizen_viruses_array[i] != "NO"){
                std::cout << "       " << citizen_viruses_array[i];
                print_space(max - int(citizen_viruses_array[i].length()));
                std::cout << ": ";
             }
            else if(citizen_viruses_array[i] == "YES"){
                std::cout << "Vaccinated on " <<  citizen_viruses_array[i+1] << "." << std::endl;

                i++;
             }
            else if(citizen_viruses_array[i] == "NO"){
                std::cout << "Not Vaccinated." << std::endl;
             }
         }
        delete [] citizen_details_array;
        delete [] citizen_viruses_array;
        citizen_details_array = NULL;
        citizen_viruses_array = NULL;
     }
}

void search_vaccination_status_monitor(int socket_buffer_size, int num_of_vrs, int socket_write, int socket_read, skip_list ***vaccine_skip_l, skip_list *information_check){
    //Function used by Monitor to search for a citizen and its records.
    std::string ID = "";

    receive_message(socket_buffer_size,socket_write,socket_read,ID);   //Receive citizen ID.

    block *ctzn = information_check->search(to_int(ID));            //Search for citizen ID.

    if(ctzn){//Inform whether or not citizen was found
        send_message(socket_buffer_size,socket_write,socket_read,"YES");
     }
    else{
        send_message(socket_buffer_size,socket_write,socket_read,"NO");
        return;
     }

    //If the citizen was found, write his details, as well as the details of the viruses he is recorded for.
    std::string age     = std::to_string(ctzn->get_citizen()->get_age());
    std::string name    = ctzn->get_citizen()->get_first_name();
    std::string country = ctzn->get_citizen()->get_country();
    std::string surname = ctzn->get_citizen()->get_surname();

    std::string message = name + " " + surname + " " + age + " " + country;

    send_message(socket_buffer_size,socket_write,socket_read,message);

    std::string citizen_viruses = "";

    vaccine_status(to_int(ID),num_of_vrs,vaccine_skip_l,citizen_viruses);

    send_message(socket_buffer_size,socket_write,socket_read,citizen_viruses);
}
