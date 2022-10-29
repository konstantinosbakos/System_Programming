#ifndef __PROC_FUNC__
#define __PROC_FUNC__

#include "../functions/functions.hpp"

#define FIRST_PORT   8000

#define SERVER_R     0                                         //Used by monitor to determine the reading named pipe.
#define SERVER_W     1                                         //Used by monitor to determine the reading named pipe.
#define TRVL_CLNT_R  1                                         //Used by travelMonitor to determine the reading named pipe.
#define TRVL_CLNT_W  0                                         //Used by travelMonitor to determine the reading named pipe.

#define IP_BUFFER    100                                       //Buffer used by get_ip_address().
#define EXECVP_BUFF  500                                       //Buffer used during the monitor initialization.

#define SIG_DONE     '0'                                       //Signal that the operation is over.
#define SIG_NOT_DONE '1'                                       //Signal that the operation is not over.

#define LOG(x)       "log_files/log_file." + std::to_string(x) //Used by "export_log" function to create the log_file names.

//---------bool_process_functions.cpp---------
bool init_tcp(int type, int port, int **sockets, int **socket_id, struct sockaddr_in **server);
bool receive_ready_check(int socket_buffer_size, int num_of_monitors, monitor_db **monitor_array);
bool send_ready_check(int socket_buffer_size, int continue_loop, int socket_write, int socket_read);
bool init_monitors(int num_of_threads, int num_of_monitors, int socket_buffer_size, int cyclic_buffer_size, long bloom_size, std::string *monitor_inputs, monitor_db ***monitor_array);

//---------void_process_functions.cpp---------
void fork_error();
void recv_error();
void send_error();
void delete_array(char *array);
void update_travel_blooms(int socket_buffer_size, int bloom_size, monitor_db *monitor);
void send_message(int socket_buffer_size, int socket_write, int socket_read, std::string message);
void export_log(int accepted_req, int rejected_req, int num_of_folders, std::string *folder_array);
void receive_message(int socket_buffer_size, int socket_write, int socket_read, std::string &message);
void send_message(int socket_buffer_size, int bloom_size, int socket_write, int socket_read, char *message);
void receive_message(int socket_buffer_size, long bloom_size, int socket_write, int socket_read, char **message);
void get_travel_blooms(int socket_buffer_size, long bloom_size, int num_of_monitors, monitor_db **monitor_array);
void send_bloom_filter(int socket_buffer_size, long bloom_size, int num_of_vrs, int socket_write, int socket_read, bloom_filter **bloom);
void receive_bloom_filter(int socket_buffer_size, int bloom_size, int socket_write, int socket_read, char **bloom_string, std::string &_virus);
void send_to_countryTo(int socket_buffer_size, std::string date, std::string countryTo, std::string _virus, monitor_db *countryTo_monitor, bool accepted);

//------------process_functions.cpp-----------
char *get_ip_address();

#endif