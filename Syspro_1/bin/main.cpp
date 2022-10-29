#include <fstream>
#include <iomanip>

#include "../src/bloom_filter/bloom_filter.hpp"
#include "../src/user_commands/user_commands.hpp"

int main(int argc, char** argv){
    srand(time(0));                                                  //Set rand for coin.

    std::remove("Initialization_Errors.txt");                        //Delete file if it already exists.

    int             continue_loop  = 1;                              //Flag used to exit program.
    long            bloom_size     = 0;                              //Bloom size set by user.
    citizen        *Sentinel_Dummy = new citizen;                    //A "dummy" citizen with ID -1 to act as the citizen of all sentinels.
    std::string     file_name;                                       //Name of the input file to initialize.
    std::ofstream   init_error_file("Initialization_Errors.txt");    //Open file to print initializarion errors.
    std::streambuf *cmd = std::cout.rdbuf();                         //Store cmd buffer.
    
    if(!program_init(argc,argv,bloom_size,file_name)){
        delete Sentinel_Dummy;
        return EXIT_SUCCESS;                                         //If command line imput is not correct exit program.
     }

    std::cout << "Initializing...\n" << std::endl;
    std::cout.rdbuf(init_error_file.rdbuf());                        //Temporary redirect output to file.

    Sentinel_Dummy->set_ID(-1);                                      //Initialize Sentinel Dummy.

    skip_list      *information_check = new skip_list(0,NULL,Sentinel_Dummy);            //Used to confirm that the new citizen's info are in accordance to the info that are already in the system, if there are any.
    data_base      *citizen_info      = new data_base(file_name,information_check);      //Initialize Data Base.
    skip_list    ***vaccine_skip_l    = create_skip_lists(citizen_info,Sentinel_Dummy);
    bloom_filter  **vacced_bloom      = create_bloom_filters(bloom_size,citizen_info);

    init_structures(vacced_bloom,vaccine_skip_l,citizen_info,information_check);         //Create the array of all skip lists and the array of all bloom filters.

    std::cout.rdbuf(cmd);                                            //Redirect output to command prompt again.

    Welcome();

    while(continue_loop){
        int          num_of_vrs = citizen_info->get_vrs_num();      //Number of viruses.
        int          command_num;                                   //Number of input command.
        int          num_of_words;                                  //Number of words of input.
        std::string *Command_Array;                                 //The array of the user input "broken" into words

        get_command(command_num,&Command_Array,num_of_words);

        switch(command_num){
            case VAC_STAT_BM:
                if(is_number(Command_Array[1]) && is_virus(Command_Array[2],citizen_info->get_vrs_ptr(),true)){  //Check if input meets the requirements.
                    vaccine_status(Command_Array[1],get_bloom_filter(num_of_vrs,Command_Array[2],vacced_bloom)); //Execute command.
                 }
                delete [] Command_Array;                            //Empty array for next input line.
                Command_Array = NULL;
                break;
            case VAC_STAT_VR:
                if(is_number(Command_Array[1]) && is_virus(Command_Array[2],citizen_info->get_vrs_ptr(),true)){
                    vaccine_status(std::stoi(Command_Array[1]),get_skip_list(num_of_vrs,Command_Array[2],vaccine_skip_l,VACCINATED));
                 }
                delete [] Command_Array;
                Command_Array = NULL;
                break;
            case VAC_STAT:
                if(is_number(Command_Array[1])){
                    vaccine_status(std::stoi(Command_Array[1]),num_of_vrs,vaccine_skip_l);
                 }
                delete [] Command_Array;
                Command_Array = NULL;
                break;
            case POP_STAT:
                if(num_of_words == 4){
                    if(is_virus(Command_Array[1],citizen_info->get_vrs_ptr(),true)){
                        population_status_all(num_of_vrs,citizen_info->get_cntr_ptr(),vaccine_skip_l,Command_Array);
                     }
                 }
                else{
                    if(is_country(Command_Array[1],citizen_info->get_cntr_ptr()) && is_virus(Command_Array[2],citizen_info->get_vrs_ptr(),true)){
                        population_status_one(num_of_vrs,vaccine_skip_l,Command_Array);
                     }
                 }
                delete [] Command_Array;
                Command_Array = NULL;
                break;
            case POP_STAT_AGE: 
                if(num_of_words == 4){
                    if(is_virus(Command_Array[1],citizen_info->get_vrs_ptr(),true)){
                        skip_list *vac_skip_l     = get_skip_list(num_of_vrs,Command_Array[1],vaccine_skip_l,VACCINATED);
                        skip_list *not_vac_skip_l = get_skip_list(num_of_vrs,Command_Array[1],vaccine_skip_l,NOT_VACCINATED);
                        population_status_by_age_all(vac_skip_l,not_vac_skip_l,citizen_info->get_cntr_ptr(),Command_Array);
                     }
                 }
                else{
                    if(is_country(Command_Array[1],citizen_info->get_cntr_ptr()) && is_virus(Command_Array[2],citizen_info->get_vrs_ptr(),true)){
                        skip_list *vac_skip_l     = get_skip_list(num_of_vrs,Command_Array[2],vaccine_skip_l,VACCINATED);
                        skip_list *not_vac_skip_l = get_skip_list(num_of_vrs,Command_Array[2],vaccine_skip_l,NOT_VACCINATED);
                        population_status_by_age_one(num_of_vrs,vac_skip_l,not_vac_skip_l,Command_Array);
                     }
                 }
                delete [] Command_Array;
                Command_Array = NULL;
                break;
            case INSERT_CTZN:
                if(is_number(Command_Array[1]) && is_string(Command_Array[2]) && is_string(Command_Array[3]) && is_string(Command_Array[4]) && is_valid_age(std::stoi(Command_Array[1]),std::stoi(Command_Array[5])) && is_valid_YES_NO(Command_Array[7])){
                    if(!is_virus(Command_Array[6],citizen_info->get_vrs_ptr(),false)){
                        insert_citizen_record_new_virus(num_of_words,bloom_size,Command_Array,&vaccine_skip_l,&vacced_bloom,information_check,citizen_info,Sentinel_Dummy);
                     }
                    else{
                        insert_citizen_record(num_of_words,num_of_vrs,Command_Array,vaccine_skip_l,vacced_bloom,citizen_info,information_check);
                     }
                 }
                delete [] Command_Array;
                Command_Array = NULL;
                break;
            case VAC_NOW:
                if(is_number(Command_Array[1]) && is_string(Command_Array[2]) && is_string(Command_Array[3]) && is_string(Command_Array[4]) && is_valid_age(std::stoi(Command_Array[1]),std::stoi(Command_Array[5]))){
                    if(!is_virus(Command_Array[6],citizen_info->get_vrs_ptr(),false)){
                        vaccinate_now_new_virus(num_of_words,bloom_size,Command_Array,&vaccine_skip_l,&vacced_bloom,information_check,citizen_info,Sentinel_Dummy);
                     }
                    else{
                        vaccinate_now(num_of_vrs,num_of_words,citizen_info->get_vrs_ptr(),Command_Array,vaccine_skip_l,vacced_bloom,citizen_info,information_check);
                     }
                 }
                delete [] Command_Array;
                Command_Array = NULL;
                break;
            case LS_NON_VAC:
                if(is_virus(Command_Array[1],citizen_info->get_vrs_ptr(),true)){
                    list_non_vaccinated_persons(get_skip_list(num_of_vrs,Command_Array[1],vaccine_skip_l,NOT_VACCINATED));
                 }
                delete [] Command_Array;
                Command_Array = NULL;
                break;
            case STM_VRS:
                system_viruses(citizen_info->get_vrs_ptr());
                delete [] Command_Array;
                Command_Array = NULL;
                break;
            case HELP:
                help();
                delete [] Command_Array;
                Command_Array = NULL;
                break;
            case TERMINATE:
                delete [] Command_Array;
                Command_Array = NULL;
                continue_loop = 0; //break loop.
                break;
            default:
                std::cout << "Error: Wrong format. Please type \"/help\"." << std::endl;
                delete [] Command_Array;
                Command_Array = NULL;
                break;
         }
     }

    delete_structures(citizen_info,vaccine_skip_l,vacced_bloom,information_check,Sentinel_Dummy); //Delete all structures used by the program.

    return EXIT_SUCCESS;
}
