#include <cmath>
#include <fstream>
#include <dirent.h>
#include <unistd.h>


#include "../user_commands/user_commands.hpp"

void welcome(){ //welcome message for the user after initialization.
    std::cout << "\n~> Welcome to the Travel Monitor."                                                   << std::endl;
    std::cout << "~> The command formats will be printed. To access them again please type /help."     << std::endl;
    std::cout << "~> To see the initialization errors please open the file initialization_errors.txt." << std::endl;
    help();     //Print instructions for the first time.
}

void print_bar(int length){ //Print 'length' bar characters.
    for(int j=0; j<=length; j++){
        if(j != length){
            std::cout << " ";
         }
        else{
            std::cout << "|";
         }
     }
    std::cout << std::endl;
}

void print_space(int length){ //Print 'length' space characters.
    for(int i=0; i<length; i++){
        std::cout << " ";
     }
}

void print_tilde(int length){ //Print 'length' tidle characters.
    std::cout << " ";
  
    for(int i=0; i<length; i++){
        std::cout << "~";
     }

    std::cout << std::endl;
}

void print_underscore(int length){ //Print 'length' underscore characters.
    std::cout << " ";
 
    for(int i=0; i<length; i++){
        std::cout << "_";
     }

    std::cout << std::endl;
}

void remove_directory(std::string dir_name){   //Remove a directory.
    DIR *directory = opendir(dir_name.c_str());

    if (directory){                            //If directory exists.
        struct dirent *element;

        while((element = readdir(directory))){ //Delete all files except "." and "..".
            if ((std::string(element->d_name) != ".") && (std::string(element->d_name) != "..")){
                unlink(std::string(dir_name + "/" + element->d_name).c_str());
             }  
         }
        closedir(directory);
     }
    rmdir(dir_name.c_str());
}

void init_socket_arrays(int **sockets, int **socket_id){ //Initialize all socket values to -1.
    (*sockets)   = new int[2];
    (*socket_id) = new int[2];

    (*sockets)  [SERVER_R] = -1;
    (*sockets)  [SERVER_W] = -1;
    (*socket_id)[SERVER_R] = -1;
    (*socket_id)[SERVER_W] = -1;
}

void compare_folder_names(std::string &name_1, std::string &name_2){
    //Compare to folder names and place to name_1 the alphabetically first.
    int         iter_length = 0;
    std::string country_1   = get_folder_country(name_1);
    std::string country_2   = get_folder_country(name_2);

    if(country_1.length() > country_2.length()){
        iter_length = country_2.length();
     }
    else{
        iter_length = country_1.length();
     }

    for(int i=0; i<iter_length; i++){
        if(country_1[i] == country_2[i]){
            continue;
         }
        else if(country_1[i] < country_2[i]){
            return;
         }
        else if(country_1[i] > country_2[i]){
            std::string temp = name_1;
            name_1           = name_2;
            name_2           = temp;
            return;
         }
     }
}

void get_command(int &command,std::string **input_array, int &num_of_words){
    //Get the user input, break it to words and return 
    std::string input_buffer = "";

    std::cout << "(travelMonitor)$ ";
    
    getline(std::cin,input_buffer);

    *input_array = split_to_words(std::string(input_buffer),num_of_words);

    if(*input_array[0] == "/travelRequest"){
        if(num_of_words == 6){
            command = TRVL_RQ;
            return;
         }
        else{
            command = WRONG_INPUT;
            return;
         }
     }
    else if((*input_array)[0] == "/travelStats"){
        if(num_of_words == 4 || num_of_words == 5){
            command = TRVL_STS;
            return;
         }
        else{
            command = WRONG_INPUT;
            return;
         }
     }
    else if((*input_array)[0] == "/addVaccinationRecords"){
        if(num_of_words == 2){
            command = ADD_VAC_REC;
            return;
         }
        else{
            command = WRONG_INPUT;
            return;
         }
     } 
    else if((*input_array)[0] == "/searchVaccinationStatus"){
        if(num_of_words == 2){
            command = SRCH_VAC_STS;
            return;
         }
        else{
            command = WRONG_INPUT;
            return;
         }
     }
    else if((*input_array)[0] == "/printPids"){
        if(num_of_words == 1){
            command = PRINT_PIDS;
            return;
         }
        else{
            command = WRONG_INPUT;
            return;
         }
     }
    else if((*input_array)[0] == "/help"){
        if(num_of_words == 1){
            command = HELP;
            return;
         }
        else{
            command = WRONG_INPUT;
            return;
         }
     }
    else if((*input_array)[0] == "/exit"){
        if(num_of_words == 1){
            command = TERMINATE;
            return;
         }
        else{
            command = WRONG_INPUT;
            return;
         }
     }
    else{
        command = WRONG_INPUT;
        return;
     }
    return;
}

void vaccine_status(int ID, int num_of_vrs, skip_list ***vaccine_skip_l, std::string &citizen_viruses){
    //Used to find the vaccine status of a citizen (used by Monitor).
    block   *b_ct_temp = NULL;
    citizen *ct_temp   = NULL;

    for(int i=0; i<num_of_vrs; i++){
        b_ct_temp = vaccine_skip_l[i][VACCINATED]->search(ID); //If the ID exists in the skip list.

        if(b_ct_temp){
            ct_temp   = b_ct_temp->get_citizen();
            b_ct_temp = NULL;
         }

        if(ct_temp){
            citizen_viruses += (ct_temp->get_virus() + " " + "YES" + " " + ct_temp->get_date() + " ");
            ct_temp = NULL;
         }

        b_ct_temp = vaccine_skip_l[i][NOT_VACCINATED]->search(ID);

        if(b_ct_temp){                                         //If ID exists.
            ct_temp   = b_ct_temp->get_citizen();
            b_ct_temp = NULL;
         }

        if(ct_temp){
            citizen_viruses += (ct_temp->get_virus() + " " + "NO" + " ");
            ct_temp = NULL;
         }
     }
}

void expand_bloom_skip_arrays(int num_of_vrs, long bloom_size, skip_list ****vaccine_skip_l, bloom_filter ***vacced_bloom, virus *head_vrs, citizen *Sentinel_Dummy){
    virus         *vrs_temp           = head_vrs;
    skip_list   ***new_vaccine_skip_l = new skip_list**[num_of_vrs];
    bloom_filter **new_vacced_bloom   = new bloom_filter*[num_of_vrs];

    for(int i=0;i<num_of_vrs;i++){          //Create new array.
        new_vaccine_skip_l[i] = new skip_list*[2];
     }

    for(int i=0;i<num_of_vrs-1;i++){        //Copy the existing.
        new_vaccine_skip_l[i][NOT_VACCINATED] = (*vaccine_skip_l)[i][NOT_VACCINATED];
        new_vaccine_skip_l[i][VACCINATED]     = (*vaccine_skip_l)[i][VACCINATED];
        new_vacced_bloom[i]                   = (*vacced_bloom)[i];
     }

    while(vrs_temp->get_next()){
        vrs_temp = vrs_temp->get_next();
     }

    new_vaccine_skip_l[num_of_vrs-1][NOT_VACCINATED] = new skip_list(NOT_VACCINATED,vrs_temp,Sentinel_Dummy); //Create the new structures for the new virus.
    new_vaccine_skip_l[num_of_vrs-1][VACCINATED]     = new skip_list(VACCINATED,vrs_temp,Sentinel_Dummy);
    new_vacced_bloom[num_of_vrs-1]                   = new bloom_filter(bloom_size,vrs_temp);

	for(int i=0; i<num_of_vrs-1; i++){      //Delete the old arrays.
        (*vaccine_skip_l)[i][VACCINATED]     = NULL;
        (*vaccine_skip_l)[i][NOT_VACCINATED] = NULL;
		
        delete [] (*vaccine_skip_l)[i];
        (*vaccine_skip_l)[i] = NULL;
	 }

    for(int i=0; i<num_of_vrs-1; i++){
        (*vacced_bloom)[i] = NULL;
     }

	delete [] (*vaccine_skip_l);
    (*vaccine_skip_l) = new_vaccine_skip_l; //Set new array.


    delete [] (*vacced_bloom);
    (*vacced_bloom) = new_vacced_bloom;     //Set new array.
}

void insert_citizen_record(int num_of_words, int num_of_vrs, std::string *input_array, skip_list ***vaccine_skip_l, bloom_filter **vacced_bloom, data_base *citizen_info, skip_list *information_check){
    //Insert a citizen record to an excisting virus.
    int           add_ctzn_to_info = 0;
    block        *new_ctzn_block = NULL;
    block        *ctzn_vac       = NULL;
    block        *ctzn_not_vac   = NULL;
    block        *citizen_exists = information_check->search(to_int(input_array[0]));
    skip_list    *vac_skip_l     = NULL;
    skip_list    *entry_skip_l   = NULL;
    skip_list    *not_vac_skip_l = NULL;
    bloom_filter *entry_bloom    = NULL;

    if(!((0 <= to_int(input_array[0])) && (to_int(input_array[0]) <= 9999))){
        std::cout << "Error: ID(" << input_array[0] << ") must be between 0 and 9999" << std::endl;
        return;
     }

    if(citizen_exists){
        if(!credential_check(citizen_exists->get_citizen(),input_array,1)){
            return;
         }
     }
    else{
        add_ctzn_to_info = 1;
     }

    if(input_array[6]  == "YES" && num_of_words == 8){                                                   //If user entered a date of vaccination.
        new_ctzn_block = information_check->search(to_int(input_array[0]));
        if(new_ctzn_block){                                                                                //If citizen already exists.
            vac_skip_l = get_skip_list(num_of_vrs,input_array[5],vaccine_skip_l,VACCINATED);
            ctzn_vac = vac_skip_l->search(to_int(input_array[0]));                                    //Search if citizen exists.
       
            if(ctzn_vac){                                                                                  //If it does then return an error message.
                std::cout << "\nError: Incosistent record for citizen " << input_array[0] << ". Already exists. (" << input_array[5] << ")" << std::endl; 
                return;
            }
            else{                                                                                          //If not,
                not_vac_skip_l = get_skip_list(num_of_vrs,input_array[5],vaccine_skip_l,NOT_VACCINATED); //check if citizen is in the not-vaccinated list of the virus.
                ctzn_not_vac   = not_vac_skip_l->search(to_int(input_array[0]));
                entry_skip_l   = vac_skip_l;                                                               //Declare the entry skip list.
                entry_bloom    = get_bloom_filter(num_of_vrs,input_array[5],vacced_bloom);               //Declare the entry bloom filter.
               
                if(ctzn_not_vac){
                    std::cout << "\nError: Incosistent record for citizen " << input_array[0] << ". Already exists. (" << input_array[5] << ")" << std::endl; 
                    return;
                 }
             }
         }
        else{                                                                                              //If citizen does not exist, declare the entry skip list and bloom filter.
            entry_skip_l   = get_skip_list(num_of_vrs,input_array[5],vaccine_skip_l,VACCINATED);
            entry_bloom    = get_bloom_filter(num_of_vrs,input_array[5],vacced_bloom);
         }
     }
    else if(input_array[6]  == "NO" && num_of_words == 7){                                               //If user entered a not vaccinated citizen.
        vac_skip_l     = get_skip_list(num_of_vrs,input_array[5],vaccine_skip_l,VACCINATED); 
        not_vac_skip_l = get_skip_list(num_of_vrs,input_array[5],vaccine_skip_l,NOT_VACCINATED);
        
        ctzn_vac       = vac_skip_l->search(to_int(input_array[0]));
        ctzn_not_vac   = not_vac_skip_l->search(to_int(input_array[0]));

        if(ctzn_vac){
            std::cout << "\nError: Incosistent record for citizen " << input_array[0] << ". Already exists. (" << input_array[5] << ")" << std::endl; 
            return;     
         }

        if(ctzn_not_vac){
            std::cout << "\nError: Incosistent record for citizen " << input_array[0] << ". Already exists. (" << input_array[5] << ")" << std::endl; 
            return;
         }
        entry_skip_l = not_vac_skip_l;                                                                     //Declare entry skip list.
     }

    citizen *new_citizen = NULL;
    
    new_citizen = citizen_info->add_citizen(num_of_words,input_array,information_check,add_ctzn_to_info);

    if(new_citizen){                                                                                       //Insert citizen to the structures.
        entry_skip_l->insert(new_citizen);                                                                 //Insert new citizen to skip list.
        if(entry_bloom){
            entry_bloom->bloom_filter_insert(input_array[0]);                                            //Insert new citizen to bloom filter.
        }
     }
    else{
        return;
     }

    if(ctzn_not_vac && vac_skip_l == entry_skip_l && add_ctzn_to_info == 0){                               //If the citizen is unvaccinated and existing.
        citizen *info_ctzn = information_check->search(to_int(input_array[0]))->get_citizen();
        citizen *del_ctzn  = not_vac_skip_l->delete_block(ctzn_not_vac->get_ID());                         //Remove from not vaccinated list.

        if(info_ctzn->get_is_vacc() == 0 && info_ctzn->get_virus() == input_array[5]){
            information_check->delete_block(to_int(input_array[0]));                                  //Remove citizen.
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

void add_to_data_base(long bloom_size, std::string file_name, bloom_filter ***vacced_bloom, skip_list ****vaccine_skip_l, skip_list *information_check, data_base *citizen_info, citizen *Sentinel_Dummy){
    std::string     str;

    std::ifstream file(file_name);                                                                       //Open file.

    while(getline(file,str)){
        citizen_info->add_citizen(str,information_check);
        }

    (*vacced_bloom)   = create_bloom_filters(bloom_size,citizen_info);
    (*vaccine_skip_l) = create_skip_lists(citizen_info,Sentinel_Dummy);
 
    init_monitor_structures((*vacced_bloom),(*vaccine_skip_l),citizen_info,information_check);                   //Create the array of all skip lists and the array of all bloom filters.
}

void insert_citizen_record_new_virus(int num_of_words, long bloom_size, std::string *input_array, skip_list ****vaccine_skip_l, bloom_filter ***vacced_bloom, skip_list *information_check, data_base *citizen_info, citizen *Sentinel_Dummy){
    //Function for adding a citizen to a new virus.
    int         num_of_vrs       = 0;                                                                                           //Number of viruses.
    int         add_ctzn_to_info = 0;                                                                                           //Flag to check if the citizen will be added to the information_check skip list.
    block      *citizen_exists   = information_check->search(to_int(input_array[0]));                                        //Check if citizen exists.
 
    if(!((0 <= to_int(input_array[0])) && (to_int(input_array[0]) <= 9999))){                                                       //Check to see if the dates are in a valid spectrum.
        std::cout << "Error: ID(" << input_array[0] << ") must be between 0 and 9999" << std::endl;
        return;
     }

    if(citizen_exists){
        if(!credential_check(citizen_exists->get_citizen(),input_array,1)){                                                     //Check if the input user credentials are the same.
            return;
         }
     }
    else{
        add_ctzn_to_info = 1;
     }

    if((input_array[6] == "YES") && (num_of_words == 8)){                                                                       //If citizen is vaccinated.
        citizen *new_citizen = NULL;

        new_citizen = citizen_info->add_citizen(num_of_words,input_array,information_check,add_ctzn_to_info);

        if(new_citizen == NULL){
            return;
         }

        num_of_vrs = citizen_info->get_vrs_num();
        expand_bloom_skip_arrays(num_of_vrs,bloom_size,vaccine_skip_l,vacced_bloom,citizen_info->get_vrs_ptr(),Sentinel_Dummy); //Create the structures for the new virus.
        (*vaccine_skip_l)[num_of_vrs-1][VACCINATED]->insert(new_citizen);
        (*vacced_bloom)[num_of_vrs-1]->bloom_filter_insert(input_array[1]);
     }
    else if(input_array[6]  == "NO" && num_of_words == 7){                                                                      //If user entered a not vaccinated citizen.
        citizen *new_citizen = citizen_info->add_citizen(num_of_words,input_array,information_check,add_ctzn_to_info);
        
        num_of_vrs = citizen_info->get_vrs_num();
        expand_bloom_skip_arrays(num_of_vrs,bloom_size,vaccine_skip_l,vacced_bloom,citizen_info->get_vrs_ptr(),Sentinel_Dummy);
        if(new_citizen){
            (*vaccine_skip_l)[num_of_vrs-1][NOT_VACCINATED]->insert(new_citizen);
         }
     }
}
