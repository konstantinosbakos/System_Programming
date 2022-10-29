#include <unistd.h>
#include <sys/stat.h>
#include <netdb.h> 	
#include <arpa/inet.h>

#include "process_functions.hpp"

bool init_tcp(int type, int port, int **sockets, int **socket_id, struct sockaddr_in **server){
    int           condition  =  1;
    char         *ip_address =  get_ip_address();
    unsigned int  server_len =  sizeof (*server)[SERVER_R];

    if(!ip_address){                                                                     //If IP not found return false.
        return false;
     }

    (*server)[type].sin_family      = AF_INET;
    (*server)[type].sin_port        = htons(port);
    (*server)[type].sin_addr.s_addr = inet_addr(ip_address);                             //Use the machine's address.

    if(((*socket_id)[type] = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == -1){            //Initialize TCP connection.
        perror("Socket");

        delete_array(ip_address);
        return false;
     }
    
    setsockopt((*socket_id)[type],SOL_SOCKET,SO_REUSEADDR,&condition,sizeof(int));       //Clear the socket (to be able to bind).

    if(bind((*socket_id)[type],(struct sockaddr *)&((*server)[type]),server_len) == -1){ //Bind the socket.
        perror("Bind");

        delete_array(ip_address);
        return false;
     }

    if(listen((*socket_id)[type],1) == -1){                                              //Start listening to the socket.
        perror("Listen");

        delete_array(ip_address);
        return false;
     }
    else{                                                                                //Wait to accept the connection.
        (*sockets)[type] = accept((*socket_id)[type],(struct sockaddr *)&((*server)[type]),&server_len);
     }

    delete_array(ip_address);
    return true;
}

bool receive_ready_check(int socket_buffer_size, int num_of_monitors, monitor_db **monitor_array){ //Receive message that all Monitors are ready to operate.
    for(int i=0; i<num_of_monitors; i++){
        std::string message = "";
        
        receive_message(socket_buffer_size,monitor_array[i]->get_socket_write(),monitor_array[i]->get_socket_read(),message);
        if(message != "PROCESS_READY"){
            return false;
         }
      }
    sleep(1); //ReadMe.txt for clarification of the usage.
    
    return true;
}

bool send_ready_check(int socket_buffer_size, int continue_loop, int socket_write, int socket_read){ //Write message that the Monitor is ready to operate.
    if(continue_loop){
        std::string message = "PROCESS_READY";
        
        send_message(socket_buffer_size,socket_write,socket_read,message);
        return true;
     }
    else{
        std::string message = "PROCESS_READY";
        
        send_message(socket_buffer_size,socket_write,socket_read,message);
        return false;
     }
}

bool init_monitors(int num_of_threads, int num_of_monitors, int socket_buffer_size, int cyclic_buffer_size, long bloom_size, std::string *monitor_inputs, monitor_db ***monitor_array){
    //A function used by travelMonitor to initialize the Monitor processes.
    char  *ip_address  = get_ip_address();
    pid_t  pid_exec    = getpid();

    (*monitor_array)  = new monitor_db*[num_of_monitors];

    for(int i=0; i<num_of_monitors; i++){    
        int connection_id_W = -1;
        int connection_id_R = -1;

        (*monitor_array)[i] = new monitor_db(i,bloom_size,monitor_inputs[i]);
        
        (*monitor_array)[i]->set_port_write(FIRST_PORT + 2 * (i + 1));
        (*monitor_array)[i]->set_port_read( FIRST_PORT + 2 * (i + 1) + 1);

        (*monitor_array)[i]->server_connections[TRVL_CLNT_W].sin_family      = AF_INET;
        (*monitor_array)[i]->server_connections[TRVL_CLNT_W].sin_port        = htons((*monitor_array)[i]->get_port_write());
        (*monitor_array)[i]->server_connections[TRVL_CLNT_W].sin_addr.s_addr = inet_addr(ip_address);

        (*monitor_array)[i]->server_connections[TRVL_CLNT_R].sin_family      = AF_INET;
        (*monitor_array)[i]->server_connections[TRVL_CLNT_R].sin_port        = htons((*monitor_array)[i]->get_port_read());
        (*monitor_array)[i]->server_connections[TRVL_CLNT_R].sin_addr.s_addr = inet_addr(ip_address);

        unsigned int server_len_R = sizeof (*monitor_array)[i]->server_connections[TRVL_CLNT_W];
        unsigned int server_len_W = sizeof (*monitor_array)[i]->server_connections[TRVL_CLNT_R];

        if((connection_id_R = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP)) == -1){
            perror("Socket");
            exit(0);
         }
        if((connection_id_W = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP)) == -1){
            perror("Socket");
            exit(0);
         }

        if((pid_exec = fork()) == -1){ //Fork the process.
            fork_error();
            return false;
         }
        if(pid_exec == 0){             //Execute the new monitor.
            char port_ch[EXECVP_BUFF];
            char bloom_size_ch[EXECVP_BUFF];
            char num_of_threads_ch[EXECVP_BUFF];
            char socket_buffer_size_ch[EXECVP_BUFF];
            char cyclic_buffer_size_ch[EXECVP_BUFF];

            sprintf(bloom_size_ch,"%ld",bloom_size);
            sprintf(num_of_threads_ch,"%d",num_of_threads);
            sprintf(socket_buffer_size_ch,"%d",socket_buffer_size);
            sprintf(cyclic_buffer_size_ch,"%d",cyclic_buffer_size);
            sprintf(port_ch,"%d",(*monitor_array)[i]->get_port_write());

            char * const initial_input[] = {(char *)("./monitorServer"),(char *)("-p"),port_ch,(char *)("-t"),num_of_threads_ch,(char *)("-b"),socket_buffer_size_ch,(char *)("-c"),cyclic_buffer_size_ch,(char *)("-s"),bloom_size_ch,(char *)monitor_inputs[i].c_str(), nullptr};
            execvp("./monitorServer",initial_input);
         }
        else{                          //Initialize the monitor structure for the specific monitor.
            int error_W = -1;
            int error_R = -1;

            while(error_W < 0){
                error_W = connect(connection_id_W,(struct sockaddr *)&((*monitor_array)[i]->server_connections[TRVL_CLNT_W]),server_len_W);
             }
            while(error_R < 0){
                error_R = connect(connection_id_R,(struct sockaddr *)&((*monitor_array)[i]->server_connections[TRVL_CLNT_R]),server_len_R);
             }

            (*monitor_array)[i]->set_socket_write(connection_id_W);
            (*monitor_array)[i]->set_socket_read(connection_id_R);
         }
     }

    delete [] ip_address;
    ip_address = NULL;

    return true;
}