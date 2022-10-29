#include <cmath>

#include "../functions/functions.hpp"

bloom_filter::bloom_filter(long size, virus *_virus){
    this->_virus           = _virus;                            //Save the virus for which this bloom filter is for.
    this->bloom_size       = size;                              //Bloom size given from.
    this->bloom_bit_array  = new uint8_t[this->bloom_size];     //Create array.

    for(int i=0;i<size;i++){
        this->bloom_bit_array[i] = 0;
     }
}

bloom_filter::~bloom_filter(){
    delete [] this->bloom_bit_array;                            //Delete bloom array.
    this->bloom_bit_array = NULL;
}

//------Private------

bool bloom_filter::look_Array(unsigned long position){          //Used by bloom filter to locate the position of
    long byte_pos = long((position/8));                         //the bit that is being looked.
    long bit_pos  = long(position) - byte_pos*8 ;

    return ((((this->bloom_bit_array)[byte_pos] & (1u << bit_pos)))?  1  :  0  );
}

void bloom_filter::update_Array(unsigned long position){        //Update the bloom filter array by locating the bit and turning it to 1.
    long byte_pos = long((position/8));
    long bit_pos  = long(position) - byte_pos*8;

    (this->bloom_bit_array)[byte_pos] |= (1u << bit_pos);
}

//------Public-------

bool bloom_filter::bloom_filter_search(std::string ID){         //Search for ID on the array.
    for(int i=0; i<K_VALUE; i++){
		unsigned long hash = hash_i(ID,i)%(this->bloom_size*8); //Trim hash to fit the bloom size.

        if(!look_Array(hash)){                                   //If not in array return Success.
            return false;
         }
     }
    return true;
}

void bloom_filter::bloom_filter_insert(std::string ID){         //Insert in array.
    for(int i=0; i<K_VALUE; i++){
        unsigned long hash = hash_i(ID,i)%((this->bloom_size)*8);

        update_Array(hash);                                     //Put ID in array.
     }
}
