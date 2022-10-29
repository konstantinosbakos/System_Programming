#include <iomanip>

#include "user_commands.hpp"

void list_non_vaccinated_persons(skip_list *vrs_skip_list){
    block *blck_temp = vrs_skip_list->get_sentinel()->get_next_block(0);

    if(blck_temp){ //If list not empty.
        std::cout << std::endl;
        std::cout << std::setw(20) << std::left << "ID";
        std::cout << std::setw(20) << "First Name";
        std::cout << std::setw(20) << "Surname";
        std::cout << std::setw(20) << "Country";
        std::cout << std::setw(20) << "Age";
        std::cout << std::endl     << std::endl;
     }

    while(blck_temp){
        citizen *ctzn_temp = blck_temp->get_citizen();

        std::cout << std::setw(20) << std::left << ctzn_temp->get_ID();
        std::cout << std::setw(20) << ctzn_temp->get_first_name();
        std::cout << std::setw(20) << ctzn_temp->get_surname();
        std::cout << std::setw(20) << ctzn_temp->get_country();
        std::cout << std::setw(20) << ctzn_temp->get_age() << std::endl;

        blck_temp = blck_temp->get_next_block(0);
     }
}

void system_viruses(virus *vrs_list){ //Print viruses.
    virus *temp_virus = vrs_list;

    while(temp_virus){
        std::cout << "Virus: " << temp_virus->get_virus() << std::endl;
        temp_virus = temp_virus->get_next();
     }
}

void help(){ //Print instructions.
    std::cout << " _______________________________________________________________________________________"  << std::endl;
    std::cout << "| /vaccineStatusBloom citizenID virusName                                               |" << std::endl;
    std::cout << "| /vaccineStatus citizenID virusName                                                    |" << std::endl;
    std::cout << "| /vaccineStatus citizenID                                                              |" << std::endl;
    std::cout << "| /populationStatus [country] virusName date1 date2                                     |" << std::endl;
    std::cout << "| /popStatusByAge [country] virusName date1 date2                                       |" << std::endl;
    std::cout << "| /insertCitizenRecord citizenID firstName lastName country age virusName YES/NO [date] |" << std::endl;
    std::cout << "| /vaccinateNow citizenID firstName lastName country age virusName                      |" << std::endl;
    std::cout << "| /list-nonVaccinated-Persons virusName                                                 |" << std::endl;
    std::cout << "| /system-viruses                                                                       |" << std::endl;
    std::cout << "| /help                                                                                 |" << std::endl;
    std::cout << "| /exit                                                                                 |" << std::endl;
    std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"  << std::endl;
    std::cout << "~> Any variable in [] can be omitted.\n"                                                   << std::endl;
}
