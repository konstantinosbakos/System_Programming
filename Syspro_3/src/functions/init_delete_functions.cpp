#include <unistd.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#include "functions.hpp"

bool init_monitor_server(int argc, char** argv, int &port, int &num_of_threads, int &socket_buffer_size, int &cyclic_buffer_size, long &bloom_size, std::string &folder_names){ //User command line input.
    int flag = 0;

    if(argc < 12){
        return false;
     }

    for(int i=0; i<argc; i++){
        if(std::string(argv[i]) == "-p" || std::string(argv[i]) == "-t" || std::string(argv[i]) == "-b" || std::string(argv[i]) == "-c" || std::string(argv[i]) == "-s"){
            flag++;
         }
     }

    if(flag < 5){
        std::cout << "Wrong input order or missing parameter. Please type \"-h\" for input format" << std::endl;
        return false;
     }

    for(int i=1; i<argc; i++){        //Store user input.
        if(i <= 10){
            if((std::string(argv[i]) == "-p")){
                port               = to_int(argv[i+1]);
             }
            else if((std::string(argv[i]) == "-t")){
                num_of_threads     = to_int(argv[i+1]);
             }
            else if((std::string(argv[i]) == "-b")){
                socket_buffer_size = to_int(argv[i+1]);
             }
            else if((std::string(argv[i]) == "-c")){
                cyclic_buffer_size = std::stol(argv[i+1]);
             }
            else if((std::string(argv[i]) == "-s")){
                bloom_size         = std::stol(argv[i+1]);
             }
         }
        else{
            folder_names += std::string(argv[i]) + " ";
        }
     }
    return true;
}

bool init_travel_monitor_client(int argc, char** argv, int &num_of_monitors, int &socket_buffer_size, int &cyclic_buffer_size, long &bloom_size, std::string &file_name, int &num_of_threads){ //User command line input.
    int flag = 0;

    if(argc == 1){
        return false;
     }

    if(std::string(argv[1]) == "-h"){ //Print instructions if user requires them.
        std::cout << " ______________________________________________________________________ " << std::endl;
        std::cout << "| [-m] is followed by the number of Monitors that will be created.     |" << std::endl;
        std::cout << "| [-b] is followed by the socket buffer size.                          |" << std::endl;
        std::cout << "| [-c] is followed by the cyclic buffer size.                          |" << std::endl;
        std::cout << "| [-s] is followed by the size of the Bloom filter (in bytes).         |" << std::endl;
        std::cout << "| [-i] is followed by the directory name that contains the directories |" << std::endl;
        std::cout << "|      used by the monitor processes.                                  |" << std::endl;
        std::cout << "| [-t] is followed by the number of threads for each monitorServer.    |" << std::endl;
        std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << std::endl;
        return false;
     }

    for(int i=0; i<argc; i++){
        if(std::string(argv[i]) == "-m" || std::string(argv[i]) == "-b" || std::string(argv[i]) == "-c" || std::string(argv[i]) == "-s" || std::string(argv[i]) == "-i" || std::string(argv[i]) == "-t"){
            flag++;
         }
     }

    if(flag < 6 && argc != 13){
        std::cout << "Wrong input order or missing parameter. Please type \"-h\" for input format" << std::endl;
        return false;
     }

    for(int i=1; i<argc; i++){        //Store user input.
        if((std::string(argv[i]) == "-m")){
            num_of_monitors    = to_int(argv[i+1]);
         }
        else if((std::string(argv[i]) == "-b")){
            socket_buffer_size = to_int(argv[i+1]);
         }
        else if((std::string(argv[i]) == "-c")){
            cyclic_buffer_size = to_int(argv[i+1]);
         }
        else if((std::string(argv[i]) == "-s")){
            bloom_size         = std::stol(argv[i+1]);
         }
        else if((std::string(argv[i]) == "-i")){
            file_name          = argv[i+1];
         }
        else if((std::string(argv[i]) == "-t")){
            num_of_threads     = to_int(argv[i+1]);
         }
     }
    return true;
}

void init_directories(){ //Initialize the folders that the program will be using.
    remove_directory("log_files");

    mkdir("log_files",0777);
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

void initialization_delete(citizen *Sentinel_Dummy, data_base *citizen_info, struct sockaddr_in *server){
    //If TCP connections fail to initialize or
    //monitorServer fails to get the right initialization data.
    delete Sentinel_Dummy;
    Sentinel_Dummy = NULL;

    delete citizen_info;
    citizen_info = NULL;

    delete [] server;
    server = NULL;
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
                    std::cout << "\nError: Incosistent record for citizen " << ct_curr->get_ID() << ". Already exists. (" << ct_curr->get_virus() << ")" << std::endl;
                    
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
                    std::cout << "\nError: Incosistent record for citizen " << ct_curr->get_ID() << ". Already exists. (" << ct_curr->get_virus() << ")" << std::endl;
                    
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

void delete_travel_monitor_structures(int num_of_monitors, std::string *subfolders, std::string *monitor_inputs, monitor_db **monitor_array){
    for(int i=0; i<num_of_monitors; i++){
        close(monitor_array[i]->get_socket_read());
        close(monitor_array[i]->get_socket_write());
     }

    if(monitor_array){
        for(int i=0; i<num_of_monitors; i++){
            delete monitor_array[i];
            monitor_array[i] = NULL;
         }

        delete [] monitor_array;
        monitor_array = NULL;
     }

    delete [] subfolders;
    delete [] monitor_inputs;

    subfolders     = NULL;
    monitor_inputs = NULL;
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

void delete_monitor_structures(int num_of_folders, int *sockets, int *socket_id,  folder_db *monitor_folders, std::string *folder_array, country_records **monitor_countries, struct sockaddr_in *server){   //Delete structures.
    close(sockets[0]);
    close(sockets[1]);    
    
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

    delete [] socket_id;
    socket_id = NULL;

    delete [] sockets;
    sockets = NULL;

    delete [] server;
    server = NULL;
}