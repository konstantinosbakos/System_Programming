#include <iomanip>

#include "user_commands.hpp"

void population_status_one(int num_of_vrs, skip_list ***vaccine_skip_l, std::string *Command_Array){
    if(!check_date_format(Command_Array[3]) || !check_date_format(Command_Array[4])){
        std::cout << "Error: Please check the date format." << std::endl;
        return;
     }

    if(!(is_valid_date(Command_Array[3]) && is_valid_date(Command_Array[4])) || compare_dates(Command_Array[3],Command_Array[4],get_todays_date())){
        std::cout << "Error: Please enter a valid date." << std::endl;
        return;
     }

    skip_list *vrs_skip      = get_skip_list(num_of_vrs,Command_Array[2],vaccine_skip_l,VACCINATED);
    block     *Sentinel_temp = vrs_skip->get_sentinel()->get_next_block(0);
    citizen   *ctzn_temp     = Sentinel_temp->get_citizen();

    float  pop      = 0.0;                                          //citizens.
    float  vac_pop  = 0.0;                                          //Vaccinated citizens.

    while(Sentinel_temp){                                           //Count each citizen to their corresponding group.
        ctzn_temp = Sentinel_temp->get_citizen();
        if((ctzn_temp->get_country() == Command_Array[1]) && (ctzn_temp->get_is_vacc())){
            pop++;
            (compare_dates(ctzn_temp->get_date(),Command_Array[3],Command_Array[4])? vac_pop++ : 0);
         }
        Sentinel_temp = Sentinel_temp->get_next_block(0);
     }
    
    vrs_skip      = get_skip_list(num_of_vrs,Command_Array[2],vaccine_skip_l,NOT_VACCINATED);
    Sentinel_temp = vrs_skip->get_sentinel()->get_next_block(0);
    ctzn_temp     = Sentinel_temp->get_citizen();     
    

    while(Sentinel_temp){                                           //Count each citizen to their corresponding group.
        ctzn_temp = Sentinel_temp->get_citizen();
        if((ctzn_temp->get_country() == Command_Array[1])){
            pop++;
         }
        Sentinel_temp = Sentinel_temp->get_next_block(0);
     }
    
        std::cout << std::left << std::setw(20);
        std::cout << Command_Array[1];
        std::cout << std::setw(8) << int(vac_pop);
        if(pop == 0){
            std::cout << SET_PR((vac_pop)*100) << "%" << std::endl; //Print results.
         }
        else{
            std::cout << SET_PR((vac_pop/pop)*100) << "%" << std::endl; //Print results.
         }
}

void population_status_all(int num_of_vrs, country *cntr_list, skip_list ***vaccine_skip_l, std::string *Command_Array){
    if(!check_date_format(Command_Array[2]) || !check_date_format(Command_Array[3])){
        std::cout << "Error: Please check the date format." << std::endl;
        return;
     }

    if(!(is_valid_date(Command_Array[2]) && is_valid_date(Command_Array[3])) || compare_dates(Command_Array[2],Command_Array[3],get_todays_date())){
        std::cout << "Error: Please enter a valid date." << std::endl;
        return;
     }

    country   *cntr_temp      = cntr_list;
    skip_list *vac_skip_l     = get_skip_list(num_of_vrs,Command_Array[1],vaccine_skip_l,VACCINATED);
    skip_list *not_vac_skip_l = get_skip_list(num_of_vrs,Command_Array[1],vaccine_skip_l,NOT_VACCINATED);

    while(cntr_temp){
        float  pop      = 0.0;                                      //citizens.
        float  vac_pop  = 0.0;                                      //Vaccinated citizens.

        if(find_country(cntr_temp->get_country(),vac_skip_l)){      //If country exists in skip list.
            block     *Sentinel_temp = vac_skip_l->get_sentinel()->get_next_block(0);
            citizen   *ctzn_temp     = Sentinel_temp->get_citizen();

            while(Sentinel_temp){                                   //Count each citizen to their corresponding group.
                ctzn_temp = Sentinel_temp->get_citizen();
                if((ctzn_temp->get_country() == cntr_temp->get_country()) && (ctzn_temp->get_is_vacc())){
                    pop++;
                    (compare_dates(ctzn_temp->get_date(),Command_Array[2],Command_Array[3])? vac_pop++ : 0);
                 }
                Sentinel_temp = Sentinel_temp->get_next_block(0);
             }
         }

        if(find_country(cntr_temp->get_country(),not_vac_skip_l)){  //If country exists.
            block     *Sentinel_temp = not_vac_skip_l->get_sentinel()->get_next_block(0);
            citizen   *ctzn_temp     = Sentinel_temp->get_citizen();   

            while(Sentinel_temp){                                   //Count each citizen to their Corresponding group.
                ctzn_temp = Sentinel_temp->get_citizen();
                if((ctzn_temp->get_country() == cntr_temp->get_country())){
                    pop++;
                 }
                Sentinel_temp = Sentinel_temp->get_next_block(0);
             }
         }

        std::cout << std::left << std::setw(20);
        std::cout << cntr_temp->get_country();
        std::cout << std::setw(8) << int(vac_pop);
        if(pop == 0){
            std::cout << SET_PR((vac_pop)*100) << "%" << std::endl;     //Print results.
         }
        else{
            std::cout << SET_PR((vac_pop/pop)*100) << "%" << std::endl; //Print results.
         }
        cntr_temp = cntr_temp->get_next();
     }
}

void population_status_by_age_one(int num_of_vrs, skip_list *vac_skip_l, skip_list *not_vac_skip_l, std::string *Command_Array){
    if(!check_date_format(Command_Array[3]) || !check_date_format(Command_Array[4])){
        std::cout << "Error: Please check the date format." << std::endl;
        return;
     }

    if(!(is_valid_date(Command_Array[3]) && is_valid_date(Command_Array[4])) || compare_dates(Command_Array[3],Command_Array[4],get_todays_date())){
        std::cout << "Error: Please enter a valid date." << std::endl;
        return;
     }

    block    *Sentinel_temp = vac_skip_l->get_sentinel()->get_next_block(0);
    citizen  *ctzn_temp     = NULL;

    float  pop_00_20        = 0.0;                                  //Variables for each population group.
    float  pop_20_40        = 0.0;
    float  pop_40_60        = 0.0;
    float  pop_60_plus      = 0.0;
    float  vac_pop_00_20    = 0.0;                                  //Variables for each vaccinated population group.
    float  vac_pop_20_40    = 0.0;
    float  vac_pop_40_60    = 0.0;
    float  vac_pop_60_plus  = 0.0;

    while(Sentinel_temp){                                           //Count every citizen to their group.
        ctzn_temp = Sentinel_temp->get_citizen();
        if((ctzn_temp->get_country() == Command_Array[1]) && (ctzn_temp->get_is_vacc())){
            if     ((ctzn_temp->get_age() >= 00) && (ctzn_temp->get_age() < 20)){
                pop_00_20++;
                if(compare_dates(ctzn_temp->get_date(),Command_Array[3],Command_Array[4])){
                    vac_pop_00_20++;
                 }
             }
            else if((ctzn_temp->get_age() >= 20) && (ctzn_temp->get_age() < 40)){
                pop_20_40++;
                if(compare_dates(ctzn_temp->get_date(),Command_Array[3],Command_Array[4])){
                    vac_pop_20_40++;
                 }
             }
            else if((ctzn_temp->get_age() >= 40) && (ctzn_temp->get_age() < 60)){
                pop_40_60++;
                if(compare_dates(ctzn_temp->get_date(),Command_Array[3],Command_Array[4])){
                    vac_pop_40_60++;
                 }
             }
            else if((ctzn_temp->get_age() >= 60)){
                pop_60_plus++;
                if(compare_dates(ctzn_temp->get_date(),Command_Array[3],Command_Array[4])){
                    vac_pop_60_plus++;
                 }
             }
         }
        Sentinel_temp = Sentinel_temp->get_next_block(0);
     }

    Sentinel_temp = not_vac_skip_l->get_sentinel()->get_next_block(0);
    ctzn_temp     = Sentinel_temp->get_citizen();

    while(Sentinel_temp){                                           //Count each citizen to their group.
        ctzn_temp = Sentinel_temp->get_citizen();
        if((ctzn_temp->get_country() == Command_Array[1])){
            if     ((ctzn_temp->get_age() >= 00) && (ctzn_temp->get_age() < 20)){
                pop_00_20++;
             }
            else if((ctzn_temp->get_age() >= 20) && (ctzn_temp->get_age() < 40)){
                pop_20_40++;
             }
            else if((ctzn_temp->get_age() >= 40) && (ctzn_temp->get_age() < 60)){
                pop_40_60++;
             }
            else if((ctzn_temp->get_age() >= 60)){
                pop_60_plus++;
             }
         }
        Sentinel_temp = Sentinel_temp->get_next_block(0);
     }
    
    print_country(Command_Array[1]);                                //Print output.
    
    std::cout << "00-20: " << std::setw(5);
    std::cout << int(vac_pop_00_20);
    
    if(pop_00_20 == 0){
        std::cout << " " << std::setw(5) << SET_PR((vac_pop_00_20)*100) << "%" << std::endl;
     }
    else{
        std::cout << " " << std::setw(5) << SET_PR((vac_pop_00_20/pop_00_20)*100) << "%" << std::endl;
     }
    
    std::cout << "20-40: " << std::setw(5);
    std::cout << int(vac_pop_20_40);
    
    if(pop_20_40 == 0){
        std::cout << " " << std::setw(5) << SET_PR((vac_pop_20_40)*100)<< "%" << std::endl;
     }
    else{
        std::cout << " " << std::setw(5) << SET_PR((vac_pop_20_40/pop_20_40)*100)<< "%" << std::endl;
     }
    
    std::cout << "40-60: " << std::setw(5);
    std::cout << int(vac_pop_40_60);
    
    if(pop_40_60 == 0){
        std::cout << " " << std::setw(5) << SET_PR((vac_pop_40_60)*100)<< "%" << std::endl;
     }
    else{
        std::cout << " " << std::setw(5) << SET_PR((vac_pop_40_60/pop_40_60)*100)<< "%" << std::endl;
     }
    
    std::cout << "60+  : " << std::setw(5);
    std::cout << int(vac_pop_60_plus);
    
    if(pop_60_plus == 0){
        std::cout << " " << std::setw(5) << SET_PR((vac_pop_60_plus)*100)<< "%" << std::endl;
     }
    else{
        std::cout << " " << std::setw(5) << SET_PR((vac_pop_60_plus/pop_60_plus)*100)<< "%" << std::endl;
     }
}

void population_status_by_age_all(skip_list *vac_skip_l ,skip_list *not_vac_skip_l, country *cntr_list, std::string *Command_Array){
    if(!check_date_format(Command_Array[2]) || !check_date_format(Command_Array[3])){
        std::cout << "Error: Please check the date format." << std::endl;
        return;
     }

    if(!(is_valid_date(Command_Array[2]) && is_valid_date(Command_Array[3])) || compare_dates(Command_Array[2],Command_Array[3],get_todays_date())){
        std::cout << "Error: Please enter a valid date." << std::endl;
        return;
     }

    country   *cntr_temp    = cntr_list;

    while(cntr_temp){
        float  pop_00_20        = 0.0;                              //Variables for each population group.
        float  pop_20_40        = 0.0;
        float  pop_40_60        = 0.0;
        float  pop_60_plus      = 0.0;
        float  vac_pop_00_20    = 0.0;                              //Variables for each vaccinated population group.
        float  vac_pop_20_40    = 0.0;
        float  vac_pop_40_60    = 0.0;
        float  vac_pop_60_plus  = 0.0;
    
        if(find_country(cntr_temp->get_country(),vac_skip_l)){      //Check if the country exists in the skip list.
            block     *Sentinel_temp = vac_skip_l->get_sentinel()->get_next_block(0);
            citizen   *ctzn_temp     = Sentinel_temp->get_citizen();

            while(Sentinel_temp){                                   //Count each citizen to their corresponding group.
                ctzn_temp = Sentinel_temp->get_citizen();
                if((ctzn_temp->get_country() == cntr_temp->get_country()) && (ctzn_temp->get_is_vacc())){
                    if     ((ctzn_temp->get_age() >= 00) && (ctzn_temp->get_age() < 20)){
                        pop_00_20++;
                        if(compare_dates(ctzn_temp->get_date(),Command_Array[2],Command_Array[3])){
                            vac_pop_00_20++;
                         }
                     }
                    else if((ctzn_temp->get_age() >= 20) && (ctzn_temp->get_age() < 40)){
                        pop_20_40++;
                        if(compare_dates(ctzn_temp->get_date(),Command_Array[2],Command_Array[3])){
                            vac_pop_20_40++;
                         }
                     }
                    else if((ctzn_temp->get_age() >= 40) && (ctzn_temp->get_age() < 60)){
                        pop_40_60++;
                        if(compare_dates(ctzn_temp->get_date(),Command_Array[2],Command_Array[3])){
                            vac_pop_40_60++;
                         }
                     }
                    else if((ctzn_temp->get_age() >= 60)){
                        pop_60_plus++;
                        if(compare_dates(ctzn_temp->get_date(),Command_Array[2],Command_Array[3])){
                            vac_pop_60_plus++;
                         }
                     }
                 }
                Sentinel_temp = Sentinel_temp->get_next_block(0);
             }
         }
        if(find_country(cntr_temp->get_country(),not_vac_skip_l)){  //Check if the exists in the skip list.
            block     *Sentinel_temp = not_vac_skip_l->get_sentinel()->get_next_block(0);
            citizen   *ctzn_temp     = Sentinel_temp->get_citizen();

            while(Sentinel_temp){                                   //Count each citizen to their corresponding group
                ctzn_temp = Sentinel_temp->get_citizen();
                if((ctzn_temp->get_country() == cntr_temp->get_country())){
                    if     ((ctzn_temp->get_age() >= 00) && (ctzn_temp->get_age() < 20)){
                        pop_00_20++;
                     }
                    else if((ctzn_temp->get_age() >= 20) && (ctzn_temp->get_age() < 40)){
                        pop_20_40++;
                     }
                    else if((ctzn_temp->get_age() >= 40) && (ctzn_temp->get_age() < 60)){
                        pop_40_60++;
                     }
                    else if((ctzn_temp->get_age() >= 60)){
                        pop_60_plus++;
                     }
                 }
                Sentinel_temp = Sentinel_temp->get_next_block(0);
             }
         }
            
        print_country(cntr_temp->get_country());                    //Print output.
        
        std::cout << "00-20: " << std::setw(5);
        std::cout << int(vac_pop_00_20);
        
        if(pop_00_20 == 0){
            std::cout << " " << std::setw(5) << SET_PR((vac_pop_00_20)*100) << "%" << std::endl;
         }
        else{
            std::cout << " " << std::setw(5) << SET_PR((vac_pop_00_20/pop_00_20)*100) << "%" << std::endl;
         }
        
        std::cout << "20-40: " << std::setw(5);
        std::cout << int(vac_pop_20_40);
        
        if(pop_20_40 == 0){
            std::cout << " " << std::setw(5) << SET_PR((vac_pop_20_40)*100)<< "%" << std::endl;
         }
        else{
            std::cout << " " << std::setw(5) << SET_PR((vac_pop_20_40/pop_20_40)*100)<< "%" << std::endl;
         }
        
        std::cout << "40-60: " << std::setw(5);
        std::cout << int(vac_pop_40_60);
        
        if(pop_40_60 == 0){
            std::cout << " " << std::setw(5) << SET_PR((vac_pop_40_60)*100)<< "%" << std::endl;
         }
        else{
            std::cout << " " << std::setw(5) << SET_PR((vac_pop_40_60/pop_40_60)*100)<< "%" << std::endl;
         }
        
        std::cout << "60+  : " << std::setw(5);
        std::cout << int(vac_pop_60_plus);
        
        if(pop_60_plus == 0){
            std::cout << " " << std::setw(5) << SET_PR((vac_pop_60_plus)*100)<< "%" << std::endl;
         }
        else{
            std::cout << " " << std::setw(5) << SET_PR((vac_pop_60_plus/pop_60_plus)*100)<< "%" << std::endl;
         }

        cntr_temp = cntr_temp->get_next();
     }
}