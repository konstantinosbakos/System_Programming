#include <sys/stat.h>

#include "functions.hpp"

bool init_program(int argc, char** argv, int &num_of_monitors, int &buffer_size, long &bloom_size, std::string &file_name){ //User command line input.
    int flag = 0;

    if(argc == 1){
        return false;
    }

    if(std::string(argv[1]) == "-h"){ //Print instructions if user requires them.
        std::cout << " ____________________________________________________________________ " << std::endl;
        std::cout << "| [-m] is followed by the number of Monitors that will be created.   |" << std::endl;
        std::cout << "| [-b] is followed by the buffer size.                               |" << std::endl;
        std::cout << "| [-s] is followed by the size of the Bloom filter (in bytes).       |" << std::endl;
        std::cout << "| [-i] is followed by the directories used by the monitor processes. |" << std::endl;
        std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << std::endl;
        return false;
     }

    if(argc != 9){                    //If not enough parameters return error.
        std::cout << "Wrong input order or missing parameter. Please type \"-h\" for input format" << std::endl;
        return false;
     }

    for(int i=0; i<argc; i++){
        if(std::string(argv[i]) == "-m" || std::string(argv[i]) == "-b" || std::string(argv[i]) == "-s" || std::string(argv[i]) == "-i"){
            flag++;
         }
     }

    if(flag < 4 && argc == 9){
        std::cout << "Wrong input order or missing parameter. Please type \"-h\" for input format" << std::endl;
        return false;
      }

    for(int i=1; i<argc; i++){        //Store user input.
        if((std::string(argv[i]) == "-m")){
            num_of_monitors = std::stoi(argv[i+1]);
         }
        else if((std::string(argv[i]) == "-b")){
            buffer_size  = std::stoi(argv[i+1]);
         }
        else if((std::string(argv[i]) == "-s")){
            bloom_size   = std::stol(argv[i+1]);
         }
        else if((std::string(argv[i]) == "-i")){
            file_name    = argv[i+1];
         } 
     }
    return true;
}

void init_directories(){       //Initialize the folders that the program will be using.
    remove_directory("pipes"); //If the folders exist, remove them.
    remove_directory("log_files");
    remove_directory("initialization_errors");

    mkdir("pipes",0777);       //Create the folders.
    mkdir("log_files",0777);
    mkdir("initialization_errors",0777);
}

void delete_TRQ_dates(std::string *response_array, int *input_date, int *vaccine_date){ //Function used in travel_requests.cpp
    if(input_date && vaccine_date){
        delete [] input_date;
        delete [] vaccine_date;
     }

    delete [] response_array;
    input_date     = NULL;
    vaccine_date   = NULL;
    response_array = NULL;
}

void init_monitor_structures(bloom_filter **vacced_bloom, skip_list ***vaccine_skip_l, data_base *db,skip_list *information_check){ //Initialize structures.
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
                else if(ct_curr->get_virus() == vr_temp->get_virus() && (exists_in_not_vac || exists_in_vac)){                     //If citizen exists and is in either of the lists for this virus, do not insert.
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
            ct_curr = ct_next; 
         }
        vr_temp = vr_temp->get_next();
        i++;
     }
}

void delete_monitor_citizen_structures(data_base *citizen_info, skip_list  ***vaccine_skip_l, bloom_filter **vacced_bloom, skip_list *information_check, citizen *Sentinel_Dummy){
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

    delete [] vacced_bloom;
    vacced_bloom = NULL;

    delete Sentinel_Dummy;
    Sentinel_Dummy = NULL;

    delete citizen_info;
    citizen_info = NULL;
}

void delete_monitor_structures(int num_of_folders, int *pipe, folder_db *monitor_folders, std::string *folder_array, country_records **monitor_countries){   //Delete structures.

    for(int i=0; i<num_of_folders; i++){
        delete monitor_countries[i];
        monitor_countries[i] = NULL;
     }

    delete [] monitor_countries;
    monitor_countries = NULL;

    delete monitor_folders;
    monitor_folders = NULL;

    delete [] folder_array;
    folder_array = NULL;

    delete [] pipe;
    pipe = NULL;
}

void delete_travel_monitor_structures(int num_of_monitors, char ***pipe_array, std::string *subfolders, std::string *monitor_inputs, monitor_db **monitor_array){
    if(monitor_array){
        for(int i=0; i<num_of_monitors; i++){
            delete monitor_array[i];
            monitor_array[i] = NULL;
         }

        delete [] monitor_array;
        monitor_array = NULL;
     }

    for(int i=0; i<num_of_monitors; i++){      
        delete [] pipe_array[i][0];
        delete [] pipe_array[i][1];
    
        pipe_array[i][0] = NULL;
        pipe_array[i][1] = NULL;
    
        delete [] pipe_array[i];
        pipe_array[i] = NULL;
    }

    delete [] pipe_array;
    delete [] subfolders;
    delete [] monitor_inputs;

    subfolders     = NULL;
    pipe_array     = NULL;
    monitor_inputs = NULL;

    remove_directory("pipes");
}