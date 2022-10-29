#ifndef __FUNC__
#define __FUNC__

#include <iostream>

#include "../skip_list/skip_list.hpp"
#include "../bloom_filter/bloom_filter.hpp"

#define VAC_STAT_BM   1 //Vaccine status bloom.
#define VAC_STAT_VR   2 //Vaccine status with virus.
#define VAC_STAT      3 //Vacinne status without virus.
#define POP_STAT      4 //Population status.
#define POP_STAT_AGE  5 //Population status with age.
#define INSERT_CTZN   6 //Insert citizen.
#define VAC_NOW       7 //Vaccinate now.
#define LS_NON_VAC    8 //List-non-vaccinated.
#define STM_VRS       9 //Print viruses in system.
#define HELP         10
#define TERMINATE    11
#define WRONG_INPUT  -1

#define DELETE_ARRAYS delete [] num_ctzn; delete [] num_1; delete [] num_2; num_ctzn = NULL; num_1 = NULL; num_2 = NULL; //Used in compare dates.

//---------functions.cpp---------
std::string get_todays_date(); //Returns today's date.

//---------bool_functions.cpp---------
bool coin_toss();
bool is_number(std::string str);                                                    //Checks if string is number.
bool is_string(std::string str);                                                    //If string contains numbers return false.
bool is_valid_age(int ID,int age);                                                  //Checks if age is valid.
bool is_valid_YES_NO(std::string str);                                              //Check if string is YES or NO.
bool check_date_format(std::string date);                                           //Check if date format is okay.
bool find_country(std::string _country, skip_list *list);                           //Check if country is in the skip list.
bool is_country(std::string _country, country *cntr_list);                          //If string is not country return false.
bool is_virus(std::string vrs, virus *virus_list, bool print);                      //If virus does not exists return false.
bool is_citizen_vaccinated(int num_of_words, std::string date);                     //Check if citizen is vaccinated.
bool credential_check(citizen *existing_ct, std::string *Command_Array, int i=0);   //Check if citizen crendetials match the ones in the data base.
bool compare_dates(std::string ctzn_date, std::string date_1, std::string date_2);  //Checks if ctzn_date is between date_1 and date_2.
bool program_init(int argc, char** argv, long &bloom_size, std::string &file_name); //Read command line input.

inline bool is_valid_date(std::string date){return compare_dates(date,"14-5-1796",get_todays_date());}; //Return if date is valid.

//---------void_functions.cpp---------
void Welcome();                                                                              //Welcome message for the user.
void print_country(std::string country);                                                     //Used by population_status_by_age to beatify the output.
void get_command(int &command, std::string **Command_Array, int &num_of_words);              //Get user input.
void init_structures(bloom_filter **vacced_bloom, skip_list ***vaccine_skip_l, data_base *db,skip_list *information_check);                                          //Initialize structures.
void delete_structures(data_base *citizen_info, skip_list  ***vaccine_skip_l, bloom_filter **vacced_bloom, skip_list *information_check, citizen *Sentinel_Dummy);   //Delete structures.
void expand_bloom_skip_arrays(int num_of_vrs, long bloom_size, skip_list ****vaccine_skip_l, bloom_filter ***vacced_bloom, virus *head_vrs,citizen *Sentinel_Dummy); //Create structures for new virus.

//---------functions.cpp---------
int *date_to_int(std::string *date);                                                                      //Turn string date array to int array.

std::string *break_date(std::string _date);                                                               //Break the string of the date into an array of DD/MM/YY.
std::string *split_to_words(std::string user_input, int &num_of_words);                                   //Split and trim string.

skip_list ***create_skip_lists(data_base *db, citizen *Sentinel_Dummy);                                   //Create skip lists.
skip_list   *get_skip_list(int num_of_vrs, std::string virus, skip_list ***vaccine_skip_l, bool is_vacc); //Get the skip list of a virus.

bloom_filter **create_bloom_filters(long bloom_size,data_base *data);                                     //Create bloom filters.
bloom_filter  *get_bloom_filter(int num_of_vrs, std::string virus, bloom_filter **vacced_bloom);          //Get the bloom filter of a specific virus.

#endif