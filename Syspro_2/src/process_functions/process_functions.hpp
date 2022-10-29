#ifndef __PROC_FUNC__
#define __PROC_FUNC__

#include "../functions/functions.hpp"

#define MON_R       0                                        //Used by monitor to determine the reading named pipe.
#define MON_W       1                                        //Used by monitor to determine the reading named pipe.
#define MON_CLR     2                                        //Used by monitor to flush the writing pipe when a signal is received.
#define TRVL_MON_R  1                                        //Used by travelMonitor to determine the reading named pipe.
#define TRVL_MON_W  0                                        //Used by travelMonitor to determine the reading named pipe.

#define HNDSHK_SIZE 10000                                    //Initial (handshake) buffer size.

#define SIG_DONE     '0'                                     //Signal that the operation is over.
#define SIG_NOT_DONE '1'                                     //Signal that the operation is not over.

#define LOG(x)       "log_files/log_file." + std::to_string(x) //Used by "export_log" function to create the log_file names.

extern int no_print;
extern int command;

//-------------signal_handlers.cpp------------
void monitor_signal_handler(int signal);
void travel_monitor_signal_handler(int signal);

//---------bool_process_functions.cpp---------
bool init_named_pipes(char ****pipe_array, int num_of_monitors);
bool send_ready_check(int buffer_size, int continue_loop, int pipe_write, int pipe_read);
bool receive_ready_check(int buffer_size, int num_of_monitors, monitor_db **monitor_array);
bool init_monitors(int buffer_size, int num_of_monitors, long bloom_size, pid_t pid_exec, char ***pipe_array, std::string *monitor_inputs, monitor_db ***monitor_array);

//---------void_process_functions.cpp---------
void fork_error();
void open_error();
void read_error();
void fgets_error();
void write_error();
void mkfifo_error();
void clear_pipe(int pipe, int buffer_size);
void create_named_pipes(char ****pipe_array, int num_of_monitors);
void receive_handshake(int &buffer_size, int **pipe, char **pipe_names);
void send_handshake(int buffer_size, char ***pipe_array, monitor_db **monitor);
void update_travel_blooms(int buffer_size, int bloom_size, monitor_db *monitor);
void send_message(int buffer_size, int pipe_write, int pipe_read, std::string message);
void receive_message(int buffer_size, int  pipe_write, int  pipe_read, std::string &message);
void export_log(int accepted_req, int rejected_req, int num_of_folders, std::string *folder_array);
void send_message(int buffer_size, int bloom_size, int  pipe_write, int  pipe_read, char *message);
void receive_message(int buffer_size, long bloom_size, int  pipe_write, int  pipe_read, char **message);
void get_travel_blooms(int buffer_size, long bloom_size, int num_of_monitors, monitor_db **monitor_array);
void send_bloom_filter(int buffer_size, long bloom_size, int num_of_vrs, int pipe_r, int pipe_w, long pid_id, bloom_filter **bloom);
void receive_bloom_filter(int buffer_size, int bloom_size, int pipe_w, int pipe_r, long &pid_id, char **bloom_string, std::string &_virus);
void send_to_countryTo(int buffer_size, std::string date, std::string countryTo, std::string _virus, monitor_db *countryTo_monitor, bool accepted);
void restart_child(int buffer_size, int num_of_monitors, char ***pipe_array, long bloom_size, monitor_db **monitor_array, std::string *monitor_inputs);

#endif