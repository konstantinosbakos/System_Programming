#include "process.hpp"
#include "../functions/functions.hpp"

folder_records::folder_records(std::string folder_name){
    this->next            = NULL;
    this->folder_name     = folder_name;
    this->num_of_files    = 0;

    std::string *temp_array  = get_files(folder_name,this->num_of_files);

    this->file_array = new std::string*[this->num_of_files];

    for(int i=0; i<this->num_of_files; i++){ //Create the files array and give all the "Not Registered" flag.
        this->file_array[i]    = new std::string[2];
        this->file_array[i][0] = temp_array[i];
        this->file_array[i][1] = std::to_string(NOT_REG);
     }

    delete [] temp_array;
    temp_array = NULL;
}

folder_records::~folder_records(){
    for(int i=0; i<this->num_of_files; i++){
        delete [] this->file_array[i];
        this->file_array[i] = NULL;
     }
    delete [] this->file_array;
    this->file_array = NULL;
}

bool folder_records::update_file_array(){
    //Search the folder for new files. If new files are found, return true and add the with the "Not Registered" flag in the file array.
    int          current_num_of_files    = 0;
    std::string *current_folder_contents = get_files(this->folder_name,current_num_of_files);
    std::string *sorted_current_contents = sort_items(current_folder_contents,current_num_of_files);

    if(current_num_of_files > this->num_of_files){                             //If new files are found,
        std::string **new_file_array = new std::string*[current_num_of_files]; //create new expanded array,

        for(int i=0; i<this->num_of_files; i++){                               //save all the old files and flags,
            new_file_array[i]    = new std::string[2];
            new_file_array[i][0] = this->file_array[i][0];
            new_file_array[i][1] = this->file_array[i][1];
         }

        for(int i=0; i<this->num_of_files; i++){
            delete [] this->file_array[i];
            this->file_array[i] = NULL;
         }
        delete [] this->file_array;


        for(int i=this->num_of_files; i<current_num_of_files; i++){            //and add the new ones as well.
            new_file_array[i]    = new std::string[2];
            new_file_array[i][0] = sorted_current_contents[i];
            new_file_array[i][1] = std::to_string(NOT_REG);
         }

        this->num_of_files = current_num_of_files;
        this->file_array   = new_file_array;

        delete [] sorted_current_contents;
        sorted_current_contents = NULL;
        return true;
     }
    else{
        delete [] sorted_current_contents;
        sorted_current_contents = NULL;
        return false;
    }
}

void folder_records::set_one_registered(std::string file_name){                //Set a specific file as registered.
    for(int i=0; i<this->num_of_files; i++){
        if(this->file_array[i][0] == file_name){
            this->file_array[i][1] = std::to_string(REG);
            break;
         }
     }
}

std::string folder_records::get_country(){                                     //Get the country of the structure.
    return get_folder_country(this->folder_name);
}

std::string  *folder_records::get_not_registered(int &num_of_not_reg){         //Get all the not registered files.
    for(int i=0; i<this->num_of_files; i++){
        if(this->file_array[i][1] == std::to_string(NOT_REG)){
            num_of_not_reg++;
         }
     }
    
    if(!num_of_not_reg){
        return NULL;
     }

    std::string *not_reg_array = new std::string[num_of_not_reg];

    int j = 0;
    for(int i=0; i<this->num_of_files; i++){
        if(this->file_array[i][1] == std::to_string(NOT_REG)){
            not_reg_array[j] = this->file_array[i][0];
            j++;
         }
     }
    return not_reg_array;
}
