#include <chrono>
#include <dirent.h>

#include "functions.hpp"

std::string get_todays_date(){                                          //Return current date.
    std::chrono::time_point<std::chrono::_V2::system_clock,std::chrono::_V2::system_clock::duration> now = std::chrono::system_clock::now();
    std::time_t now_c       = std::chrono::system_clock::to_time_t(now);
    struct tm  *time_stamps = std::localtime(&now_c);

    return std::to_string(time_stamps->tm_mday)+"-"+std::to_string((time_stamps->tm_mon)+1)+"-"+std::to_string((time_stamps->tm_year)+1900);
}

int to_int(std::string num){ //Turn a char array to int.
    if(is_number(num)){
        int ten_p   = 1;
        for(int i=0; i<int(num.length()-1); i++){
            ten_p  *= 10;
         }

        int         int_num   = 0;
        const char *x         =  num.c_str();

        for(int i=0; i<int(num.length()); i++){
            int_num += ten_p * (x[i] - '0');
            ten_p /= 10;
         }
        return int_num;
     }
  return -1;
}

int *date_to_int(std::string str_date){ //Turn the string array of the date to int array.
    std::string *date = break_date(str_date);

    int *int_array    = new int[3];
    
    int_array[0] = to_int(date[0]);
    int_array[1] = to_int(date[1]);
    int_array[2] = to_int(date[2]);

    delete [] date;                     //Free string array.
    date = NULL;

    return int_array;
}

std::string *break_date(std::string _date){ //Break the date string and return a string array.
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

std::string get_folder_country(std::string str){                    //Isolates and returns the country name from a subfolder name.
    int position = str.find_first_of("/");
    
    std::string new_str = str.substr(position+1);
    
    return new_str;
}

std::string *get_files(std::string folder_name, int &num_of_files){ //Get the names of the files of the folder given.
    DIR           *subfolder = opendir((const char*)(folder_name.c_str()));
    struct dirent *file;

    if(!subfolder){                                                 //If folder does not exist return NULL.
        std::cout << "Folder not found. (" << folder_name << ")" << std::endl;
        return NULL;
     }

    while((file = readdir(subfolder)) != NULL){                     //Count all the files that have length 4 or more (".txt" = 4 and this is done to exclude "." and "..").
        if(int(std::string(file->d_name).length()) > 4){
            std::string is_file = std::string(file->d_name).substr(std::string(file->d_name).length() - 4);
            if(is_file == ".txt"){
                num_of_files++;
             }
         }
     }

    int          i          = 0;
    std::string *file_array = new std::string[num_of_files];        //Create the array of the files.

    seekdir(subfolder,0);                                           //Reinitialize pointer.

    while((file = readdir(subfolder)) != NULL){                     //This time iterate the folder but store the file names.
        if(int(std::string(file->d_name).length()) > 4){
            std::string is_file = std::string(file->d_name).substr(std::string(file->d_name).length() - 4);
            if(is_file == ".txt"){
                file_array[i] = folder_name + "/" + std::string(file->d_name);
                i++;
             }
         }
     }

    closedir(subfolder);                                            //Close the folder.

    file      = NULL;
    subfolder = NULL;

    return file_array;
}

std::string *sort_items(std::string *subfolders, int num_of_items){ //Bubble-sort.
    for(int i=0; i<num_of_items-1; i++){
        for(int j=0; j<num_of_items-i-1; j++){
            compare_folder_names(subfolders[j],subfolders[j+1]);
         }
     }

    return subfolders;
}

std::string *split_to_words(std::string user_input, int &num_of_words){ //Split string to separate words based on white spaces.
    if(user_input == ""){
        std::string *Array = new std::string[1];
        Array[0]  = " ";
        return Array;
     }

    if(user_input.back() == '\n'){
        user_input = user_input.substr(0,user_input.length()-1);
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

    std::string *Array = new std::string[num_of_words];                 //Create string array.
    
    int j     = 0;
    Array[0]  = "";
    for(int i=0; i<int(clean_input.length()); i++){                     //Store each word to the string array and return it.
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

std::string *get_subfolders(std::string folder_name, int &num_of_subfolders){ //Similar to get_files().
    DIR           *main_folder = opendir((const char*)(folder_name.c_str()));
    struct dirent *folder;

    if(!main_folder){
        std::cout << "Folder not found. (" << folder_name << ")" << std::endl;
        return NULL;
     }

    while((folder = readdir(main_folder)) != NULL){ 
        if(std::string(folder->d_name) != "." && std::string(folder->d_name) != ".."){
                num_of_subfolders++;
         }
     }

    int          i          = 0;
    std::string  Monitor    = "./Monitor ";
    std::string *subF_Array = new std::string[num_of_subfolders];

    seekdir(main_folder,0);

    while((folder = readdir(main_folder)) != NULL){
        if(std::string(folder->d_name) != "." && std::string(folder->d_name) != ".."){
                subF_Array[i] =   folder_name + "/" + std::string(folder->d_name);
                i++;
         }
     }

    closedir(main_folder);

    folder      = NULL;
    main_folder = NULL;

    return subF_Array;

}

std::string *get_monitor_input_RR(std::string *subfolders, int &num_of_monitors, int num_of_subfolders){
    //This function is used by travelMonitor and takes the names of the subfolders and returns the 
    //strings that will be used as the input for the Monitors, with RR (Round Robin).
    int          empty_cells        = 0; //Variable used to locate empty cells in the return array. If there are any they are deleted.
    int          curr_monitor       = 0;
    std::string *monitor_inputs     = new std::string[num_of_monitors];
    std::string *new_monitor_inputs = NULL;

    for(int i=0; i<num_of_monitors; i++){
        monitor_inputs[i] = "";
    }

    for(int i=0; i<num_of_subfolders; i++){
        monitor_inputs[curr_monitor] += (subfolders[i] + " ");

        if(curr_monitor  == num_of_monitors-1){
            curr_monitor = 0;
         }
        else{
            curr_monitor++;
         }
     }
    
    for(int i=0; i<int(num_of_monitors); i++){
        if(monitor_inputs[i] == ""){
            empty_cells     = 1;
            num_of_monitors = i;         //the number of monitors is equal to the 
         }
     }

    if(empty_cells){
        new_monitor_inputs = new std::string[num_of_monitors];
        for(int i=0; i<num_of_monitors; i++){
            new_monitor_inputs[i] = monitor_inputs[i];
         }
        delete [] monitor_inputs;
        monitor_inputs = new_monitor_inputs;
     }

    for(int i=0; i<num_of_monitors; i++){
        monitor_inputs[i].pop_back();
     }
    return monitor_inputs;
}

skip_list ***create_skip_lists(data_base *db, citizen *Sentinel_Dummy){               //Create skip list array.
	int          i     = 0;
	virus       *temp  = db->get_vrs_ptr();
	skip_list ***Array = new skip_list**[db->get_vrs_num()];

	for(int i=0; i<db->get_vrs_num(); i++){
		Array[i] = new skip_list*[2];
	 }

	while(temp){
		Array[i][NOT_VACCINATED] = new skip_list(NOT_VACCINATED,temp,Sentinel_Dummy); //For every virus, there are two cells in the array, one for the
		Array[i][VACCINATED]     = new skip_list(VACCINATED,temp,Sentinel_Dummy);     //vaccinated and one for the not vaccinated.
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

monitor_db *get_country_monitor(int num_of_monitors, std::string _country, monitor_db **monitor_array){
    //Return the Monitor of a specific country.
    for(int i=0; i<num_of_monitors; i++){
        if(monitor_array[i]->has_country(_country)){
            return monitor_array[i];
         }   
     }
    return NULL; //If country does not exist return NULL.
}

bloom_filter **create_bloom_filters(long bloom_size,data_base *data){ //Create bloom filter array. One cell for each virus.
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

bloom_filter *get_bloom_filter(int num_of_vrs, std::string virus, bloom_filter **vacced_bloom){ //Get bloom filter based on a specific virus.
    for(int i=0; i<num_of_vrs; i++){
        if(vacced_bloom[i]->get_virus() == virus){
            return vacced_bloom[i];
         }
     }
    return NULL;
}

country_records *get_country_record(std::string _country, int num_of_folders, country_records **monitor_countries){
    //Get the country records for a specific country.
    for(int i=0; i<num_of_folders; i++){
        if(_country == monitor_countries[i]->get_country()){
            return monitor_countries[i];
         }
     }
    return NULL;
}