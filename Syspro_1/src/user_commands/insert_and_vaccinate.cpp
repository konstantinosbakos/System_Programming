#include "user_commands.hpp"

void insert_citizen_record_new_virus(int num_of_words, long bloom_size, std::string *Command_Array, skip_list ****vaccine_skip_l, bloom_filter ***vacced_bloom, skip_list *information_check, data_base *citizen_info, citizen *Sentinel_Dummy){
    //Function for adding a citizen to a new virus.
    int         num_of_vrs       = 0;                                                                   //Number of viruses.
    int         add_ctzn_to_info = 0;                                                                   //Flag to check if the citizen will be added to the information_check skip list.
    block      *citizen_exists   = information_check->search(std::stoi(Command_Array[1]));              //Check if citizen exists.
    std::string date             = "";
 
    if(!((0 <= stoi(Command_Array[1])) && (stoi(Command_Array[1]) <= 9999))){                           //Check to see if the dates are in a valid spectrum.
        std::cout << "Error: ID(" << Command_Array[1] << ") must be between 0 and 9999" << std::endl;
        return;
     }

    if(citizen_exists){
        if(!credential_check(citizen_exists->get_citizen(),Command_Array)){                             //Check if the input user credentials are the same.
            return;
         }
     }
    else{
        add_ctzn_to_info = 1;
     }

    if((Command_Array[7] == "YES") && (num_of_words == 8)){                                             //If citizen is vaccinated and there is no date of vaccination, ask the user if he wishes to enter today's date.
        std::string user_input = "";

        std::cout << "Error: If citizen is vaccinated please insert date of vaccination." << std::endl; //else todays date will be put y r n
        std::cout << "       Do you want to proceed with today's date? [y/n]"              << std::endl;
        std::cout << "(VaccineMonitor)$ ";

        getline(std::cin,user_input);

        if(user_input == "y" || user_input == ""){
            date = get_todays_date();
            num_of_words++;
         }
        else{
            return;
         }
     }

    if((Command_Array[7] == "YES") && (num_of_words == 9)){ //If citizen is vaccinated.
        citizen *new_citizen = NULL;

        if(date == ""){                                     //If the user has put in a custom date,
            new_citizen = citizen_info->add_citizen(num_of_words,Command_Array,information_check,add_ctzn_to_info);
         }
        else{                                               //and if the user chose today's date.
            new_citizen = citizen_info->add_citizen(num_of_words,Command_Array,information_check,add_ctzn_to_info,date);
         }
        if(new_citizen == NULL){
            return;
         }

        num_of_vrs = citizen_info->get_vrs_num();
        expand_bloom_skip_arrays(num_of_vrs,bloom_size,vaccine_skip_l,vacced_bloom,citizen_info->get_vrs_ptr(),Sentinel_Dummy); //Create the structures for the new virus.
        (*vaccine_skip_l)[num_of_vrs-1][VACCINATED]->insert(new_citizen);
        (*vacced_bloom)[num_of_vrs-1]->bloom_filter_insert(Command_Array[1]);
     }
    else if(Command_Array[7]  == "NO" && num_of_words == 8){                                                                                          //If user entered a not vaccinated citizen.
        citizen *new_citizen = citizen_info->add_citizen(num_of_words,Command_Array,information_check,add_ctzn_to_info);
        
        num_of_vrs = citizen_info->get_vrs_num();
        expand_bloom_skip_arrays(num_of_vrs,bloom_size,vaccine_skip_l,vacced_bloom,citizen_info->get_vrs_ptr(),Sentinel_Dummy);
        if(new_citizen){
            (*vaccine_skip_l)[num_of_vrs-1][NOT_VACCINATED]->insert(new_citizen);
         }
     }
}

void insert_citizen_record(int num_of_words, int num_of_vrs, std::string *Command_Array, skip_list ***vaccine_skip_l, bloom_filter **vacced_bloom, data_base *citizen_info, skip_list *information_check){
    //Insert a citizen record to an excisting virus.
    int           add_ctzn_to_info = 0;
    block        *new_ctzn_block = NULL;
    block        *ctzn_vac       = NULL;
    block        *ctzn_not_vac   = NULL;
    block        *citizen_exists = information_check->search(stoi(Command_Array[1]));
    skip_list    *vac_skip_l     = NULL;
    skip_list    *entry_skip_l   = NULL;
    skip_list    *not_vac_skip_l = NULL;
    std::string   date           = "";
    bloom_filter *entry_bloom    = NULL;

    if(!((0 <= stoi(Command_Array[1])) && (stoi(Command_Array[1]) <= 9999))){
        std::cout << "Error: ID(" << Command_Array[1] << ") must be between 0 and 9999" << std::endl;
        return;
     }

    if(citizen_exists){
        if(!credential_check(citizen_exists->get_citizen(),Command_Array)){
            return;
         }
     }
    else{
        add_ctzn_to_info = 1;
     }

    if((Command_Array[7] == "YES") && (num_of_words == 8)){                                             //If citizen is vaccinated and there is no date of vaccination, ask the user if he wishes to enter today's date.
        std::string user_input = "";

        std::cout << "Error: If citizen is vaccinated please insert date of vaccination." << std::endl; //else todays date will be put y r n
        std::cout << "       Do you want to proceed with today's date? [y/n]"              << std::endl;
        std::cout << "(VaccineMonitor)$ ";

        getline(std::cin,user_input);

        if(user_input == "y" || user_input == ""){
            date = get_todays_date();
            num_of_words++;
         }
        else{
            return;
         }
     }

    if(Command_Array[7]  == "YES" && num_of_words == 9){                                                   //If user entered a date of vaccination.
        new_ctzn_block = information_check->search(std::stoi(Command_Array[1]));
        if(new_ctzn_block){                                                                                //If citizen already exists.
            vac_skip_l = get_skip_list(num_of_vrs,Command_Array[6],vaccine_skip_l,VACCINATED);
            ctzn_vac = vac_skip_l->search(std::stoi(Command_Array[1]));                                    //Search if citizen exists.
       
            if(ctzn_vac){                                                                                  //If it does then return an error message.
                std::cout << "Error: Citizen " << Command_Array[1] << " has already been vaccinated on " << ctzn_vac->get_citizen()->get_date() << "." << std::endl;
                return;
            }
            else{                                                                                          //If not,
                not_vac_skip_l = get_skip_list(num_of_vrs,Command_Array[6],vaccine_skip_l,NOT_VACCINATED); //check if citizen is in the not-vaccinated list of the virus.
                ctzn_not_vac   = not_vac_skip_l->search(std::stoi(Command_Array[1]));
                entry_skip_l   = vac_skip_l;                                                               //Declare the entry skip list.
                entry_bloom    = get_bloom_filter(num_of_vrs,Command_Array[6],vacced_bloom);               //Declare the entry bloom filter.
            }
         }
        else{                                                                                              //If citizen does not exist, declare the entry skip list and bloom filter.
            entry_skip_l   = get_skip_list(num_of_vrs,Command_Array[6],vaccine_skip_l,VACCINATED);
            entry_bloom    = get_bloom_filter(num_of_vrs,Command_Array[6],vacced_bloom);
         }
     }
    else if(Command_Array[7]  == "NO" && num_of_words == 8){                                               //If user entered a not vaccinated citizen.
        vac_skip_l     = get_skip_list(num_of_vrs,Command_Array[6],vaccine_skip_l,VACCINATED); 
        not_vac_skip_l = get_skip_list(num_of_vrs,Command_Array[6],vaccine_skip_l,NOT_VACCINATED);
        
        ctzn_vac       = vac_skip_l->search(std::stoi(Command_Array[1]));
        ctzn_not_vac   = not_vac_skip_l->search(std::stoi(Command_Array[1]));

        if(ctzn_vac){
            std::cout << "Error: Citizen " << Command_Array[1] << " has already been vaccinated on " << ctzn_vac->get_citizen()->get_date() << "." << std::endl;
            return;     
         }

        if(ctzn_not_vac){
            std::cout << "Citizen " << Command_Array[1] << " already exists unvaccinated for the virus " << Command_Array[6] << "." << std::endl; 
            return;
         }
        entry_skip_l = not_vac_skip_l;                                                                     //Declare entry skip list.
     }

    citizen *new_citizen = NULL;
    
    if(date == ""){                                                                                        //Insert citizen to the data base.
        new_citizen = citizen_info->add_citizen(num_of_words,Command_Array,information_check,add_ctzn_to_info);
     }
    else{
        new_citizen = citizen_info->add_citizen(num_of_words-1,Command_Array,information_check,add_ctzn_to_info,date);
     }

    if(new_citizen){                                                                                       //Insert citizen to the structures.
        entry_skip_l->insert(new_citizen);                                                                 //Insert new citizen to skip list.
        if(entry_bloom){
            entry_bloom->bloom_filter_insert(Command_Array[1]);                                            //Insert new citizen to bloom filter.
        }
     }
    else{
        return;
     }

    if(ctzn_not_vac && vac_skip_l == entry_skip_l && add_ctzn_to_info == 0){                               //If the citizen is unvaccinated and existing.
        citizen *info_ctzn = information_check->search(std::stoi(Command_Array[1]))->get_citizen();
        citizen *del_ctzn  = not_vac_skip_l->delete_block(ctzn_not_vac->get_ID());                         //Remove from not vaccinated list.

        if(info_ctzn->get_is_vacc() == 0 && info_ctzn->get_virus() == Command_Array[6]){
            information_check->delete_block(std::stoi(Command_Array[1]));                                  //Remove citizen.
            citizen_info->remove_citizen(del_ctzn);                                                        //Delete citizen.
            information_check->insert(new_citizen);                                                        //Insert new citizen.
         }
        else{
            citizen_info->remove_citizen(del_ctzn);
         }
     }
    else if(add_ctzn_to_info){
        information_check->insert(new_citizen);
     }
}

void vaccinate_now_new_virus(int num_of_words, long bloom_size, std::string *Command_Array, skip_list ****vaccine_skip_l, bloom_filter ***vacced_bloom, skip_list *information_check, data_base *citizen_info, citizen *Sentinel_Dummy){
    //Insert a vaccinated citizen for a new virus.
    int         num_of_vrs       = 0;
    int         add_ctzn_to_info = 0;
    block      *citizen_exists   = information_check->search(std::stoi(Command_Array[1]));
    citizen    *new_citizen      = NULL;
    std::string date             = get_todays_date();
 
    if(!((0 <= stoi(Command_Array[1])) && (stoi(Command_Array[1]) <= 9999))){
        std::cout << "Error: ID(" << Command_Array[1] << ") must be between 0 and 9999" << std::endl;
        return;
     }

    if(citizen_exists){
        if(!credential_check(citizen_exists->get_citizen(),Command_Array)){
            return;
         }
     }
    else{
        add_ctzn_to_info = 1;
     }

    new_citizen = citizen_info->add_citizen(num_of_words,Command_Array,information_check,add_ctzn_to_info,date);

    num_of_vrs = citizen_info->get_vrs_num();                                                                       //Get new number of viruses.
    expand_bloom_skip_arrays(num_of_vrs,bloom_size,vaccine_skip_l,vacced_bloom,citizen_info->get_vrs_ptr(),Sentinel_Dummy); //Create structures for the new virus.
    (*vaccine_skip_l)[num_of_vrs-1][VACCINATED]->insert(new_citizen);                                               //Insert citizen.
}

void vaccinate_now(int num_of_vrs, int num_of_words, virus *vrs_list, std::string *Command_Array, skip_list ***vaccine_skip_l, bloom_filter **vacced_bloom, data_base *citizen_info,skip_list *information_check){
    //Insert a vaccinated citizen/vaccinate an existing citizen for an existing virus.
    if(!((0 <= stoi(Command_Array[1])) && (stoi(Command_Array[1]) <= 9999))){
        std::cout << "Error: ID(" << Command_Array[1] << ") must be between 0 and 9999" << std::endl;
        return;
     }
    
    int add_ctzn_to_info = 0;

    skip_list *vac_skip_l     = get_skip_list(num_of_vrs,Command_Array[6],vaccine_skip_l,VACCINATED);               //Get the virus skip lists.
    skip_list *not_vac_skip_l = get_skip_list(num_of_vrs,Command_Array[6],vaccine_skip_l,NOT_VACCINATED);
    
    block *ctzn_vac           = vac_skip_l->search(std::stoi(Command_Array[1]));                                    //Search to see if the citizen is present in any of them,
    block *ctzn_not_vac       = not_vac_skip_l->search(std::stoi(Command_Array[1]));
    block *citizen_exists     = information_check->search(std::stoi(Command_Array[1]));
    
    if(citizen_exists){
        if(!credential_check(citizen_exists->get_citizen(),Command_Array)){
            return;
         }
     }
    else{
        add_ctzn_to_info = 1;
     }

    if(ctzn_vac){                                                                                                   //If citizen is vaccinated return an error message.
        std::cout << "Error: Citizen " << ctzn_vac->get_citizen()->get_ID() << " already vaccinated on " << ctzn_vac->get_citizen()->get_date() << "." << std::endl;
        return;
     }
    else{

        std::string   date        = get_todays_date();                                                              //Get today's date for vaccination.
        bloom_filter *vac_bloom   = get_bloom_filter(num_of_vrs,Command_Array[6],vacced_bloom);                     //Get the virus bloom filter.
        citizen      *new_citizen = citizen_info->add_citizen(num_of_words,Command_Array,information_check,1,date); //Create new citizen.

        if(new_citizen){                                              //If the new citizen is created succesfully, insert to structures.
            vac_skip_l->insert(new_citizen);
            vac_bloom->bloom_filter_insert(Command_Array[1]);

            if(ctzn_not_vac){                                         //If citizen was in the not-vaccinated list,
                not_vac_skip_l->delete_block(ctzn_not_vac->get_ID()); //remove from not vaccinated list.
             }
    
            if(add_ctzn_to_info){
                information_check->insert(new_citizen);
             }
            else{ 
                citizen *info_ctzn = citizen_exists->get_citizen();

                if(info_ctzn->get_is_vacc() == 0 && info_ctzn->get_virus() == Command_Array[6]){
                    citizen *del_ctzn = information_check->delete_block(std::stoi(Command_Array[1]));

                    citizen_info->remove_citizen(del_ctzn);
                    information_check->insert(new_citizen);
                 }                 
             }
         }
     }
}
