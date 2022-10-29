#ifndef __USR_CMD__
#define __USR_CMD__

#include "../functions/functions.hpp"
#include "../process_functions/process_functions.hpp"

//-------------Travel-Request--------------
int travel_request_send(int buffer_size, int num_of_monitors, std::string *input_array, monitor_db **monitor_array);

void add_to_destination(int buffer_size, int num_of_folders, int pipe_write, int pipe_read, virus *vrs_list, country_records **monitor_countries);
void travel_request_receive(int buffer_size, int pipe_write, int pipe_read, skip_list ***vaccine_skip_l, skip_list *information_check, data_base *citizen_info);

//--------------Travel-Stats---------------
void travel_stats(int buffer_size, int num_of_words, int num_of_monitors, std::string *input_array, monitor_db **monitor_array);
void travel_stats_monitor(int buffer_size, int num_of_folders, int pipe_write, int pipe_read, country_records **monitor_countries);

//---------Add-Vaccination-Records---------
void add_vaccination_records(int buffer_size, int num_of_monitors, char ***pipe_array, long bloom_size, std::string *input_array, monitor_db **monitor_array);
void add_vaccination_records_monitor(int buffer_size, int *pipe, folder_db *monitor_folders, long bloom_size, skip_list ***vaccine_skip_l, bloom_filter **vacced_bloom, skip_list *information_check, data_base *citizen_info, citizen *Sentinel_Dummy);

//Search-Vaccination-Status
void search_vaccination_status(int buffer_size, int num_of_monitors, std::string *input_array, monitor_db **monitor_array);
void search_vaccination_status_monitor(int buffer_size, int num_of_vrs, int pipe_write, int pipe_read, skip_list ***vaccine_skip_l, skip_list *information_check);

//-----------Misc-User-Commands------------
void help();
void print_monitor_pids(int num_of_monitors, monitor_db **monitor_array);
void export_monitor_log(int num_of_folders, std::string *folder_array, country_records **monitor_countries);
void terminate(int buffer_size, int accepted_req, int rejected_req, int num_of_monitors, int num_of_subfolders, std::string *subfolders, monitor_db **monitor_array);

#endif