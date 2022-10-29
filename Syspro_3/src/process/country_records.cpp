#include "process.hpp"
#include "../functions/functions.hpp"

date_list::date_list(std::string date, virus *_virus){
    this->date           = date;
    this->next           = NULL;
    this->_virus         = _virus;
    this->num_of_acc_req = 0;
    this->num_of_rej_req = 0;
}

date_list::~date_list(){
    this->next = NULL;
}

std::string date_list::get_virus(){return this->_virus->get_virus();} //Return the virus string.

country_records::country_records(std::string _country){
    this->dates           = NULL;
    this->_country        = _country;
}

country_records::~country_records(){
    date_list *temp   = this->dates;
    date_list *temp_1 = NULL;

    while(temp){
        temp_1 = temp->get_next();
        delete temp;
        temp = NULL;
        temp = temp_1;
     }
}

int country_records::get_num_of_accepted(){ //Get the number of the accepted requests to the country.
    int        accepted = 0;
    date_list *temp     = this->dates;

    while(temp){
        accepted += temp->get_num_of_acc_req();
        temp = temp->get_next();
     }
    return accepted;
}

int country_records::get_num_of_rejected(){ //Get the number of the rejected requests to the country.
    int        rejected = 0;
    date_list *temp     = this->dates;

    while(temp){
        rejected += temp->get_num_of_rej_req();
        temp = temp->get_next();
     }
    return rejected;
}

void country_records::update_date_stats(int req, std::string date, virus *_virus){ //Update the statistics for a date (for a specific virus).
    date_list *temp = this->dates;

    while(temp){
        if(temp->get_date() == date && temp->get_virus() == _virus->get_virus()){  //If the date does exist for the virus, update it.
            if(req == ACCEPTED_REQ){
                temp->increase_num_of_acc_req();
             }
            else{
                temp->increase_num_of_rej_req();
             }  
            return;
         }
        temp = temp->get_next();
     }

    date_list *new_node = new date_list(date,_virus);                              //If it does not, create it.
    new_node->set_next(this->dates);
    this->dates = new_node;

    if(req == ACCEPTED_REQ){
        new_node->increase_num_of_acc_req();
     } 
    else{
        new_node->increase_num_of_rej_req();
     }
}

void country_records::get_period_stats(int &accepted, int &rejected, std::string _virus, std::string date_start, std::string date_finish){
    //Get the statistics for a date range for a virus.
    date_list *temp_date = this->dates;

    while(temp_date){
        if(compare_dates(temp_date->get_date(),date_start,date_finish) && temp_date->get_virus() == _virus){
            accepted += temp_date->get_num_of_acc_req();
            rejected += temp_date->get_num_of_rej_req();
         }
        temp_date = temp_date->get_next();
     }
}
