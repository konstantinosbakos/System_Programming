#include <unistd.h>
#include <sys/stat.h>

#include "process_functions.hpp"

bool init_named_pipes(char ****pipe_array, int num_of_monitors){                            //Create the named pipes, two for each process.
    for(int i=0; i<num_of_monitors; i++){
        if(mkfifo((*pipe_array)[i][0],0666) == -1 || mkfifo((*pipe_array)[i][1],0666) == -1 ){
            mkfifo_error();            
            return false;
         }
     }
    return true;
}   

bool send_ready_check(int buffer_size, int continue_loop, int pipe_write, int pipe_read){   //Send message that the Monitor is ready to operate.
    if(continue_loop){
        std::string message = "PROCESS_READY";
        
        send_message(buffer_size,pipe_write,pipe_read,message);
        return true;
     }
    else{
        std::string message = "PROCESS_READY";
        
        send_message(buffer_size,pipe_write,pipe_read,message);
        return false;
     }
}

bool receive_ready_check(int buffer_size, int num_of_monitors, monitor_db **monitor_array){ //Receive message that all Monitors are ready to operate.
    for(int i=0; i<num_of_monitors; i++){
        std::string message = "";
        
        receive_message(buffer_size,monitor_array[i]->get_pipe_write(),monitor_array[i]->get_pipe_read(),message);
        if(message != "PROCESS_READY"){
            return false;
         }
      }
    sleep(1);
    
    return true;
}

bool init_monitors(int buffer_size, int num_of_monitors, long bloom_size, pid_t pid_exec, char ***pipe_array, std::string *monitor_inputs, monitor_db ***monitor_array){
    //A function used by travelMonitor to initialize the Monitor processes.
    (*monitor_array) = new monitor_db*[num_of_monitors];
   
    for(int i=0; i<num_of_monitors; i++){
        if((pid_exec = fork()) == -1){ //Fork the process.
            fork_error();
            return false;
         }

        if(pid_exec == 0){             //Execute the new monitor.
            char * const initial_input[] = {(char *)("./Monitor"), (pipe_array[i][TRVL_MON_W]), (pipe_array[i][TRVL_MON_R]), nullptr};
            execvp("./Monitor",initial_input);
         }
        else{                          //Initialize the monitor structure for the specific monitor.
            (*monitor_array)[i] = new monitor_db(i,bloom_size,pid_exec,monitor_inputs[i]);
         }

        send_handshake(buffer_size,pipe_array,&(*monitor_array)[i]);                                                                       //Send the buffer size and establish connection.
        send_message  (buffer_size,(*monitor_array)[i]->get_pipe_write(),(*monitor_array)[i]->get_pipe_read(),std::to_string(bloom_size)); //Send the bloom size.
        send_message  (buffer_size,(*monitor_array)[i]->get_pipe_write(),(*monitor_array)[i]->get_pipe_read(),monitor_inputs[i]);          //Send the countries that the monitor will control.
     }
    return true;
}