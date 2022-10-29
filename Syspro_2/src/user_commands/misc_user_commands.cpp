#include <cmath>
#include <signal.h>
#include <unistd.h>

#include "user_commands.hpp"

void help(){ //Print instructions.
    std::cout << " _______________________________________________________________"  << std::endl;
    std::cout << "| /travelRequest citizenID date countryFrom countryTo virusName |" << std::endl;
    std::cout << "| /travelStats virusName date1 date2 [country]                  |" << std::endl;
    std::cout << "| /addVaccinationRecords country                                |" << std::endl;
    std::cout << "| /searchVaccinationStatus citizenID                            |" << std::endl;
    std::cout << "| /printPids                                                    |" << std::endl;
    std::cout << "| /help                                                         |" << std::endl;
    std::cout << "| /exit                                                         |" << std::endl;
    std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"  << std::endl;
    std::cout << "~> Any variable in [] can be omitted.\n"                          << std::endl;
}

void print_monitor_pids(int num_of_monitors, monitor_db **monitor_array){ //Print the current pids of the monitors.
    std::cout << " ~> Printing Monitor pids:" << std::endl;

    for(int i=0; i<num_of_monitors; i++){
        std::cout << "        Monitor " << ": ";
        std::cout << "[" << monitor_array[i]->get_monitor_pid() << "]" << std::endl;
     }

    std::cout << "        Parent  " << ": "  << "[" << getpid() << "]" << std::endl;
}   

void export_monitor_log(int num_of_folders, std::string *folder_array, country_records **monitor_countries){
    //Export the log_file of the monitor.
    int accepted_req = 0;
    int rejected_req = 0;

    for(int i=0; i<num_of_folders; i++){
        accepted_req += monitor_countries[i]->get_num_of_accepted();
        rejected_req += monitor_countries[i]->get_num_of_rejected();
     }
    export_log(accepted_req,rejected_req,num_of_folders,folder_array);
}

void terminate(int buffer_size, int accepted_req, int rejected_req, int num_of_monitors, int num_of_subfolders, std::string *subfolders, monitor_db **monitor_array){
    //Termination operation used by TravelMonitor.

    std::cout << "\n\nThe program will now exit.\n" << std::endl;

    for(int i=0; i<num_of_monitors; i++){
        kill(monitor_array[i]->get_monitor_pid(),SIGKILL);
     }
    export_log(accepted_req,rejected_req,num_of_subfolders,subfolders);
}
