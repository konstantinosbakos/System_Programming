#include <cmath>

#include "skip_list.hpp"
#include "../functions/functions.hpp"

skip_list::skip_list(int type, virus *_virus, citizen *Sentinel_Dummy){
    this->_virus      = _virus;                    //The virus that the skip list is made for.
    this->Sentinel    = new block(Sentinel_Dummy); //Initialize sentinel.
    this->max_level   = 0;
    this->block_num   = 0;
    this->max_element = this->Sentinel;            //Max level is Sentinel (When list is empty).

    this->Sentinel->set_next_block(0,NULL);
}

skip_list::~skip_list(){                           //Free all the structures.
    block *temp = this->Sentinel;
    block *next_temp;

    while(temp){
        next_temp = temp->get_next_block(0);
        delete temp;
        temp      = NULL;
        temp      = next_temp;
     }

    this->_virus      = NULL;
    this->max_element = NULL;
}

void skip_list::print(){
        block *temp = this->get_sentinel()->get_next_block(0);
        while(temp){
            std::cout << temp->get_ID() << std::endl;
            temp = temp->get_next_block(0);
         }
}

void skip_list::insert(citizen *ctzn){                           //Insert citizen, used to execute user's input.
    int      flag      = 0;
    block   *new_block = new block(ctzn);
    block ***positions = this->find_entry_pos(ctzn->get_ID());   //An array with the positions between which
                                                                 //the citizen ID may enter. O(logn)   
    if(!positions){
        delete new_block;
        new_block = NULL;
        return;
     }

    for(int i=this->max_level; i>=0; i--){                       //Choose randomly how many levels will the ID be added to.
        if(coin_toss()){
            flag = i;
            break;
         }
     }    

    for(int i=0; i<=this->max_level; i++){                        //Add the ID to those levels.
        if(flag >= i){
            positions[i][0]->set_next_block(i,new_block);
            new_block->set_next_block(i,positions[i][1]);
         }
     }

    for(int i=0; i<=this->max_level; i++){                        //Free the array with the positions.
        positions[i][0] = NULL;
        positions[i][1] = NULL;
        delete [] positions[i];
        positions[i]    = NULL;
     }
    delete [] positions;
    positions = NULL;

    if(this->max_element->get_ID() < new_block->get_ID()){        //If new_ID > max_ID set max element.
        this->max_element = new_block;
     }

    this->block_num++;

    if(this->max_level < int(ceil(log2(this->block_num)))){       //If a level can be added, regenerate levels.
        this->max_level = int(ceil(log2(this->block_num)));
        this->generate_levels();
     }
}

block *skip_list::search(int id){                                 //Search in the skip list.
    int    levels     = this->max_level;
    block *low_bound  = this->Sentinel;
    block *high_bound = this->max_element;


    for(int i=levels; i>=0; i--){
        block *temp  = low_bound->get_next_block(i);

        while(temp){
            int temp_ID = temp->get_ID();
            int high_id = high_bound->get_ID();

            if(temp_ID  > high_id){                               //If temp ID is bigger than the high bound, break and go down a level.
                break;
             }
            else{
                if(temp_ID == id){                                //If block is found, return it.
                    return temp;
                 }
                else if(temp_ID > id){                            //If temp ID is higher than the ID being searched, and temp_ID < high_ID, then high_ID = temp_ID.
                    high_bound  = temp;
                    break;
                 }
                else{                                             //Else set new low ID.
                    low_bound = temp;
                 }
             }
            temp = temp->get_next_block(i);
         }
     }
    return NULL;                                                  //If ID not found return NULL.
}

citizen *skip_list::delete_block(int id){                         //Delete block from skip list.
    block    *del_block = this->search(id);                       //Check if ID exists.

    if(del_block){
        int       curr_length;
        int       prev_length;
        block    *prev      = NULL;
        block    *curr      = NULL;
        citizen  *ctzn      = NULL;

        for(int i=this->max_level; i>=0; i--){                    //Remove block from all lists.
            prev = this->Sentinel;
            curr = this->Sentinel->get_next_block(i);

            if(!curr){
                continue;
             }

            while(curr){
                if(curr == del_block){
                    prev->set_next_block(i,curr->get_next_block(i));
                    if(i == 0){
                        if(this->max_element == del_block){
                            this->max_element = this->Sentinel;
                         }
                        ctzn = del_block->get_citizen();
                        delete del_block;
                        del_block = NULL;
                     }
                    break;
                 }
        
                prev = curr;
                curr = curr->get_next_block(i);
             }
         }
        
        this->block_num--;

        if(this->max_element == this->Sentinel){                  //If deleted block was max element, set new max element.
            curr = this->Sentinel->get_next_block(0);
            if(curr){
                while(curr->get_next_block(0)){
                    curr = curr->get_next_block(0);
                 }
                this->max_element = curr;
             }
         }

        if(this->max_level > int(ceil(log2(this->block_num)))){   //If the skip list should have less levels, regenerate levels.
            this->max_level--;
            this->generate_levels(0);
            return ctzn;
         }
        
        curr_length = this->list_length(1);
        
        for(int level=1; level<=this->max_level; level++){        //If level number should rename the same,
            if(level == 1){                                       //check levels' length and if they are not satisfying, regenarate *some* of them.
                if(curr_length < this->max_level){
                    this->generate_levels(-1);
                    return ctzn;
                 }
                continue;
             }
            else if(level>2){
                if((prev_length == curr_length) || (curr_length > (this->max_level - level + 1))){
                    this->generate_levels(level);
                    return ctzn;
                 }
             }
            prev_length = curr_length;
            curr_length = this->list_length(level);
         }
        return ctzn;
     }
    else{
        return NULL;
     }
}
