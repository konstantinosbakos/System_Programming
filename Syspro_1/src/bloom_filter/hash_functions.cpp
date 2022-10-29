// Provided from the System Programming Course, 2020
// https://eclass.uoa.gr/courses/DI507/


#include <cstring>

#include "bloom_filter.hpp"

unsigned long djb2(char *str){
	int c; 
	unsigned long hash = 5381;
	
	while ((c = *str++)) {
		hash = ((hash << 5) + hash) + c; 
	 }
	return hash;
}

unsigned long sdbm(char *str){
	int c;
	unsigned long hash = 0;

	while ((c = *str++)) {
		hash = c + (hash << 6) + (hash << 16) - hash;
	 }
	return hash;
}

unsigned long hash_i(std::string str, unsigned int i){
    char   c_str[str.length()+1];
    strcpy(c_str,str.c_str());

	return djb2(c_str) + i*sdbm(c_str) + i*i;
}