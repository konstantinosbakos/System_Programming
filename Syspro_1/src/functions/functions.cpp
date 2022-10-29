#include <chrono>

#include "functions.hpp"

std::string get_todays_date(){                                          //Return current date.
    std::chrono::time_point<std::chrono::_V2::system_clock,std::chrono::_V2::system_clock::duration> now = std::chrono::system_clock::now();
    std::time_t now_c      = std::chrono::system_clock::to_time_t(now);
    struct tm *time_stamps = std::localtime(&now_c);

    return std::to_string(time_stamps->tm_mday)+"-"+std::to_string((time_stamps->tm_mon)+1)+"-"+std::to_string((time_stamps->tm_year)+1900);
}

int *date_to_int(std::string *date){                                    //Turn the string array of the date to int array.
    int *int_array = new int[3];
    
    int_array[0] = stoi(date[0]);
    int_array[1] = stoi(date[1]);
    int_array[2] = stoi(date[2]);

    delete [] date;                                                     //Free string array.
    date = NULL;

    return int_array;
}

std::string *break_date(std::string _date){                             //Break the date string and return a string array.
    int          j     = 0;
    std::string *array = new std::string[3];
    
    array[0] = "";
    for(int i=0; i<int(_date.length()); i++){
        if(_date[i] == '-'){
            j++;
            array[j] = "";
         }
        else{
            array[j] += _date[i];
         }
     }
    return array;
}

std::string *split_to_words(std::string user_input, int &num_of_words){ //Split string to separate words based on white spaces.
    if(user_input == ""){
        std::string *Array = new std::string[1];
        Array[0]  = " ";
        return Array;
     }

    if(user_input.find_first_not_of(' ') == user_input.npos){
        std::string *Array = new std::string[1];
        Array[0]  = " ";
        return Array;
     }

    num_of_words  = 0;
    int _begin    = user_input.find_first_not_of(' ');                  //Trim input.
    int _end      = user_input.find_last_not_of(' ') + 1;

    std::string clean_input = user_input.substr(_begin,_end);           //Store trimmed input.

    for(int i=0; i<int(clean_input.length()-1); i++){                   //Count number of words.
        if((clean_input[i] == ' ') && (i != 0)){
            if(clean_input[i-1] != ' '){
                num_of_words++;
             }
         }
     }
    num_of_words++;

    if(num_of_words > 9){
        return NULL;
     }
    
    std::string *Array = new std::string[num_of_words];                 //Create string array.
    
    int j     = 0;
    Array[0]  = "";
    for(int i=0; i<int(clean_input.length()); i++){                     //Stor each word to the string array and return it.
        if(clean_input[i] != ' '){
            Array[j] += clean_input[i];
         }
        else if(i != 0){
            if(clean_input[i-1] != ' '){
                j++;
                Array[j]  = "";
             }
         }
     }
    return Array;
}

skip_list ***create_skip_lists(data_base *db, citizen *Sentinel_Dummy){ //Create skip list array.
	int          i     = 0;
	virus       *temp  = db->get_vrs_ptr();
	skip_list ***Array = new skip_list**[db->get_vrs_num()];

	for(int i=0; i<db->get_vrs_num(); i++){
		Array[i] = new skip_list*[2];
	 }

	while(temp){
		Array[i][NOT_VACCINATED] = new skip_list(NOT_VACCINATED,temp,Sentinel_Dummy);                 //For every virus, there are two cells in the array, one for the
		Array[i][VACCINATED]     = new skip_list(VACCINATED,temp,Sentinel_Dummy);                     //vaccinated and one for the not vaccinated.
		temp = temp->get_next();
		i++;
	 }
	return Array;
}

skip_list *get_skip_list(int num_of_vrs, std::string virus, skip_list ***vaccine_skip_l, bool is_vacc){ //Get skip list based on a specific virus.
    if(is_vacc){
        for(int i=0; i<num_of_vrs; i++){
            if(vaccine_skip_l[i][VACCINATED]->get_virus() == virus){
                return vaccine_skip_l[i][VACCINATED];
             }
         }
     }
    else{
        for(int i=0; i<num_of_vrs; i++){
            if(vaccine_skip_l[i][NOT_VACCINATED]->get_virus() == virus){
                return vaccine_skip_l[i][NOT_VACCINATED];
             }
         }  
     }
    return NULL;
}

bloom_filter **create_bloom_filters(long bloom_size,data_base *data){                                 //Create bloom filter array. One cell for each virus.
	int i                = 0;
	virus *temp          = data->get_vrs_ptr();
	bloom_filter **Array = new bloom_filter*[data->get_vrs_num()];

	while(temp){
		Array[i] = new bloom_filter(bloom_size,temp);
		temp = temp->get_next();
		i++;
	 }

	return Array;
}

bloom_filter *get_bloom_filter(int num_of_vrs, std::string virus, bloom_filter **vacced_bloom){       //Get bloom filter based on a specific virus.
    for(int i=0; i<num_of_vrs; i++){
        if(vacced_bloom[i]->get_virus() == virus){
            return vacced_bloom[i];
         }
     }
    return NULL;
}
