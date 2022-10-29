#ifndef __BLOOM_FILTER__
#define __BLOOM_FILTER__

#include "../data_base/data_base.hpp"

#define K_VALUE 16

class bloom_filter{
    private:
        char    *bloom_bit_array;
        long     bloom_size;                                               //Size of the array.
        virus   *_virus;

        bool look_Array(unsigned long position);                           //Reads a bit of the array.

        void update_Array(unsigned long position);                         //Updates a bit of the array.
    public:
        bloom_filter(long size, virus *_virus, char *bloom_string = NULL);
        ~bloom_filter();

        bool bloom_filter_search(std::string ID);                          //Search in bloom filter.

        void bloom_filter_insert(std::string ID);                          //Insert in bloom filter.
        void set_bloom_filter(char *bloom_string);                         //Set bloom array.

        char *get_bloom_array(){return this->bloom_bit_array;};

        inline std::string get_virus(){return this->_virus->get_virus();}; //Return virus of bloom filter.
};

unsigned long hash_i(std::string str, unsigned int i);                     //Hash function.

#endif