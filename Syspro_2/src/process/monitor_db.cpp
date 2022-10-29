#include "process.hpp"
#include "../functions/functions.hpp"

monitor_db::monitor_db(int monitor_num, long bloom_size, pid_t ID, std::string input){
    this->vrs_list      = NULL; 
    this->pipe_read     = -1;
    this->pipe_write    = -1;
    this->bloom_size    = bloom_size;
    this->monitor_num   = monitor_num;
    this->monitor_pid   = ID;
    this->num_of_blooms = 0;
    this->bloom_filters = NULL;

    std::string *split = split_to_words(input,this->num_of_countries);

    this->cntr_array = new std::string[this->num_of_countries];

    for(int i=0; i<this->num_of_countries; i++){                              //Get all the countries that the Monitor will manage,
        this->cntr_array[i] = get_folder_country(split[i]);                   //and store the in the array.
     }

    delete [] split;
    split = NULL;
}

monitor_db::~monitor_db(){
    virus *temp   = this->vrs_list;
    virus *temp_1 = this->vrs_list;

    while(temp){
        temp_1 = temp->get_next();
        delete temp;
        temp   = NULL;
        temp   = temp_1;
    }

    delete [] this->cntr_array;
    this->cntr_array = NULL;

    for(int i=0; i<this->num_of_blooms; i++){
        delete this->bloom_filters[i];
        this->bloom_filters[i] = NULL;
     }
    delete [] this->bloom_filters;
    this->bloom_filters = NULL;
}

virus *monitor_db::get_virus(std::string _virus){                             //Get a pointer to a virus.
    virus *temp = this->vrs_list;

    while(temp){
        if(temp->get_virus() == _virus){
            return temp;
         }
        temp = temp->get_next();
     }
    
    virus *new_virus = new virus(_virus);                                     //If virus does not exist, create it.

    new_virus->set_next(this->vrs_list);
    this->vrs_list = new_virus;

    return new_virus;
}

bool monitor_db::has_virus(std::string _virus){                               //Check if the Monitor has a virus.
    for(int i=0; i<this->num_of_blooms; i++){
        if(this->bloom_filters[i]->get_virus() == _virus){
            return true;
         }
     }
    return false;
 }

bool monitor_db::has_country(std::string _country){                           //Check if the Monitor manages a specific country.
    for(int i=0; i<this->num_of_countries; i++){
        if(this->cntr_array[i] == _country){
            return true;
         }
     }
    return false;
}

bool monitor_db::search_bloom_filter(std::string ID, std::string _virus){     //Search in the bloom filter of a virus of the monitor.
    for(int i=0; i<this->num_of_blooms; i++){
        if(this->bloom_filters[i]->get_virus() == _virus){
            return this->bloom_filters[i]->bloom_filter_search(ID);
         }
     }
    return false;
}

void monitor_db::add_bloom_filter(char *bloom_string, std::string _virus){    //Add a bloom filter to the bloom filter array.
    virus *vrs_temp = NULL;
   
    for(int i=0; i<this->num_of_blooms; i++){
        if(this->bloom_filters[i]->get_virus() == _virus){                    //If bloom filter exists, update it.
            this->update_bloom_filter(bloom_string,_virus);
            return;
         }
     }
    
    //If bloom filter does not exist, expand the array and add it to the latter.
    this->num_of_blooms++;
    
    bloom_filter **new_bloom_filters = new bloom_filter*[this->num_of_blooms];

    if(this->bloom_filters != NULL){
        for(int i=0; i<this->num_of_blooms-1; i++){
            new_bloom_filters[i]   = this->bloom_filters[i];
            this->bloom_filters[i] = NULL;
         }
        
        delete [] this->bloom_filters;
     }

    this->bloom_filters = new_bloom_filters;

    vrs_temp = get_virus(_virus);                                             //Create the virus node (if it does not exist).

    this->bloom_filters[this->num_of_blooms - 1] = new bloom_filter(this->bloom_size,vrs_temp,bloom_string);
}

void monitor_db::update_bloom_filter(char *bloom_string, std::string _virus){ //Update a bloom filter.
    for(int i=0; i<this->num_of_blooms; i++){
        if(this->bloom_filters[i]->get_virus() == _virus){
            this->bloom_filters[i]->set_bloom_filter(bloom_string);
            return;
         }
     }
    
    //If the bloom filter does not exist, add it.
    this->add_bloom_filter(bloom_string,_virus);
}

std::string *monitor_db::get_countries(int &num_of_countries){                //Get the countries' array and their number.
    num_of_countries = this->num_of_countries;
    return this->cntr_array;
}
