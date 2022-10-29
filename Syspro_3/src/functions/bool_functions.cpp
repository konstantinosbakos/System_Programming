#include <cmath>
#include <dirent.h>

#include "functions.hpp"

bool coin_toss(){                //Return 0 or 1. The initialization of rand is in main.cpp.
    return rand()%2;
}

bool is_number(char chr){        //Check if string is a number.
    if(chr > '9' || chr < '0'){
        return false;            //If it is not, print message and return 0.
     }
    return true;
}

bool is_number(std::string str){ //Check if string is a number.
    for(int i=0; i<int(str.length()); i++){
        if(str[i] > '9' || str[i] < '0'){
            std::cout << "Error: Input variable should be number but it is not. For more information type \'/help\'" << std::endl;
            return false;        //If it is not, print message and return 0.
         }
     }
    return true;
}

bool check_date_format(std::string date){
    int flag = 0;
    
    if((date[0] < '0' || date[0] > '9') || (date.back() < '0' || date.back() > '9')){
        return false;
     }

    for(int i=0; i<int(date.length()); i++){
        if((date[i] < '0' || date[i] > '9') && date[i] != '-'){
            return false;
         }
        
        if(i != 0){
            if(date[i] == '-' && date[i-1] == '-'){
                return false;
             }
         }
        
        if(date[i] == '-'){
            flag++;
         }
        
        if(flag > 2){
            return false; 
         }
     }
     
    if(flag < 2){
        return false;
     }

    return true;
}

bool is_virus(std::string vrs, virus *virus_list, bool print){                      //Check if virus is in list.
    virus *temp = virus_list;

    while(temp){
        if(temp->get_virus() == vrs){
            return true;
         }
        temp = temp->get_next();
     }
    if(print){
        std::cout << "Error: Input variable should be virus but it is not. For more information type \'/help\'" << std::endl;
     }
    return false;                                                                   //If it is not, print message and return 0.
}

bool credential_check(citizen *existing_ct, std::string *Array, int i){             //Check if a citizen's info is the same as those in the data base.
    if(!(existing_ct->get_first_name() == Array[2-i] && existing_ct->get_surname() == Array[3-i] && existing_ct->get_country() == Array[4-i] && existing_ct->get_age() == to_int(Array[5-i]))){
        std::cout << "citizen information mismatch with already existsing information for this citizen." << std::endl;
        std::cout << "    ID         : " <<existing_ct->get_ID()         << std::endl;
        std::cout << "    First name : " <<existing_ct->get_first_name() << std::endl;
        std::cout << "    Surname    : " <<existing_ct->get_surname()    << std::endl;
        std::cout << "    Country    : " <<existing_ct->get_country()    << std::endl;
        std::cout << "    Age        : " <<existing_ct->get_age()        << std::endl;

        return false;
     }
    return true;
}

bool compare_dates(std::string ctzn_date, std::string date_1, std::string date_2){  //Check if ctzn_date is between date_1 and date_2.
    int *num_ctzn = date_to_int(ctzn_date);
    int *num_1    = date_to_int(date_1);
    int *num_2    = date_to_int(date_2);

    if((0 >= num_ctzn[0]) || (30 < num_ctzn[0]) || (0 >= num_ctzn[1]) || (12 < num_ctzn[1]) || (0 >= num_ctzn[2])){
        DELETE_ARRAYS;
        return false;                                                              //Check if the date is realistic.
     }

    if(num_1[2] <  num_ctzn[2] && num_ctzn[2] < num_2[2]){DELETE_ARRAYS;return true;}
    if(num_1[2] == num_ctzn[2] && num_1[2] != num_2[2]){
        if(num_1[1] < num_ctzn[1]){DELETE_ARRAYS;return true;}
        if(num_1[1] == num_ctzn[1] && num_1[0] <= num_ctzn[0]){DELETE_ARRAYS;return true;}
     }
    if(num_ctzn[2] == num_2[2] && num_1[2] != num_2[2]){
        if(num_ctzn[1] < num_2[1]){DELETE_ARRAYS;return true;}
        if(num_ctzn[1] == num_2[1] && num_ctzn[0] <= num_2[0]){DELETE_ARRAYS;return true;}
     }
    if(num_1[2] == num_ctzn[2] && num_ctzn[2] == num_2[2]){
        if(num_1[1] < num_ctzn[1] && num_ctzn[1] < num_2[1]){DELETE_ARRAYS;return true;}
        if(num_1[1] == num_ctzn[1] && num_ctzn[1] != num_2[1] && num_1[1] <= num_ctzn[1]){DELETE_ARRAYS;return true;}
        if(num_1[1] == num_ctzn[1] && num_ctzn[1] == num_2[1] && num_1[0] <= num_ctzn[0] && num_ctzn[0] <= num_2[0]){DELETE_ARRAYS;return true;}
     }

    DELETE_ARRAYS;
    return false;
}

bool process_subfolders(int &num_of_monitors, int &num_of_subfolders, std::string folder_name, std::string **subfolders, std::string **monitor_inputs){
    DIR *mon_dir = opendir((const char*)(folder_name.c_str()));                                    //Check if folder exists.

    if(mon_dir){                                                                                   //If it does,
        closedir(mon_dir);                                                                         //close it.

        (*subfolders)     = get_subfolders(folder_name,num_of_subfolders);                         //Get the subfolders,
        (*subfolders)     = sort_items((*subfolders),num_of_subfolders);                           //sort the by name,
        (*monitor_inputs) = get_monitor_input_RR((*subfolders),num_of_monitors,num_of_subfolders); //and create the inputs that the monitors will receive with Round Robin.

        return true;
     }
    else{                                                                                          //If it does not exist, return error.
        std::cout << "~> Error: Folder does not exist. Please enter an existing folder." << std::endl;

        return false;
     }
}
