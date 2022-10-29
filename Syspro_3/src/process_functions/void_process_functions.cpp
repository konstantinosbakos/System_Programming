#include <fcntl.h>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/socket.h>

#include "process_functions.hpp"

void fork_error(){              //Error to be printed if fork() fails.
    perror(("\n~> Error: Process could not be forked (" + std::to_string(errno) + ").").c_str());
}

void recv_error(){              //Error to be printed if recv() fails.
    perror(("\n~> Error: Message could not be received (" + std::to_string(errno) + ").").c_str());
}

void send_error(){              //Error to be printed if send() fails.
    perror(("\n~> Error: Message could not be sent (" + std::to_string(errno) + ").").c_str());
}

void delete_array(char *array){ //Delete a dynamic buffer (used locally).
    delete [] array;
    array = NULL;
}

void update_travel_blooms(int socket_buffer_size, int bloom_size, monitor_db *monitor){ //Function used in order to update the bloom filters of travelMonitor.
    int         num_of_vrs     = 0;
    std::string num_of_vrs_str = "";

    receive_message(socket_buffer_size,monitor->get_socket_write(),monitor->get_socket_read(),num_of_vrs_str);

    num_of_vrs = to_int(num_of_vrs_str);

    for(int i=0; i<num_of_vrs; ++i){
        char       *bloom_string = NULL;
        std::string temp_vrs     = "";

        receive_bloom_filter(socket_buffer_size,bloom_size,monitor->get_socket_write(),monitor->get_socket_read(),&bloom_string,temp_vrs);
        monitor->update_bloom_filter(bloom_string,temp_vrs);
     }
}

void send_message(int socket_buffer_size, int socket_write, int socket_read, std::string message){ //Function used to osend messages via sockets.
    int   continue_loop  = 1;
    char *buffer         = new char[socket_buffer_size];

    if(message == ""){
        std::cout << "Error: Message to be sent is empty." << std::endl;
        return;
     }

    while(continue_loop){                                                  //A loop, that will end when all the message passes through (if it needs to be sent in to pieces).
        memset(buffer,'\0',socket_buffer_size);

        if(recv(socket_read,buffer,socket_buffer_size,MSG_WAITALL) == -1){ //The receiving end, sends a message to inform that it is okay to proceed.
            recv_error();
            delete_array(buffer);
            return;
         }
        else if(buffer[0] == SIG_DONE){                                    //Continue with passing the message to the named pipe.
            memset(buffer,'\0',socket_buffer_size);

            if(int(message.length()) <= (socket_buffer_size-2)){           //If the message is of equal or smaller length to the buffer size, then proceed and terminate the loop.
                message = SIG_DONE + message;                              //Place the SIG_DONE flag to inform that the trasmittion is over. 

                sprintf(buffer,"%s",message.c_str());

                if(send(socket_write,buffer,socket_buffer_size,0) == -1){
                    send_error();
                    delete_array(buffer);
                    return;
                 }

                continue_loop = 0;
             }
            else{                                                          //Else write a SIG_NOT_DONE signal to inform that there ar emore parts of the message to be sent.
                message = SIG_NOT_DONE + message;

                sprintf(buffer,"%s",message.substr(0,socket_buffer_size-1).c_str());

                if(send(socket_write,buffer,socket_buffer_size,0) == -1){
                    send_error();
                    delete_array(buffer);
                    return;
                 }

                message.erase(0,socket_buffer_size-1);           //Remove the part that was just sent.
             }
         }
     }
    delete_array(buffer);
}

void export_log(int accepted_req, int rejected_req, int num_of_folders, std::string *folder_array){ //Print the log_file of a process, as requested.
    int             max                 = 0;
    std::string     temp                = "";
    std::string     countries           = "";
    std::string    *stats_print_array   = new std::string[3];
    std::string    *country_print_array = new std::string[num_of_folders+1];
    std::streambuf *cmd                 = std::cout.rdbuf();
    std::ofstream   init_log_file(LOG(getpid()));
    
    std::cout.rdbuf(init_log_file.rdbuf());

    std::cout << "> Monitor : " + std::to_string(getpid()) << std::endl;

    for(int i=0; i<=num_of_folders; i++){ //Print the countries of the process.
        if(i == 0){
            country_print_array[i] = "|Countries:";
         }
        else{
            country_print_array[i] = "|    " + get_folder_country(folder_array[i-1]);
         }

        if(int(country_print_array[i].length()) > max){
            max = country_print_array[i].length();
         }
     }

    print_underscore(max-1);

    for(int i=0; i<= num_of_folders; i++){
        std::cout << country_print_array[i];
        print_bar(max - int(country_print_array[i].length()));
     }

    print_tilde(max-1);

    max = 0;
    //Print the total and partial requests.
    stats_print_array[0] =  "| Total travel requests: " + std::to_string(accepted_req + rejected_req);
    stats_print_array[1] =  "|  >Accepted:            " + std::to_string(accepted_req);
    stats_print_array[2] =  "|  >Rejected:            " + std::to_string(rejected_req);

    for(int i=0; i<3; i++){
        if(int(stats_print_array[i].length()) > max){
            max = int(stats_print_array[i].length());
         }
     }

    print_underscore(max-1);

    for(int i=0; i<3; i++){
        std::cout << stats_print_array[i];
        print_bar(max - int(stats_print_array[i].length()));
     }

    print_tilde(max-1);

    std::cout.rdbuf(cmd);

    delete [] stats_print_array;
    stats_print_array = NULL;

    delete [] country_print_array;
    country_print_array = NULL;
}

void receive_message(int socket_buffer_size, int socket_write, int socket_read, std::string &message){ //A function for receiving messages via sockets.
    int  continue_loop = 1;
    char *buffer       = new char[socket_buffer_size];

    memset(buffer,'\0',socket_buffer_size);
    buffer[0] = SIG_DONE;

    if(send(socket_write,buffer,socket_buffer_size,0) == -1){              //write a message to inform that the message passing can now begin.
        send_error();
        delete_array(buffer);
        return;
     }

    while(continue_loop){                                                  //A loop, that will end when all the message passes through (if it needs to be sent in to pieces).
        memset(buffer,'\0',socket_buffer_size);

        if(recv(socket_read,buffer,socket_buffer_size,MSG_WAITALL) == -1){ //Read the next part of the message.
            recv_error();
            delete_array(buffer);
            return;
         }
        else{
            message = message + (buffer + 1);                              //Add the new part to the message.

            if(buffer[0] == SIG_DONE){                                     //If all the message is through, stop the loop.
                continue_loop = 0;
             }
            else{                                                          //If the trasmittion must continue, inform that it is okay to do so.
                memset(buffer,'\0',socket_buffer_size);
                buffer[0] = SIG_DONE;
                if(send(socket_write,buffer,socket_buffer_size,0) == -1){
                    send_error();
                    delete_array(buffer);
                    return;
                 }
             }
         }
     }
    delete_array(buffer);
}

void send_message(int socket_buffer_size, int bloom_size, int socket_write, int socket_read, char *message){
    //While the philosophy of the way the function works is the same as above, it is slightly 
    //modified in order to pass char arrays (The function is used to write bloom_strings).
    int  continue_loop  = 1;
    int  curr_length    = 0;
    char *buffer        = new char[socket_buffer_size];

    while(continue_loop){
        memset(buffer,'\0',socket_buffer_size);

        if(recv(socket_read,buffer,socket_buffer_size,MSG_WAITALL) == -1){
            recv_error();
            delete_array(buffer);
            return;
         }
        else if(buffer[0] == SIG_DONE){
            memset(buffer,'\0',socket_buffer_size);

            if((curr_length + socket_buffer_size) > bloom_size - 1){
                buffer[0] = SIG_DONE;

                memcpy(buffer+1,(message + curr_length),bloom_size - curr_length);
                if(send(socket_write,buffer,socket_buffer_size,0) == -1){
                    send_error();
                    delete_array(buffer);
                    return;
                 }

                continue_loop = 0;
             }
            else{
                memset(buffer,'\0',socket_buffer_size);

                buffer[0] = SIG_NOT_DONE;

                memcpy(buffer+1,(message + curr_length),(socket_buffer_size - 1));

                curr_length = curr_length + socket_buffer_size - 1;

                if(send(socket_write,buffer,socket_buffer_size,0) == -1){
                    send_error();
                    delete_array(buffer);
                    return;
                 }
             }
         }
     }
    delete_array(buffer);
}

void receive_message(int socket_buffer_size, long bloom_size, int socket_write, int socket_read, char **message){
    //While the philosophy of the way the function works is the same as above, it is slightly 
    //modified in order to pass char arrays (The function is used to receive bloom_strings).
    int   curr_length   = 0;
    int   continue_loop = 1;
    char *buffer        = new char[socket_buffer_size];

    (*message) = new char[bloom_size];

    memset(buffer,'\0',socket_buffer_size);
    buffer[0] = SIG_DONE;

    if(send(socket_write,buffer,socket_buffer_size,0) == -1){
        send_error();
        delete_array(buffer);
        return;
     }

    while(continue_loop){
        memset(buffer,'\0',socket_buffer_size);

        if(recv(socket_read,buffer,socket_buffer_size,MSG_WAITALL) == -1){
            recv_error();
            delete_array(buffer);
            return;
         }
        else{
            if((curr_length + socket_buffer_size) < bloom_size){
                memcpy(((*message)+curr_length),(buffer+1),(socket_buffer_size-1));
                curr_length = curr_length + socket_buffer_size - 1;
             }
            else{
                int last_length = bloom_size - curr_length;
                memcpy(((*message)+curr_length),buffer+1,last_length);
             }

            if(buffer[0] == SIG_DONE){
                continue_loop = 0;
             }
            else{
                memset(buffer,'\0',socket_buffer_size);
                buffer[0] = SIG_DONE;
                if(send(socket_write,buffer,socket_buffer_size,0) == -1){
                    send_error();
                    delete_array(buffer);
                    return;
                 }
             }
         }
     }
    delete_array(buffer);
}

void get_travel_blooms(int socket_buffer_size, long bloom_size, int num_of_monitors, monitor_db **monitor_array){ //A function used by travelMonitor in order to receive the bloom_filters.
    for(int i=0; i<num_of_monitors;i++){
        int         num_of_vrs     = 0;
        std::string num_of_vrs_str = "";
        receive_message(socket_buffer_size,monitor_array[i]->get_socket_write(),monitor_array[i]->get_socket_read(),num_of_vrs_str);

        num_of_vrs = to_int(num_of_vrs_str);

        for(int j=0; j<num_of_vrs; j++){
            char       *bloom_string = NULL;
            std::string temp_vrs     = "";

            receive_bloom_filter(socket_buffer_size,bloom_size,monitor_array[i]->get_socket_write(),monitor_array[i]->get_socket_read(),&bloom_string,temp_vrs);

            monitor_array[i]->add_bloom_filter(bloom_string,temp_vrs);
         }
     }
}

void send_bloom_filter(int socket_buffer_size, long bloom_size, int num_of_vrs, int socket_write, int socket_read, bloom_filter **bloom){        //A function used to write bloom_filters.
    send_message(socket_buffer_size,socket_write,socket_read,std::to_string(num_of_vrs));

    for(int i=0; i<num_of_vrs; i++){
        send_message(socket_buffer_size,socket_write,socket_read,bloom[i]->get_virus());
        send_message(socket_buffer_size,bloom_size,socket_write,socket_read,bloom[i]->get_bloom_array());
     }
}

void receive_bloom_filter(int socket_buffer_size, int bloom_size, int socket_write, int socket_read, char **bloom_string, std::string &_virus){ //A function used to receive bloom_filters.
    receive_message(socket_buffer_size,socket_write,socket_read,_virus);
    receive_message(socket_buffer_size,bloom_size,socket_write,socket_read,bloom_string);
}

void send_to_countryTo(int socket_buffer_size, std::string date, std::string countryTo, std::string _virus, monitor_db *countryTo_monitor, bool accepted){
    //A function used to inform a country of the approval (or not) of a travel request.
    std::string command = std::to_string(COUNTRY_TO_M);

    if(accepted){
        std::string message = "YES " + countryTo + " " + _virus;

        send_message(socket_buffer_size,countryTo_monitor->get_socket_write(),countryTo_monitor->get_socket_read(),command); //Write the command,
        send_message(socket_buffer_size,countryTo_monitor->get_socket_write(),countryTo_monitor->get_socket_read(),message); //the information
        send_message(socket_buffer_size,countryTo_monitor->get_socket_write(),countryTo_monitor->get_socket_read(),date);    //and the date (of the request).
     }
    else{
        std::string message = "NO " + countryTo + " " + _virus;
     
        send_message(socket_buffer_size,countryTo_monitor->get_socket_write(),countryTo_monitor->get_socket_read(),command);
        send_message(socket_buffer_size,countryTo_monitor->get_socket_write(),countryTo_monitor->get_socket_read(),message);
        send_message(socket_buffer_size,countryTo_monitor->get_socket_write(),countryTo_monitor->get_socket_read(),date);

     }
}