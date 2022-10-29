#include "skip_list.hpp"

block::block(citizen *ctzn){
    this->ctzn          = ctzn;                       //Initialize citizen pointer.
    this->level         = new block*[1];              //Create the level array.
    this->num_of_levels = 1;                          //Initialize the number of levels that the block is in.
    this->set_next_block(0,NULL);                     //Initialize next pointer.
}

block::~block(){
    int num_of_levels = this->num_of_levels;

    for(int i=0; i<num_of_levels; i++){
        this->array_reduction();                      //Reduce the array one by one.
     }
    this->ctzn = NULL;
}

void block::array_reduction(){                        //Reduces the size of the array.
    if(this->num_of_levels > 1){
        block **new_array  = new block *[this->num_of_levels-1];

        for(int i=0; i<this->num_of_levels-1; i++){   //Copy pointers to the new array.
            new_array[i]   = this->level[i];
         }

        for(int i=0; i<this->num_of_levels; i++){     //Delete old array.
            this->level[i] = NULL;
         }
        delete [] this->level;

        this->level = new_array;                      //Set new array.
        this->num_of_levels -= 1;                     //Reduce number of levels.
     }
    else{
        this->level[0]      = NULL;                   //Set array pointer to NULL if last pointer is deleted.
        delete [] this->level;
        this->level         = NULL;
        this->num_of_levels = 0;
     }
}

void block::array_expansion(block *ptr){              //Expand array and add new pointer to it.
    int     new_num_of_levels = this->num_of_levels + 1;
    block **new_array         = new block *[new_num_of_levels];

    for(int i=0; i<this->num_of_levels; i++){         //Copy old array to the new one
        new_array[i] = this->level[i];
     }

    for(int i=0; i<new_num_of_levels-1; i++){         //Delete old array.
        this->array_reduction();
     }

    new_array[new_num_of_levels-1] = ptr;             //Add pointer to new array.
    this->level = new_array;                          //Set new array.
    this->num_of_levels = new_num_of_levels;          //Set new number of levels.
}

void   block::set_next_block(int _level, block *ptr){ //Set the next block of the specific level.
    if(_level >= this->num_of_levels){
        this->array_expansion(ptr);
     }
    else{
        this->level[_level] = ptr;
     }
}

block *block::get_next_block(int _level){             //Get the next block of the specific level.
    if(_level >= this->num_of_levels){
        this->array_expansion(NULL);
     }
    return this->level[_level];
}