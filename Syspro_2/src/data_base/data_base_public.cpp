#include "data_base.hpp"
#include "../functions/functions.hpp"


data_base::data_base(){ //Initialize data base.
    this->vrs_num     = 0;
    this->nam_head    = NULL;
    this->vrs_head    = NULL;
    this->ctzn_head   = NULL;
    this->ctzn_tail   = NULL;
    this->cntr_head   = NULL;
    this->surnam_head = NULL;
}

data_base::~data_base(){
    name       *nm_temp       = this->nam_head;
    virus      *v_temp        = this->vrs_head;
    citizen    *ctzn_temp     = this->ctzn_head;
    country    *c_temp        = this->cntr_head;
    surname    *snm_temp      = this->surnam_head;

    name       *pr_nm_temp    = this->nam_head;
    virus      *pr_v_temp     = this->vrs_head;
    citizen    *pr_ct_temp    = this->ctzn_head;
    country    *pr_c_temp     = this->cntr_head;
    surname    *pr_snm_temp   = this->surnam_head;

    while(nm_temp){                               //Free all lists in the data base.
        pr_nm_temp  = nm_temp->get_next();
        delete nm_temp;
        
        nm_temp     = NULL;
        nm_temp     = pr_nm_temp;
     }

    while(v_temp){
        pr_v_temp   = v_temp->get_next();
        delete v_temp;
        
        v_temp      = NULL;
        v_temp      = pr_v_temp;
     }

    while(ctzn_temp){
        ctzn_temp->set_country(NULL);
        ctzn_temp->set_virus(NULL);

        pr_ct_temp  = ctzn_temp->get_next();
        delete ctzn_temp;

        ctzn_temp     = NULL;
        ctzn_temp     = pr_ct_temp;
     }
    this->ctzn_tail = NULL;

    while(c_temp){
        pr_c_temp   = c_temp->get_next();
        delete c_temp;
        
        c_temp      = NULL;
        c_temp      = pr_c_temp;
     }

    while(snm_temp){
        pr_snm_temp = snm_temp->get_next();
        delete snm_temp;
       
        snm_temp    = NULL;
        snm_temp    = pr_snm_temp;
     }
}

void data_base::remove_citizen(citizen *ctzn){           //Remove citizen from data base.
    name    *n_ptr = ctzn->get_first_name_ptr();
    virus   *v_ptr = ctzn->get_virus_ptr();
    country *c_ptr = ctzn->get_country_ptr();
    surname *s_ptr = ctzn->get_surname_ptr();  

    n_ptr->decrease_usage();                             //Decrease usage of name/country/surname since one less citizen uses them.
    v_ptr->decrease_usage();
    c_ptr->decrease_usage();
    s_ptr->decrease_usage();

    if((n_ptr->get_used_by()) == 0){remove(n_ptr);}      //If the name/country/surname is not used, remove it.
    if((v_ptr->get_used_by()) == 0){remove(v_ptr);}
    if((c_ptr->get_used_by()) == 0){remove(c_ptr);}
    if((s_ptr->get_used_by()) == 0){remove(s_ptr);}

    ctzn->set_first_name(NULL);                          //Set the pointers to NULL.
    ctzn->set_virus(NULL);
    ctzn->set_country(NULL);
    ctzn->set_surname(NULL);

    if(this->ctzn_head == ctzn){
        this->ctzn_head = ctzn->get_next();
     }

    if(this->ctzn_tail == ctzn){
        this->ctzn_tail = ctzn->get_prev();
     }

    if(ctzn->get_prev()){
        ctzn->get_prev()->set_next(ctzn->get_next());    //Remove citizen from list.
     }

    if(ctzn->get_next()){
        ctzn->get_next()->set_prev(ctzn->get_prev());
     }

    delete ctzn;                                         //Delete citizen and set it to NULL.
    ctzn = NULL;
}

void data_base::add_citizen(std::string data, skip_list *information_check){ //Add citizen to the data base.
    int          num_of_words     = 0;
    int          add_citizen_flag = 0;
    std::string *broken_date;
    std::string *broken_input;

    broken_input = split_to_words(data,num_of_words);

    if(!((0 <= std::stoi(broken_input[0])) && (std::stoi(broken_input[0]) <= 9999))){
        std::cout << "Error: ID(" << broken_input[0] << ") must be between 1 and 9999" << std::endl;
        delete [] broken_input;
        return;
     }

    if(!((1 <= std::stoi(broken_input[4])) && (std::stoi(broken_input[4]) <= 120))){
        std::cout << "Error: ID(" << broken_input[0] << "): Age(" << broken_input[4] << ") must be between 1 and 120" << std::endl;
        delete [] broken_input;
        return;
     }

    block   *citizen_exists   = information_check->search(stoi(broken_input[0]));
    citizen *new_citizen      = new citizen;

    if(citizen_exists){
        if(!credential_check(citizen_exists->get_citizen(),broken_input,1)){
            delete    new_citizen;
            delete [] broken_input;
            new_citizen  = NULL;
            broken_input = NULL;
            return;
         }
     }
    else{
        add_citizen_flag = 1;
     }

    if(num_of_words == 8 && broken_input[6] == "YES"){   //If citizen is vaccinated.

        if(!is_valid_date(broken_input[7])){             //If the date is not in the correct boundaries, do not add citizen.
            delete    new_citizen;
            delete [] broken_input;
            new_citizen  = NULL;
            broken_input = NULL;      
            return;
         }

        new_citizen->set_ID(std::stoi(broken_input[0])); //Initialize the respective fields.
        new_citizen->set_first_name(check_name(broken_input[1]));
        new_citizen->set_surname(check_surname(broken_input[2]));
        new_citizen->set_country(check_country(broken_input[3]));
        new_citizen->set_age(int(std::stoi(broken_input[4])));
        new_citizen->set_virus(check_virus(broken_input[5]));
        new_citizen->set_is_vacc(VACCINATED);

        broken_date = break_date(broken_input[7]);       //Split date to three numbers.

        new_citizen->set_dd(broken_date[0]);
        new_citizen->set_mm(broken_date[1]);
        new_citizen->set_yy(broken_date[2]);

        delete [] broken_date;
        broken_date = NULL;
     }
    else if(num_of_words == 7 && broken_input[6] == "NO"){
        new_citizen->set_ID(std::stoi(broken_input[0]));
        new_citizen->set_first_name(check_name(broken_input[1]));
        new_citizen->set_surname(check_surname(broken_input[2]));
        new_citizen->set_country(check_country(broken_input[3]));
        new_citizen->set_age(int(std::stoi(broken_input[4])));
        new_citizen->set_virus(check_virus(broken_input[5]));
        new_citizen->set_is_vacc(NOT_VACCINATED); 
     }
    else{
        delete    new_citizen;
        delete [] broken_input;
        new_citizen  = NULL;
        broken_input = NULL;      
        return;
    }

    delete [] broken_input;
    broken_input = NULL;

    if(this->ctzn_tail){                                 //Insert citizen to list of citizens.
        this->ctzn_tail->set_next(new_citizen);
        new_citizen->set_prev(this->ctzn_tail);
        new_citizen->set_next(NULL);
        this->ctzn_tail = new_citizen;
     }
    else{
        this->ctzn_head = new_citizen;
        this->ctzn_tail = new_citizen;
        new_citizen->set_prev(NULL);
        new_citizen->set_next(NULL);
     }

    if(add_citizen_flag){
        information_check->insert(new_citizen);
     }
}


citizen *data_base::add_citizen(int num_of_words, std::string *input_array, skip_list *information_check, int add_citizen_flag){ //Add citizen from user input.
    citizen     *new_citizen = new citizen;
    std::string *broken_date;

    if(num_of_words == 8 && input_array[6] == "YES"){ //If one of the input cases is found, then citizen is vaccinated.

        if(!(check_date_format(input_array[7]))){
            std::cout << "Error: Please check the date format." << std::endl;
            delete new_citizen;
            return NULL;
        }

        if(!(is_valid_date(input_array[7]))){
            std::cout << "Error: Please enter a valid date." << std::endl;
            delete new_citizen;
            return NULL;
         }

        new_citizen->set_ID(std::stoi(input_array[0]));
        new_citizen->set_first_name(check_name(input_array[1]));
        new_citizen->set_surname(check_surname(input_array[2]));
        new_citizen->set_country(check_country(input_array[3]));
        new_citizen->set_age(int(std::stoi(input_array[4])));
        new_citizen->set_virus(check_virus(input_array[5]));
        new_citizen->set_is_vacc(VACCINATED);

        broken_date = break_date(input_array[7]);       

        new_citizen->set_dd(broken_date[0]);
        new_citizen->set_mm(broken_date[1]);
        new_citizen->set_yy(broken_date[2]);

        delete [] broken_date;
        broken_date = NULL;
     }
    else if(num_of_words == 7 && input_array[6] == "NO"){
        new_citizen->set_ID(std::stoi(input_array[0]));
        new_citizen->set_first_name(check_name(input_array[1]));
        new_citizen->set_surname(check_surname(input_array[2]));
        new_citizen->set_country(check_country(input_array[3]));
        new_citizen->set_age(int(std::stoi(input_array[4])));
        new_citizen->set_virus(check_virus(input_array[5]));
        new_citizen->set_is_vacc(NOT_VACCINATED); 
     }

    if(this->ctzn_tail){                                 //Insert citizen to list of citizens.
        this->ctzn_tail->set_next(new_citizen);
        new_citizen->set_prev(this->ctzn_tail);
        new_citizen->set_next(NULL);
        this->ctzn_tail = new_citizen;
     }
    else{
        this->ctzn_head = new_citizen;
        this->ctzn_tail = new_citizen;
        new_citizen->set_prev(NULL);
        new_citizen->set_next(NULL);
     }

    if(add_citizen_flag){
        information_check->insert(new_citizen);
     }
    return new_citizen;
}