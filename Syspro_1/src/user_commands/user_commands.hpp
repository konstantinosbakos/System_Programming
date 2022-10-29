#ifndef __USR_CMD__
#define __USR_CMD__

#include "../functions/functions.hpp"

#define SET_PR(x)      std::fixed << std::setprecision(2) << x //Used for float precision.

//-----Vaccine-User-Commands-----
void vaccine_status(std::string ID, bloom_filter *bloom);
void vaccine_status(int ID,skip_list *skip_l);
void vaccine_status(int ID, int num_of_vrs, skip_list ***vaccine_skip_l);
//-------Population-Status-------
void population_status_one(int num_of_vrs, skip_list ***vaccine_skip_l, std::string *Command_Array);
void population_status_all(int num_of_vrs, country *cntr_list, skip_list ***vaccine_skip_l, std::string *Command_Array);
void population_status_by_age_one(int num_of_vrs, skip_list *vac_skip_l, skip_list *not_vac_skip_l, std::string *Command_Array);
void population_status_by_age_all(skip_list *vac_skip_l ,skip_list *not_vac_skip_l, country *cntr_list, std::string *Command_Array);
//------Insert-And-Vaccinate-----
void insert_citizen_record_new_virus(int num_of_words, long bloom_size, std::string *Command_Array, skip_list ****vaccine_skip_l, bloom_filter ***vacced_bloom, skip_list *information_check, data_base *citizen_info, citizen *Sentinel_Dummy);
void insert_citizen_record(int num_of_words, int num_of_vrs, std::string *Command_Array, skip_list ***vaccine_skip_l, bloom_filter **vacced_bloom, data_base *citizen_info, skip_list *information_check);
void vaccinate_now_new_virus(int nlm_of_words, long bloom_size, std::string *Command_Array, skip_list ****vaccine_skip_l, bloom_filter ***vacced_bloom, skip_list *information_check, data_base *citizen_info, citizen *Sentinel_Dummy);
void vaccinate_now(int num_of_vrs, int num_of_words, virus *vrs_list, std::string *Command_Array, skip_list ***vaccine_skip_l, bloom_filter **vacced_bloom, data_base *citizen_info, skip_list *information_check);
//-------Misc-User-Commands------
void list_non_vaccinated_persons(skip_list *vrs_skip_list);
void system_viruses(virus *vrs_list);
void help();


#endif