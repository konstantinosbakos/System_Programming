#ifndef __FUNC__
#define __FUNC__

#include <iostream>

#include "../process/process.hpp"
#include "../skip_list/skip_list.hpp"
#include "../bloom_filter/bloom_filter.hpp"

#define TRVL_RQ       1  //Travel Request.
#define TRVL_STS      2  //Travel Stats.
#define ADD_VAC_REC   3  //Add vaccination records.
#define SRCH_VAC_STS  4  //Search vaccination status.
#define HELP          5  //Print help text.
#define TERMINATE     6  //Terminate the program.
#define COUNTRY_TO_M  7  //Receive a request's information.
#define EXPORT_LOG    8  //Export the monitor's/travelMonitor's log_file.
#define PRINT_PIDS    9  //Print the current Monitors' pids.
#define CHLD_CRASH   10  //Restard a child.
#define WRONG_INPUT  -1

#define INPUT_BUFF   500 //The size used for fgets (for the user's input).

#define OLDEST_DATE  "14-5-1796"

#define DELETE_ARRAYS delete [] num_ctzn; delete [] num_1; delete [] num_2; num_ctzn = NULL; num_1 = NULL; num_2 = NULL; //Used in compare dates.

//------------functions.cpp-----------
std::string get_todays_date(); //Returns today's date.

//---------bool_functions.cpp---------
bool coin_toss();
bool is_number(char chr);                                                                               //Check if character is a number.
bool is_number(std::string str);                                                                        //Checks if string is number.
bool check_date_format(std::string date);                                                               //Check if date format is okay.
bool is_virus(std::string vrs, virus *virus_list, bool print);                                          //If virus does not exists return false.
bool credential_check(citizen *existing_ct, std::string *input_array, int i=0);                         //Check if citizen crendetials match the ones in the data base.
bool compare_dates(std::string ctzn_date, std::string date_1, std::string date_2);                      //Checks if ctzn_date is between date_1 and date_2.
bool process_subfolders(int &num_of_monitors, int &num_of_subfolders, std::string folder_name, std::string **subfolders, std::string **monitor_inputs);

inline bool is_valid_date(std::string date){return compare_dates(date,OLDEST_DATE,get_todays_date());}; //Return if date is valid.

//---------void_functions.cpp---------
void welcome();                                                                                         //welcome message for the user.
void print_bar(int length);
void print_space(int length);
void print_tilde(int length);
void print_underscore(int length);
void remove_directory(std::string dir_name);
void get_command(int &command, std::string **input_array, int &num_of_words);                           //Get user input.
void compare_folder_names(std::string &name_1, std::string &name_2);
void vaccine_status(int ID, int num_of_vrs, skip_list ***vaccine_skip_l, std::string &citizen_viruses);
void init_monitor_basics(int &buffer_size, long &bloom_size, int **pipe, char **pipe_names, std::string &folder_names);
void expand_bloom_skip_arrays(int num_of_vrs, long bloom_size, skip_list ****vaccine_skip_l, bloom_filter ***vacced_bloom, virus *head_vrs,citizen *Sentinel_Dummy); //Create structures for new virus.
void insert_citizen_record(int num_of_words, int num_of_vrs, std::string *input_array, skip_list ***vaccine_skip_l, bloom_filter **vacced_bloom, data_base *citizen_info, skip_list *information_check);
void add_new_records(std::string folder_name, long bloom_size, skip_list ***vaccine_skip_l, bloom_filter **vacced_bloom, skip_list *information_check, data_base *citizen_info, citizen *Sentinel_Dummy);
void init_monitor_citizen_structures(long bloom_size, skip_list ****vaccine_skip_l, bloom_filter ***vacced_bloom, skip_list *information_check, data_base *citizen_info, citizen *Sentinel_Dummy, folder_db *monitor_folders);
void add_new_records(int num_of_files, std::string folder_name, std::string *files, long bloom_size, skip_list ***vaccine_skip_l, bloom_filter **vacced_bloom, skip_list *information_check, data_base *citizen_info, citizen *Sentinel_Dummy);
void insert_citizen_record_new_virus(int num_of_words, long bloom_size, std::string *input_array, skip_list ****vaccine_skip_l, bloom_filter ***vacced_bloom, skip_list *information_check, data_base *citizen_info, citizen *Sentinel_Dummy);
void add_to_data_base(int num_of_files, long bloom_size, std::string folder_name, std::string *file_array, bloom_filter ***vacced_bloom, skip_list ****vaccine_skip_l, skip_list *information_check, data_base *citizen_info, citizen *Sentinel_Dummy);

//------------functions.cpp-----------
int  *date_to_int(std::string str_date);                                                                                    //Turn string date array to int array.

char *pipe_name(int i, int choice);

std::string *break_date(std::string _date);                                                                                 //Break the string of the date into an array of DD/MM/YY.
std::string  get_folder_country(std::string str);
std::string *get_files(std::string folder_name, int &num_of_files);                                                         //Get the names of the files.
std::string *split_to_words(std::string user_input, int &num_of_words);                                                     //Split and trim string.
std::string *get_subfolders(std::string folder_name, int &num_of_subfolders);
std::string *sort_items(std::string *subfolders, int num_of_items);                                                         //Bubble-sort.
std::string *get_monitor_input_RR(std::string *subfolders, int &num_of_monitors, int num_of_subfolders);                    //Round_Robin

skip_list ***create_skip_lists(data_base *db, citizen *Sentinel_Dummy);                                                     //Create skip lists.
skip_list   *get_skip_list(int num_of_vrs, std::string virus, skip_list ***vaccine_skip_l, bool is_vacc);                   //Get the skip list of a virus.

monitor_db *get_country_monitor(int num_of_monitors, std::string _country, monitor_db **monitor_array);

bloom_filter **create_bloom_filters(long bloom_size,data_base *data);                                                       //Create bloom filters.
bloom_filter  *get_bloom_filter(int num_of_vrs, std::string virus, bloom_filter **vacced_bloom);                            //Get the bloom filter of a specific virus.

country_records *get_country_record(std::string _country, int num_of_folders, country_records **monitor_countries);

//------init_delete_functions.cpp-----
bool init_program(int argc, char** argv, int &num_of_monitors, int &buffer_size, long &bloom_size, std::string &file_name); //User command line input.

void init_directories();
void delete_TRQ_dates(std::string *response_array, int *input_date=NULL, int *vaccine_date=NULL);void init_monitor_structures(bloom_filter **vacced_bloom, skip_list ***vaccine_skip_l, data_base *db,skip_list *information_check);                                          //Initialize structures.
void delete_monitor_citizen_structures(data_base *citizen_info, skip_list  ***vaccine_skip_l, bloom_filter **vacced_bloom, skip_list *information_check, citizen *Sentinel_Dummy);
void delete_travel_monitor_structures(int num_of_monitors, char ***pipe_array, std::string *subfolders, std::string *monitor_inputs, monitor_db **monitor_array=NULL);
void delete_monitor_structures(int num_of_folders, int *pipe, folder_db *monitor_folders, std::string *folder_array, country_records **monitor_countries);

#endif