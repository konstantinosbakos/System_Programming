#ifndef __SKIP_LIST__
#define __SKIP_LIST__

#include <string>

#include "../data_base/data_base.hpp"

#define VACCINATED     1
#define NOT_VACCINATED 0

class block{
    private:
        int       num_of_levels;          //Number of levels this block participates in the skip list.
        block   **level;
        citizen  *ctzn;
    public:
        block(citizen *ctzn);
        ~block();

        void array_reduction();           //Reduce the array of levels.
        void array_expansion(block *ptr); //Expand the array of levels.
        void set_next_block(int _level, block *ptr);

        block *get_next_block(int _level);

        inline int get_ID(){return this->ctzn->get_ID();};
 
        inline citizen *get_citizen(){return this->ctzn;};
};

class skip_list{
    private:
        int    max_level;
        int    block_num;
        block *Sentinel;
        block *max_element;
        virus *_virus;

        int list_length(int level);                           //Return the length of a skip list level.

        void erase_list(int level);                           //Erase chosen skip list level.
        void generate_levels(int level=1);                    //Generates or re-generates the levels of the skip list.

        block ***find_entry_pos(int id);                      //Create the arrayof pointer for the insertion of a new 
                                                              //block to the right position in each list.
    public:
        skip_list(int type, virus *_virus, citizen *Sentinel_Dummy);
        ~skip_list();

        void insert(citizen *ctzn);                           //Insert in O(logn) time (used in user commands).

        block   *search(int id);                              //Search in O(logn) time.

        citizen *delete_block(int id);                        //Delete a skip list block and regenerate the levels of the skip list if needed.

        inline int  get_block_num(){return this->block_num;};

        inline block *get_sentinel(){return this->Sentinel;};

        inline std::string get_virus(){return this->_virus->get_virus();};
};

#endif