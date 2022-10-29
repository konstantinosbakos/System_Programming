#include "../functions/functions.hpp"

int skip_list::list_length(int level){           //Return how many nodes a list has.
    int    length  = 0;
    block *temp    = this->get_sentinel()->get_next_block(level);

    while(temp){
        length++;
        temp = temp->get_next_block(level);
     }
    return length;
}

void skip_list::erase_list(int level){           //Erase the chosen level of a skip list.
    block *temp = this->Sentinel;
    block *next_temp;

    while(temp){
        next_temp = temp->get_next_block(level);
        temp->array_reduction();
        temp = next_temp;
     }
}

void skip_list::generate_levels(int level){                       //Creates skip list levels.
    int prev_cond_l      = this->block_num;
    int condition_length = 0;

    if(level > 1 || level == -1){                                 //If there are existing levels abovel the level given, delete them.
        if(level == -1){
            level = 1;
         }
        for(int i=this->max_level; i>=level; i--){
            this->erase_list(i);
         }
     }

    if(level == 0){
        for(int i=((this->max_level)+1); i>0; i--){
            this->erase_list(i);
         }
        level = 1;
     }

    for(; level<=this->max_level; level++){                       //Process for creating new levels.
        block *lower_level   = this->Sentinel->get_next_block(level-1);
        block *higher_level  = this->Sentinel;
        condition_length     = 0;

        while(lower_level){
            if(coin_toss()){                                      //Decide randomly if a node will be added to the level.
                higher_level->set_next_block(level,lower_level);
                higher_level = higher_level->get_next_block(level);
                higher_level->set_next_block(level,NULL);
                condition_length++;
             }
            lower_level = lower_level->get_next_block(level-1);
         }

        if(this->Sentinel->get_next_block(level) == NULL){        //If the level remained empty, remake it.
            this->Sentinel->array_reduction();                    //Reduce the levels of Sentinel to remake level.
            level--;
            continue;
         }

        if((condition_length == prev_cond_l) || (( condition_length) < ( this->max_level - level + 1))){ //If the level is the same length as the previous, 
            this->erase_list(level);                                                                     //or the level is too small, remake it.
            level--;
         }
        else{
            prev_cond_l = condition_length;                       //Set previous condition length and continue.
         }
     }
}

block ***skip_list::find_entry_pos(int id){      //Find the position in each list that the selected ID will be placed to. O(logn).
    int      levels     = this->max_level;
    block   *low_bound  = this->Sentinel;
    block   *high_bound = this->max_element;
    block ***positions  = new block **[this->max_level + 1];

    if(this->max_element->get_ID() == id){
        delete [] positions;
        return NULL;
     }

    for(int i=0; i<= this->max_level; i++){
        positions[i] = new block *[2];
     }

    for(int i=levels; i>=0; i--){
        block *temp  = low_bound->get_next_block(i);

        if(this->max_element->get_ID() < id){    //If the ID will be the max ID, create the conditions
            positions[i][0] = this->max_element; //to put it to the end of the list.
            positions[i][1] = NULL;
            continue;
         }

        while(temp && (temp != high_bound)){     //Fill the array that will contain the elements before
            int temp_ID    = temp->get_ID();     //and after the ID that will enter the list.
            int low_id  = low_bound->get_ID();
            int high_id = high_bound->get_ID();

            if(temp_ID <= low_id){
                temp = temp->get_next_block(i);
                continue;
             }
            else if(temp_ID >= high_id){         //If ID is bigger or equal to current high, break loop.
                break;
             }
            else{
                if(temp_ID > id){                //If insert_ID is lower than the current ID, make the current ID, the high ID.
                    high_bound = temp;
                    break;
                 }
                else if(temp_ID == id){
                    for(int i=0; i<=this->max_level; i++){ //Free the array with the positions.
                        positions[i][0] = NULL;
                        positions[i][1] = NULL;
                        delete [] positions[i];
                        positions[i]    = NULL;
                     }
                    delete [] positions;
                    positions = NULL;
                    return NULL;
                 }
                else{
                    low_bound = temp;                      //If insert_ID is higher than the current ID, make the current ID, the low ID.
                 }
             }
            temp = temp->get_next_block(i);
         }

        positions[i][0] = low_bound;                       //Store the pointers to the array.
        if(!temp){
            positions[i][1] = NULL;
         }
        else{
            positions[i][1] = high_bound;
         }
     }
    return positions;
}