#ifndef __USR_CMD__
#define __USR_CMD__

#include "../functions/functions.hpp"
#include "../process_functions/process_functions.hpp"

//-------------Travel-Request--------------
int travel_request_send(int socket_buffer_size, int num_of_monitors, std::string *input_array, monitor_db **monitor_array);

void add_to_destination(int socket_buffer_size, int num_of_folders, int socket_write, int socket_read, virus *vrs_list, country_records **monitor_countries);
void travel_request_receive(int socket_buffer_size, int socket_write, int socket_read, skip_list ***vaccine_skip_l, skip_list *information_check, data_base *citizen_info);

//--------------Travel-Stats---------------
void travel_stats(int socket_buffer_size, int num_of_words, int num_of_monitors, std::string *input_array, monitor_db **monitor_array);
void travel_stats_monitor(int socket_buffer_size, int num_of_folders, int socket_write, int socket_read, country_records **monitor_countries);

//---------Add-Vaccination-Records---------
void add_vaccination_records(int socket_buffer_size, int num_of_monitors, long bloom_size, std::string *input_array, monitor_db **monitor_array);
void add_vaccination_records_monitor(int socket_buffer_size, int socket_write, int socket_read, int num_of_folders, folder_db **monitor_folders);

//--------Search-Vaccination-Status--------
void search_vaccination_status(int socket_buffer_size, int num_of_monitors, std::string *input_array, monitor_db **monitor_array);
void search_vaccination_status_monitor(int socket_buffer_size, int num_of_vrs, int socket_write, int socket_read, skip_list ***vaccine_skip_l, skip_list *information_check);

//-----------Misc-User-Commands------------
void help();
void export_server_log(int num_of_folders, std::string *folder_array, country_records **monitor_countries);
void terminate(int socket_buffer_size, int accepted_req, int rejected_req, int num_of_monitors, int num_of_subfolders, std::string *subfolders, monitor_db **monitor_array);

#endif