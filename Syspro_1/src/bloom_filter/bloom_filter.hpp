#ifndef __BLOOM_FILTER__
#define __BLOOM_FILTER__

#include "../data_base/data_base.hpp"

#define K_VALUE 16

class bloom_filter{
    private:
        long     bloom_size;                           //Size of the array.
        virus   *_virus;
        uint8_t *bloom_bit_array;

        bool look_Array(unsigned long position);       //Reads a bit of the array.

        void update_Array(unsigned long position);     //Updates a bit of the array.
    public:
        bloom_filter(long size, virus *_virus);
        ~bloom_filter();

        bool bloom_filter_search(std::string ID);      //Search in bloom filter.

        void bloom_filter_insert(std::string ID);      //Insert in bloom filter.

        inline std::string get_virus(){return this->_virus->get_virus();}; //Return virus of bloom filter.
};

unsigned long hash_i(std::string str, unsigned int i); //Hash function.

#endif