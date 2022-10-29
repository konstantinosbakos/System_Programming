#include "process.hpp"

folder_db::folder_db(int num_of_folders, std::string *folder_array){
    this->folder_list = NULL;

    for(int i=0; i<num_of_folders; i++){                     //Initialize the list.
        folder_records *new_node = new folder_records(folder_array[i]);
        new_node->set_next(this->folder_list);
        this->folder_list = new_node;
     }
}

folder_db::~folder_db(){
    folder_records *temp   = this->folder_list;
    folder_records *temp_1 = NULL;

    while(temp){
        temp_1 = temp->get_next();
        delete temp;
        temp   = NULL;
        temp   = temp_1;
     }
}

folder_records *folder_db::get_folder(std::string _country){ //Get the records of a specific country.
    folder_records *temp = this->folder_list;

    while(temp){
        if(temp->get_country() == _country){
            return temp;
         }
     } 
    return NULL;
}
