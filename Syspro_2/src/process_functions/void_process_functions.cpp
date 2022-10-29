#include <fcntl.h>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "process_functions.hpp"

void fork_error(){                                                       //Error to be printed if fork() fails.
    perror(("\n~> Error: Process could not be forked (" + std::to_string(errno) + ").").c_str());
}

void open_error(){                                                       //Error to be printed if open() fails.
    perror(("\n~> Error: Pipe could not be opened (" + std::to_string(errno) + ").").c_str());
}

void read_error(){                                                       //Error to be printed if read() fails.
    if(no_print == 0){
        perror(("\n~> Error: Pipe could not be read (" + std::to_string(errno) + ").").c_str());
     }
    else{
        no_print = 0;
     }
}

void fgets_error(){                                                      //Error to be printed if fgets() fails.
    if(no_print == 0){
        perror("\n~> Error : Fgets");
     }
    else{
        no_print = 0;
     }
}

void write_error(){                                                      //Error to be printed if write() fails.
    if(no_print == 0){
        perror(("\n~> Error: Pipe could not be written (" + std::to_string(errno) + ").").c_str());
     }
    else{
        no_print = 0;
     }
}

void mkfifo_error(){                                                     //Error to be printed if mkfifo() fails.
    perror(("\n~> Error: Pipe could not be made (" + std::to_string(errno) + ").").c_str());
}

void delete_buffer(char *buffer){                                        //Delete a dynamic buffer (used locally).
    delete [] buffer;
    buffer = NULL;
}

void clear_pipe(int pipe, int buffer_size){                              //Clear contents of a named pipe.
    char *buffer = new char[buffer_size];

    if(read(pipe,buffer,buffer_size) == -1){
        read_error();
     }
    delete_buffer(buffer);
}

void create_named_pipes(char ****pipe_array, int num_of_monitors){       //Create the array of the named pipes that will be used.
    (*pipe_array) = new char**[num_of_monitors];

    for(int i=0; i<num_of_monitors; i++){
        (*pipe_array)[i] = new char*[2];   
     }
    for(int i=0; i<num_of_monitors; i++){
        (*pipe_array)[i][0] = pipe_name(i+1,0);
        (*pipe_array)[i][1] = pipe_name(i+1,1);
     }
}

void receive_handshake(int &buffer_size, int **pipe, char **pipe_names){ //Receive the size of the buffer and open the named pipes.
    int   pipe_read;
    int   pipe_write;
    char *handshake_buffer = new char[HNDSHK_SIZE];

    if((pipe_read = open(pipe_names[MON_R],O_RDONLY)) == -1){
        open_error();
     }

    if((pipe_write = open(pipe_names[MON_W],O_WRONLY)) == -1){
        open_error();
     }


    (*pipe)[MON_R] = pipe_read;
    (*pipe)[MON_W] = pipe_write;
    if(((*pipe)[2] = open(pipe_names[MON_W],O_RDONLY)) == -1){           //The pipe that the monitor will be writting is also 
        open_error();                                                    //opened for reading in order to empty the pipe when 
     }                                                                   //the monitor receives a signal.

    if(read(pipe_read,handshake_buffer,HNDSHK_SIZE) == -1){
        read_error();
        delete_buffer(handshake_buffer);
        return;
     }

    sscanf(handshake_buffer,"%d",&buffer_size);

    char *buffer = new char[buffer_size];
    
    memset(buffer,'\0',buffer_size);
    buffer[0] = SIG_DONE;

    if(write(pipe_write,buffer,buffer_size) == -1){
        write_error();
        delete_buffer(buffer);
        delete_buffer(handshake_buffer);
        return;
     }
    delete_buffer(buffer);
    delete_buffer(handshake_buffer);
}

void send_handshake(int buffer_size, char ***pipe_array, monitor_db **monitor){ //TravelMonitor sends the buffer size.
    int   pipe_read;
    int   pipe_write;
    int   monitor_num      = (*monitor)->get_monitor_num();
    char *handshake_buffer = new char[HNDSHK_SIZE];

    if((pipe_write = open(pipe_array[monitor_num][TRVL_MON_W],O_WRONLY)) == -1){
        open_error();
     }

    if((pipe_read = open(pipe_array[monitor_num][TRVL_MON_R],O_RDONLY)) == -1){
        open_error();
     }

    (*monitor)->set_pipe_read(pipe_read);
    (*monitor)->set_pipe_write(pipe_write);

    memset(handshake_buffer,'\0',HNDSHK_SIZE);

    sprintf(handshake_buffer,"%d",buffer_size);

    if(write(pipe_write,handshake_buffer,HNDSHK_SIZE) == -1){
        write_error();
        delete_buffer(handshake_buffer);
        return;
     }

    char *buffer = new char[buffer_size];
    memset(buffer,'\0',buffer_size);

    if(read(pipe_read,buffer,buffer_size) == -1){
        read_error();
        delete_buffer(buffer);
        delete_buffer(handshake_buffer);
        return;
     }

    if(buffer[0] == SIG_DONE){
        delete_buffer(buffer);
        delete_buffer(handshake_buffer);
        return;
     }
}

void update_travel_blooms(int buffer_size, int bloom_size, monitor_db *monitor){ //Function used in order to update the bloom filters of travelMonitor.
    int         num_of_vrs     = 0;
    std::string num_of_vrs_str = "";

    receive_message(buffer_size,monitor->get_pipe_write(),monitor->get_pipe_read(),num_of_vrs_str);

    num_of_vrs = std::stoi(num_of_vrs_str);

    for(int i=0; i<num_of_vrs; ++i){
        char       *bloom_string = NULL;
        long        pid_id       = 0;
        std::string temp_vrs     = "";

        receive_bloom_filter(buffer_size,bloom_size,monitor->get_pipe_write(),monitor->get_pipe_read(),pid_id,&bloom_string,temp_vrs);
        monitor->update_bloom_filter(bloom_string,temp_vrs);
     }
}

void send_message(int buffer_size, int pipe_write, int pipe_read, std::string message){//Function used to osend messages via named pipes.
    int   continue_loop  = 1;
    char *buffer         = new char[buffer_size];

    if(message == ""){
        std::cout << "Error: Message to be sent is empty." << std::endl;
        return;
     }

    while(continue_loop){                                 //A loop, that will end when all the message passes through (if it needs to be sent in to pieces).
        memset(buffer,'\0',buffer_size);

        if(read(pipe_read,buffer,buffer_size) == -1){     //The receiving end, sends a message to inform that it is okay to proceed.
            read_error();
            delete_buffer(buffer);
            return;
         }
        else if(buffer[0] == SIG_DONE){                   //Continue with passing the message to the named pipe.
            memset(buffer,'\0',buffer_size);

            if(int(message.length()) <= (buffer_size-2)){ //If the message is of equal or smaller length to the buffer size, then proceed and terminate the loop.
                message = SIG_DONE + message;             //Place the SIG_DONE flag to inform that the trasmittion is over. 

                sprintf(buffer,"%s",message.c_str());

                if(write(pipe_write,buffer,buffer_size) == -1){
                    write_error();
                    delete_buffer(buffer);
                    return;
                 }

                continue_loop = 0;
             }
            else{                                         //Else send a SIG_NOT_DONE signal to inform that there ar emore parts of the message to be sent.
                message = SIG_NOT_DONE + message;

                sprintf(buffer,"%s",message.substr(0,buffer_size-1).c_str());

                if(write(pipe_write,buffer,buffer_size) == -1){
                    write_error();
                    delete_buffer(buffer);
                    return;
                 }

                message.erase(0,buffer_size-1);           //Remove the part that was just sent.
             }
         }
     }
    delete_buffer(buffer);
}

void receive_message(int buffer_size, int  pipe_write, int  pipe_read, std::string &message){ //A function for receiving messages via named pipes.
    int  continue_loop = 1;
    char *buffer       = new char[buffer_size];

    memset(buffer,'\0',buffer_size);
    buffer[0] = SIG_DONE;

    if(write(pipe_write,buffer,buffer_size) == -1){   //Send a message to inform that the message passing can now begin.
        write_error();
        delete_buffer(buffer);
        return;
     }

    while(continue_loop){                             //A loop, that will end when all the message passes through (if it needs to be sent in to pieces).
        memset(buffer,'\0',buffer_size);

        if(read(pipe_read,buffer,buffer_size) == -1){ //Read the next part of the message.
            read_error();
            delete_buffer(buffer);
            return;
         }
        else{
            message = message + (buffer + 1);         //Add the new part to the message.

            if(buffer[0] == SIG_DONE){                //If all the message is through, stop the loop.
                continue_loop = 0;
             }
            else{                                     //If the trasmittion must continue, inform that it is okay to do so.
                memset(buffer,'\0',buffer_size);
                buffer[0] = SIG_DONE;
                if(write(pipe_write,buffer,buffer_size) == -1){
                    write_error();
                    delete_buffer(buffer);
                    return;
                 }
             }
         }
     }
    delete_buffer(buffer);
}

void export_log(int accepted_req, int rejected_req, int num_of_folders, std::string *folder_array){//Print the log_file of a process, as requested.
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

void send_message(int buffer_size, int bloom_size, int  pipe_write, int  pipe_read, char *message){ //A function to pass a message via named pipes.
    //While the philosophy of the way the function works is the same as above, it is slightly 
    //modified in order to pass char arrays (The function is used to send bloom_strings).
    int  continue_loop  = 1;
    int  curr_length    = 0;
    char *buffer        = new char[buffer_size];

    while(continue_loop){
        memset(buffer,'\0',buffer_size);

        if(read(pipe_read,buffer,buffer_size) == -1){
            read_error();
            delete_buffer(buffer);
            return;
         }
        else if(buffer[0] == SIG_DONE){
            memset(buffer,'\0',buffer_size);

            if((curr_length + buffer_size) > bloom_size - 1){
                buffer[0] = SIG_DONE;

                memcpy(buffer+1,(message + curr_length),bloom_size - curr_length);
                if(write(pipe_write,buffer,buffer_size) == -1){
                    write_error();
                    delete_buffer(buffer);
                    return;
                 }

                continue_loop = 0;
             }
            else{
                memset(buffer,'\0',buffer_size);

                buffer[0] = SIG_NOT_DONE;

                memcpy(buffer+1,(message + curr_length),(buffer_size - 1));

                curr_length = curr_length + buffer_size - 1;

                if(write(pipe_write,buffer,buffer_size) == -1){
                    write_error();
                    delete_buffer(buffer);
                    return;
                 }
             }
         }
     }
    delete_buffer(buffer);
}

void receive_message(int buffer_size, long bloom_size, int  pipe_write, int  pipe_read, char **message){ //A function used to receive messages.
    //While the philosophy of the way the function works is the same as above, it is slightly 
    //modified in order to pass char arrays (The function is used to receive bloom_strings).
    int   curr_length   = 0;
    int   continue_loop = 1;
    char *buffer        = new char[buffer_size];

    (*message) = new char[bloom_size];

    memset(buffer,'\0',buffer_size);
    buffer[0] = SIG_DONE;

    if(write(pipe_write,buffer,buffer_size) == -1){
        write_error();
        delete_buffer(buffer);
        return;
     }

    while(continue_loop){
        memset(buffer,'\0',buffer_size);

        if(read(pipe_read,buffer,buffer_size) == -1){
            read_error();
            delete_buffer(buffer);
            return;
         }
        else{
            if((curr_length + buffer_size) < bloom_size){
                memcpy(((*message)+curr_length),(buffer+1),(buffer_size-1));
                curr_length = curr_length + buffer_size - 1;
             }
            else{
                int last_length = bloom_size - curr_length;
                memcpy(((*message)+curr_length),buffer+1,last_length);
             }

            if(buffer[0] == SIG_DONE){
                continue_loop = 0;
             }
            else{
                memset(buffer,'\0',buffer_size);
                buffer[0] = SIG_DONE;
                if(write(pipe_write,buffer,buffer_size) == -1){
                    write_error();
                    delete_buffer(buffer);
                    return;
                 }
             }
         }
     }
    delete_buffer(buffer);
}

void get_travel_blooms(int buffer_size, long bloom_size, int num_of_monitors, monitor_db **monitor_array){ //A function used by travelMonitor in order to receive the bloom_filters.
    for(int i=0; i<num_of_monitors;i++){
        int         num_of_vrs     = 0;
        std::string num_of_vrs_str = "";

        receive_message(buffer_size,monitor_array[i]->get_pipe_write(),monitor_array[i]->get_pipe_read(),num_of_vrs_str);

        num_of_vrs = std::stoi(num_of_vrs_str);

        for(int j=0; j<num_of_vrs; j++){
            char       *bloom_string = NULL;
            long        pid_id       = 0;
            std::string temp_vrs     = "";

            receive_bloom_filter(buffer_size,bloom_size,monitor_array[i]->get_pipe_write(),monitor_array[i]->get_pipe_read(),pid_id,&bloom_string,temp_vrs);
            
            monitor_array[i]->add_bloom_filter(bloom_string,temp_vrs);
         }
     }
}

void send_bloom_filter(int buffer_size, long bloom_size, int num_of_vrs, int pipe_r, int pipe_w, long pid_id, bloom_filter **bloom){        //A function used to send bloom_filters.
    send_message(buffer_size,pipe_w,pipe_r,std::to_string(num_of_vrs));

    for(int i=0; i<num_of_vrs; i++){
        send_message(buffer_size,pipe_w,pipe_r,std::to_string(pid_id));
        send_message(buffer_size,pipe_w,pipe_r,bloom[i]->get_virus());
        send_message(buffer_size,bloom_size,pipe_w,pipe_r,bloom[i]->get_bloom_array());
     }
}

void receive_bloom_filter(int buffer_size, int bloom_size, int pipe_w, int pipe_r, long &pid_id, char **bloom_string, std::string &_virus){ //A function used to receive bloom_filters.
    std::string pid_id_str  = "";
    receive_message(buffer_size,pipe_w,pipe_r,pid_id_str);
    receive_message(buffer_size,pipe_w,pipe_r,_virus);
    receive_message(buffer_size,bloom_size,pipe_w,pipe_r,bloom_string);

    pid_id = std::stol(pid_id_str);
}

void send_to_countryTo(int buffer_size, std::string date, std::string countryTo, std::string _virus, monitor_db *countryTo_monitor, bool accepted){
    //A function used to inform a country of the approval (or not) of a travel request.
    std::string command = std::to_string(COUNTRY_TO_M);

    if(accepted){
        std::string message = "YES " + countryTo + " " + _virus;

        send_message(buffer_size,countryTo_monitor->get_pipe_write(),countryTo_monitor->get_pipe_read(),command);//Send the command,
        send_message(buffer_size,countryTo_monitor->get_pipe_write(),countryTo_monitor->get_pipe_read(),message);//the information
        send_message(buffer_size,countryTo_monitor->get_pipe_write(),countryTo_monitor->get_pipe_read(),date);//and the date (of the request).
     }
    else{
        std::string message = "NO " + countryTo + " " + _virus;
     
        send_message(buffer_size,countryTo_monitor->get_pipe_write(),countryTo_monitor->get_pipe_read(),command);
        send_message(buffer_size,countryTo_monitor->get_pipe_write(),countryTo_monitor->get_pipe_read(),message);
        send_message(buffer_size,countryTo_monitor->get_pipe_write(),countryTo_monitor->get_pipe_read(),date);

     }
}

void restart_child(int buffer_size, int num_of_monitors, char ***pipe_array, long bloom_size, monitor_db **monitor_array, std::string *monitor_inputs){
    //A function used by travelMonitor to restart a N number of children that crashed.
    pid_t crashed_child;

    fflush(stdin);                                                 //Empty input stream (to unblock fgets).
    std::cout << std::endl;
    std::cout << "~> Warning: Child crashed, please wait...\n" << std::endl;

    while((crashed_child = waitpid(-1,NULL,WNOHANG))){             //Find the crashed children.
        int          num_of_vrs      = 0;
        monitor_db  *crashed_monitor = NULL;
        std::string  message         = "";
        std::string  num_of_vrs_str  = "";

        for(int i=0; i<num_of_monitors; i++){                      //Find the respective monitor.
            if(monitor_array[i]->get_monitor_pid() == crashed_child){
                crashed_monitor = monitor_array[i];
                break;
             }
         }

        unlink(pipe_array[crashed_monitor->get_monitor_num()][0]); //Delete the named pipes.
        unlink(pipe_array[crashed_monitor->get_monitor_num()][1]);

        //Recreate the named pipes.
        if(mkfifo(pipe_array[crashed_monitor->get_monitor_num()][0],0666) == -1 || mkfifo(pipe_array[crashed_monitor->get_monitor_num()][1],0666) == -1 ){
            mkfifo_error();            
            return;
         }

        if((crashed_child = fork()) == -1){                        //Fork the new process.
            fork_error();
            return;
         }

        if(crashed_child == 0){
            char * const initial_input[] = {(char *)("./Monitor"),pipe_array[crashed_monitor->get_monitor_num()][TRVL_MON_W],pipe_array[crashed_monitor->get_monitor_num()][TRVL_MON_R], nullptr};
            execvp("./Monitor",initial_input);
         }
        else{                                                      //Update the pid.
            crashed_monitor->update_pid(crashed_child);
         }
        //Initialize the new process
        send_handshake (buffer_size,pipe_array,&monitor_array[crashed_monitor->get_monitor_num()]);
        send_message   (buffer_size,crashed_monitor->get_pipe_write(),crashed_monitor->get_pipe_read(),std::to_string(bloom_size));
        send_message   (buffer_size,crashed_monitor->get_pipe_write(),crashed_monitor->get_pipe_read(),monitor_inputs[crashed_monitor->get_monitor_num()]);
        receive_message(buffer_size,crashed_monitor->get_pipe_write(),crashed_monitor->get_pipe_read(),num_of_vrs_str);
        
        num_of_vrs = std::stoi(num_of_vrs_str);

        for(int j=0; j<num_of_vrs; j++){                           //Receive the new bloom_filters and update the existing ones.
            char       *bloom_string = NULL;
            long        pid_id       = 0;
            std::string temp_vrs     = "";

            receive_bloom_filter(buffer_size,bloom_size,crashed_monitor->get_pipe_write(),crashed_monitor->get_pipe_read(),pid_id,&bloom_string,temp_vrs);
            crashed_monitor->add_bloom_filter(bloom_string,temp_vrs);
         }

        receive_message(buffer_size,crashed_monitor->get_pipe_write(),crashed_monitor->get_pipe_read(),message); //Receive "READY" message.
        

        if(message != "PROCESS_READY"){
            std::cout << "~> Error: Process not initialized." << std::endl;
         }
     }
    sleep(1); //A sleep for one second, only for printing purposes (if the sleep does not happen the travelMonitor and Monitor prints overlap).
    std::cout << std::endl;
}
