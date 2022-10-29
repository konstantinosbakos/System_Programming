#include <cmath>

#include "functions.hpp"

bool coin_toss(){                                                                   //Return 0 or 1. The initialization of rand is in main.cpp.
    return rand()%2;
}

bool is_number(std::string str){                                                    //Check if string is a number.
    for(int i=0; i<int(str.length()); i++){
        if(str[i] > '9' || str[i] < '0'){
            std::cout << "Error: Input variable should be number but it is not. For more information type \'/help\'" << std::endl;
            return false;                                                           //If it is not, print message and return 0.
         }
     }
    return true;
}

bool is_string(std::string str){                                                    //Check if string does not contain number characters.
    for(int i=0; i<int(str.length()); i++){
        if(!(str[i] > '9' || str[i] < '0')){
            std::cout << "Error: Input variable should be std::string but it is not. For more information type \'/help\'" << std::endl;
            return false;                                                           //If it does, print message and return 0.
         }
     }
    return true;
}

bool is_valid_age(int ID,int age){
    if(age >= 1 && age <= 120){
        return true;
    }
        std::cout << "Error: Age("<< age <<") must be between 1 and 120. (ID = " << ID << ")" << std::endl; 
    return false;
}

bool is_valid_YES_NO(std::string str){                                              //Check if the YES or NO strings are either YES or NO.
    if(str == "YES" || str == "NO"){
        return true;
     }
    std::cout << "Error: Input variable should be YES/NO but it is not. For more information type \'/help\'" << std::endl;
    return false;
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

bool find_country(std::string _country, skip_list *list){                           //Check if country exists.(In skip_list)
    block *vac_l_temp = list->get_sentinel()->get_next_block(0);

    while(vac_l_temp){
        if(vac_l_temp->get_citizen()->get_country() == _country){
            return true;
         }
        vac_l_temp = vac_l_temp->get_next_block(0);
     }

    return false;                                                                   //If country does not exists print message and return 0.
}

bool is_country(std::string _country, country *cntr_list){                          //If string is not country return false.
    country *temp_country = cntr_list;

    while(temp_country){
        if(_country == temp_country->get_country()){
            return true;
         }
        temp_country = temp_country->get_next();
     }
    std::cout << "Error: Input variable should be country but it is not. For more information type \'/help\'" << std::endl;
    return false;
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

bool is_citizen_vaccinated(int num_of_words, std::string date){ //Checks if - based on the number of input words
    if((num_of_words == 7) && (date != "")){return true;}       //and if the date if empty or not - the citizen is 
    if((num_of_words == 8) && (date != "")){return true;}       //vaccinated.
    if(num_of_words == 9)                  {return true;}
    return false;
}

bool credential_check(citizen *existing_ct, std::string *Array, int i){             //Check if a citizen's info is the same as those in the data base.
    if(!(existing_ct->get_first_name() == Array[2-i] && existing_ct->get_surname() == Array[3-i] && existing_ct->get_country() == Array[4-i] && existing_ct->get_age() == std::stoi(Array[5-i]))){
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
    std::string *broken_ctzn   = break_date(ctzn_date);                             //Turn string arrays to int arrays.
    std::string *broken_date_1 = break_date(date_1);
    std::string *broken_date_2 = break_date(date_2);

    int *num_ctzn = date_to_int(broken_ctzn);
    int *num_1    = date_to_int(broken_date_1);
    int *num_2    = date_to_int(broken_date_2);

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

bool program_init(int argc, char** argv, long &bloom_size, std::string &file_name){ //User command line input.
    if(std::string(argv[1]) == "-h"){                                      //Print instructions if user requires them.
        std::cout << " _________________________________________________________________ " << std::endl;
        std::cout << "| [-c] should be followed by a file with citizen information      |" << std::endl;
        std::cout << "| [-b] should be followed with a number for the bloom filter size |" << std::endl;
        std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << std::endl;
        return false;
     }
    if(argc < 5){                                                                   //If not enough parameters return error.
        std::cout << "Wrong input order or missing parameter. Please type \"-h\" for input format" << std::endl;
        return false;
     }

    int flag = 0;
    for(int i=0; i<argc; i++){
        if(std::string(argv[i]) == "-b" || std::string(argv[i]) == "-c"){
            flag++;
         }
     }
     
     if(flag < 2){
        std::cout << "Wrong input order or missing parameter. Please type \"-h\" for input format" << std::endl;
        return false;
      }

    for(int i=1; i<argc; i++){
        if((std::string(argv[i]) == "-c")){
            file_name = argv[i+1];                                                  //Get the file name from user input.
         }
        else if((std::string(argv[i]) == "-b")){
            bloom_size = std::stol(argv[i+1]);                                      //Get the bloom size from user input.
         }
     }
    return true;
}
