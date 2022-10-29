#include <iomanip>

#include "user_commands.hpp"

void vaccine_status(std::string ID, bloom_filter *bloom){
    if(bloom->bloom_filter_search(ID)){                      //If ID exists, print info.
        std::cout << "~> MAYBE" << std::endl;
     }
    else{
        std::cout << "~> NOT VACCINATED" << std::endl;
     }
}

void vaccine_status(int ID, skip_list *skip_l){
    block *ctzn_block = skip_l->search(ID);                  //If ID exists.
    citizen *ctzn     = NULL;
    
    if(ctzn_block){
        ctzn = ctzn_block->get_citizen();
     }

    if(ctzn){                                                //Print info.
        std::cout << "~> VACCINATED ON " << ctzn->get_date() << std::endl;
     }
    else{
        std::cout << "~> NOT VACCINATED." << std::endl;
     }
}

void vaccine_status(int ID, int num_of_vrs, skip_list ***vaccine_skip_l){
    block *b_ct_temp = NULL;
    citizen *ct_temp = NULL;

    for(int i=0; i<num_of_vrs; i++){
        b_ct_temp = vaccine_skip_l[i][VACCINATED]->search(ID);                                         //If the ID exists in the skip list.

        if(b_ct_temp){
            ct_temp   = b_ct_temp->get_citizen();
            b_ct_temp = NULL;
         }

        if(ct_temp){
            std::cout << "~>" << ct_temp->get_virus() << " YES " << ct_temp->get_date() << std::endl; //Print info.
            ct_temp = NULL;
         }

        b_ct_temp = vaccine_skip_l[i][NOT_VACCINATED]->search(ID);

        if(b_ct_temp){                                                                               //If ID exists.
            ct_temp   = b_ct_temp->get_citizen();
            b_ct_temp = NULL;
         }

        if(ct_temp){
            std::cout << "~>" << ct_temp->get_virus() << " NO" << std::endl;                          //Print info.
            ct_temp = NULL;
         }
     }

}
