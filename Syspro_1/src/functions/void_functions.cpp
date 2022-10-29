#include <cmath>

#include "../user_commands/user_commands.hpp"

void Welcome(){                                                                //Welcome message for the user after initialization.
    std::cout << "~> Welcome to the Vaccine Monitor." << std::endl;
    std::cout << "~> The command formats will be printed. To access them again please type /help." << std::endl;
    std::cout << "~> To see the initialization errors please open the file Initialization_Errors.txt." << std::endl;
    help();                                                                    //Print instructions for the first time.
}

void print_country(std::string country){                                       //Used to beatify output,
    int a = 19 - country.length();                                             //of population status by age.
    int b = floor(a/2);

    for(int i=0; i<b;i++){std::cout << "-";}
    std::cout << country;
    for(int i=0; i<b;i++){std::cout << "-" ;}
    if(b*2 < a){std::cout << "-" ;}
    std::cout << std::endl;
}

void get_command(int &command,std::string **Command_Array, int &num_of_words){ //Get the user input, break it to words and return 
    std::string user_input;                                                    //the correct output for the switch-case in main.cpp.
    
    std::cout << "(VaccineMonitor)$ ";
    getline(std::cin,user_input);
    *Command_Array = split_to_words(user_input,num_of_words);

    if(*Command_Array[0] == "/vaccineStatusBloom"){
        if(num_of_words == 3){
            command = VAC_STAT_BM;
            return;
         }
        else{
            command = WRONG_INPUT;
            return;
         }
     }
    else if((*Command_Array)[0] == "/vaccineStatus"){
        if(num_of_words == 3){
            command = VAC_STAT_VR;
            return;
         }
        else if(num_of_words == 2){
            command = VAC_STAT;
            return;
         }
        else{
            command = WRONG_INPUT;
            return;
         }
     }
    else if((*Command_Array)[0] == "/populationStatus"){
        if(num_of_words == 4){
            command = POP_STAT;
            return;
         }
        else if(num_of_words == 5){
            command = POP_STAT;
            return;
         }
        else{
            command = WRONG_INPUT;
            return;
         }
     }
    else if((*Command_Array)[0] == "/popStatusByAge"){
        if(num_of_words == 4  || num_of_words == 5){
            command = POP_STAT_AGE;
            return;
         }
        else{
            command = WRONG_INPUT;
            return;
         }
     }
    else if((*Command_Array)[0] == "/insertCitizenRecord"){
        if(num_of_words == 8 || ((num_of_words == 9) && (*Command_Array)[7] == "YES")){
            command = INSERT_CTZN;
            return;
         }
        else{
            command = WRONG_INPUT;
            return;
         }
     }
    else if((*Command_Array)[0] == "/vaccinateNow"){
        if(num_of_words == 7){
            command = VAC_NOW;
            return;
         }
        else{
            command = WRONG_INPUT;
            return;
         }
     }
    else if((*Command_Array)[0] == "/list-nonVaccinated-Persons"){
        if(num_of_words == 2){
            command = LS_NON_VAC;
            return;
         }
        else{
            command = WRONG_INPUT;
            return;
         }
     }
    else if((*Command_Array)[0] == "/system-viruses"){
        if(num_of_words == 1){
            command = STM_VRS;
            return;
         }
        else{
            command = WRONG_INPUT;
            return;
         }
     }
    else if((*Command_Array)[0] == "/help"){
        if(num_of_words == 1){
            command = HELP;
            return;
         }
        else{
            command = WRONG_INPUT;
            return;
         }
     }
    else if((*Command_Array)[0] == "/exit"){
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

void init_structures(bloom_filter **vacced_bloom, skip_list ***vaccine_skip_l, data_base *db,skip_list *information_check){ //Initialize structures.
    int      i       = 0;
    virus   *vr_temp = db->get_vrs_ptr();
    citizen *ct_next = db->get_citizen_ptr();

    while(vr_temp){
        citizen *ct_curr = db->get_citizen_ptr();
        while(ct_curr){ 
            block *exists_in_vac     = (vaccine_skip_l[i][VACCINATED]->search(ct_curr->get_ID()));
            block *exists_in_not_vac = (vaccine_skip_l[i][NOT_VACCINATED]->search(ct_curr->get_ID()));

            ct_next = ct_curr->get_next();
            
            if(ct_curr->get_is_vacc() == VACCINATED){                                                         //If citizen is vaccinated,
                if(ct_curr->get_virus() == vr_temp->get_virus() && !exists_in_not_vac && !exists_in_vac){     //and virus matches,
                    vaccine_skip_l[i][VACCINATED]->insert(ct_curr);                                           //insert element,
                    vacced_bloom[i]->bloom_filter_insert(std::to_string(ct_curr->get_ID()));                    
                    if(!information_check->search(ct_curr->get_ID())){                                        //If citizen is not in the skip list, insert.
                        information_check->insert(ct_curr);
                     }
                 }
                else if(ct_curr->get_virus() == vr_temp->get_virus() && (exists_in_not_vac || exists_in_vac)){ //If citizen exists in either of the lists for this virus, do not insert.
                    std::cout << "Error: Incosistent record for citizen " << ct_curr->get_ID() << ". Already exists. (" << ct_curr->get_virus() << ")" << std::endl;
                    
                    information_check->delete_block(ct_curr->get_ID());
                    db->remove_citizen(ct_curr);
                    if(exists_in_not_vac){
                        information_check->insert(exists_in_not_vac->get_citizen());
                     }
                    else{
                        information_check->insert(exists_in_vac->get_citizen());
                    }
                    ct_curr = NULL;
                 }
             }
            else{                                                                                                                  //If citizen is not vaccinated,
                if(ct_curr->get_virus() == vr_temp->get_virus() && !exists_in_vac && !exists_in_not_vac){                          //and virus matches,
                    vaccine_skip_l[i][NOT_VACCINATED]->insert(ct_curr);                                                            //insert citizen to list.
                    if(!information_check->search(ct_curr->get_ID())){
                        information_check->insert(ct_curr);
                     }
                 }
                else if(ct_curr->get_virus() == vr_temp->get_virus() && (exists_in_not_vac || exists_in_vac)){ //If citizen exists and is in either of the lists for this virus, do not insert.
                    std::cout << "E rror: Incosistent record for citizen " << ct_curr->get_ID() << ". Already exists. (" << ct_curr->get_virus() << ")" << std::endl;
                    
                    information_check->delete_block(ct_curr->get_ID());
                    db->remove_citizen(ct_curr);
                    if(exists_in_not_vac){
                        information_check->insert(exists_in_not_vac->get_citizen());
                     }
                    else{
                        information_check->insert(exists_in_vac->get_citizen());
                    }
                    ct_curr = NULL;
                 }

             }
            ct_curr = ct_next; 
         }
        vr_temp = vr_temp->get_next();
        i++;
     }
}

void delete_structures(data_base *citizen_info, skip_list  ***vaccine_skip_l, bloom_filter **vacced_bloom, skip_list *information_check, citizen *Sentinel_Dummy){ //Delete all the structures used by the program.
	for(int i=0; i<citizen_info->get_vrs_num(); i++){
		delete vaccine_skip_l[i][VACCINATED];
		delete vaccine_skip_l[i][NOT_VACCINATED];
        
        vaccine_skip_l[i][VACCINATED]     = NULL;
        vaccine_skip_l[i][NOT_VACCINATED] = NULL;
		
        delete [] vaccine_skip_l[i];
        vaccine_skip_l[i] = NULL;
	 }

    for(int i=0; i<citizen_info->get_vrs_num(); i++){
        delete vacced_bloom[i];
        vacced_bloom[i] = NULL;
     }
     
    delete information_check;
    information_check = NULL;

	delete [] vaccine_skip_l;
    vaccine_skip_l = NULL;

    delete citizen_info;
    citizen_info = NULL;

    delete Sentinel_Dummy;
    Sentinel_Dummy = NULL;

    delete [] vacced_bloom;
    vacced_bloom = NULL;

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
