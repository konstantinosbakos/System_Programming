#include <fstream>

#include "threads.hpp"
#include "../user_commands/user_commands.hpp"


void add_new_records(std::string file_name, long bloom_size, skip_list ***vaccine_skip_l, bloom_filter **vacced_bloom, skip_list *information_check, data_base *citizen_info, citizen *Sentinel_Dummy){
    //Add records to Monitor citizen structures.
    int             num_of_words = 0;
    int             num_of_virus = citizen_info->get_vrs_num();
    virus          *virus_list   = citizen_info->get_vrs_ptr();
    std::string     str;
    std::string    *input_array;
    std::ifstream   file(file_name);                   //Open file.

    while(getline(file,str)){
        input_array = split_to_words(str,num_of_words);

        if(is_virus(input_array[5],virus_list,false)){ //Check if virus exists.
            mutex_lock(&insert_citizen_mutex);
            insert_citizen_record(num_of_words,num_of_virus,input_array,vaccine_skip_l,vacced_bloom,citizen_info,information_check);
            mutex_unlock(&insert_citizen_mutex);
         }
        else{                                          //If not create it.
            mutex_lock(&insert_citizen_new_virus_mutex);
            insert_citizen_record_new_virus(num_of_words,bloom_size,input_array,&vaccine_skip_l,&vacced_bloom,information_check,citizen_info,Sentinel_Dummy);
            mutex_unlock(&insert_citizen_new_virus_mutex);
         }

        delete [] input_array;
        input_array = NULL;
     }
}
