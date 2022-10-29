#include "data_base.hpp"

void data_base::remove(name *ptr){ //Remove a name/virus/country/surname from the data base.
    name *temp = this->nam_head;
    name *prev = this->nam_head;

    while(temp){//Find the name/virus/country/surname and remove it.
        if(temp == ptr){
            if(this->nam_head == ptr){
                this->nam_head = this->nam_head->get_next();
                delete ptr;
                ptr = NULL;
                return;
             }
            else{
                prev->set_next(ptr->get_next());
                delete ptr;
                ptr = NULL;
                return;
             }
         }
        prev = temp;
        temp = temp->get_next();          
     }
}

void data_base::remove(virus *ptr){
    virus *temp = this->vrs_head;
    virus *prev = NULL;

    while(temp){
        if(temp == ptr){
            if(this->vrs_head == ptr){
                this->vrs_head = ptr->get_next();
                delete ptr;
                ptr = NULL;
                return;
             }
            else{
                prev->set_next(ptr->get_next());
                delete ptr;
                ptr = NULL;
                return;
             }
         }
        prev = temp;
        temp = temp->get_next();
     }
}

void data_base::remove(country *ptr){
    country *temp = this->cntr_head;
    country *prev = NULL;

    while(temp){
        if(temp == ptr){
            if(this->cntr_head == ptr){
                this->cntr_head = ptr->get_next();
                delete ptr;
                ptr = NULL;
                return;
             }
            else{
                prev->set_next(ptr->get_next());
                delete ptr;
                ptr = NULL;
                return;
             }
         }
        prev = temp;
        temp = temp->get_next();
     }
}

void data_base::remove(surname *ptr){
    surname *temp = this->surnam_head;
    surname *prev = NULL;

    while(temp){
        if(temp == ptr){
            if(this->surnam_head == ptr){
                this->surnam_head = ptr->get_next();
                delete ptr;
                ptr = NULL;
                return;
             }
            else{
                prev->set_next(ptr->get_next());
                delete ptr;
                ptr = NULL;
                return;
             }
         }
        prev = temp;
        temp = temp->get_next();
     }
}

name *data_base::check_name(std::string _name){  //Check if name/virus/country/surname exists, or add new name to list.
    name *temp      = this->nam_head;
    name *prev_temp = NULL;

    if(temp){
        while(temp){                            //Find nam and return it.
            if(temp->get_name() == _name){
                temp->increase_usage();         //Increase the number of citizens that use this name/virus/country/surname.
                return temp;
             }
            prev_temp = temp;
            temp = temp->get_next();
         }

        name *new_name = new name(_name);       //Or create new name/virus/country/surname and return it.
        
        prev_temp->set_next(new_name);
        new_name->increase_usage();
        new_name->set_next(NULL);

        return new_name;
     }
    else{                                       //If list is empty add the new node to the head.
        name *new_name = new name(_name);

        this->nam_head = new_name;
        new_name->increase_usage();
        new_name->set_next(NULL);

        return new_name;
     }
    return NULL;
}

virus *data_base::check_virus(std::string _virus){
    virus *temp      = this->vrs_head;
    virus *prev_temp = NULL;

    if(temp){
        while(temp){
            if(temp->get_virus() == _virus){
                temp->increase_usage();
                return temp;
             }
            prev_temp = temp;
            temp = temp->get_next();
         }

        virus *new_virus = new virus(_virus);

        prev_temp->set_next(new_virus);
        new_virus->increase_usage();
        new_virus->set_next(NULL);
        this->vrs_num++;                        //Increase virus number.

        return new_virus;
     }
    else{
        virus *new_virus = new virus(_virus);

        this->vrs_head = new_virus;
        new_virus->increase_usage();
        new_virus->set_next(NULL);
        this->vrs_num++;

        return new_virus;       
     }
    return NULL;
}

country *data_base::check_country(std::string _country){
    country *temp      = this->cntr_head;
    country *prev_temp = NULL;

    if(temp){
        while(temp){
            if(temp->get_country() == _country){
                temp->increase_usage();
                return temp;
             }
            prev_temp = temp;
            temp = temp->get_next();
         }

        country *new_country = new country(_country);

        prev_temp->set_next(new_country);
        new_country->increase_usage();
        new_country->set_next(NULL);

        return new_country;
     }
    else{
        country *new_country = new country(_country);

        this->cntr_head = new_country;
        new_country->increase_usage();
        new_country->set_next(NULL);
        
        return new_country;
     }
    return NULL;
}

surname *data_base::check_surname(std::string _surname){
    surname *temp      = this->surnam_head;
    surname *prev_temp = NULL;

    if(temp){
        while(temp){
            if(temp->get_surname() == _surname){
                temp->increase_usage();
                return temp;
             }
            prev_temp = temp;
            temp = temp->get_next();
         }

        surname *new_surname = new surname(_surname);
        prev_temp->set_next(new_surname);
        new_surname->increase_usage();
        new_surname->set_next(NULL);

        return new_surname;
     }
    else{
        surname *new_surname = new surname(_surname);

        this->surnam_head = new_surname;
        new_surname->increase_usage();
        new_surname->set_next(NULL);

        return new_surname;
     }
    return NULL;
}