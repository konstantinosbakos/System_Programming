#include <cstring>

#include "data_base.hpp"

name::name(std::string _name){
    this->_name   = new char[_name.size() + 1]; //Create char array to store info. 
                                                //Same process happens for virus,country,surname,DD,MM,YY.
    strcpy(this->_name,_name.c_str());          //Copy info.
    this->next    = NULL;                       //Initialize pointer to NULL.
    this->used_by = 0;
}

name::~name(){
    delete [] this->_name;
    this->_name = NULL;
}

virus::virus(std::string _virus){
    this->_virus  = new char[_virus.size() + 1];
    strcpy(this->_virus,_virus.c_str());
    this->next    = NULL;
    this->used_by = 0;
}

virus::~virus(){
    delete [] this->_virus;
    this->_virus = NULL;
}

country::country(std::string _country){
    this->_country = new char[_country.size() + 1];
    strcpy  (this->_country,_country.c_str());
    this->next     = NULL;
    this->used_by  = 0;
}

country::~country(){
    delete [] this->_country;
    this->_country = NULL;
}

surname::surname(std::string _surname){
    this->_surname = new char[_surname.size() + 1];
    strcpy(this->_surname,_surname.c_str());
    this->next     = NULL;
    this->used_by  = 0;
}

surname::~surname(){
    delete [] this->_surname;
    this->_surname = NULL;
}

citizen::citizen(){ //Initialize all pointers to NULL.
    this->dd       = NULL;
    this->mm       = NULL;
    this->yy       = NULL;
    this->_name    = NULL;
    this->_virus   = NULL;
    this->_surname = NULL;
    this->_country = NULL;

    this->next     = NULL;
    this->prev     = NULL;
}

void citizen::set_dd(std::string _dd){
    this->dd = new char[_dd.length() + 1];
    strcpy(this->dd,_dd.c_str());
}

void citizen::set_mm(std::string _mm){
    this->mm = new char[_mm.length() + 1];
    strcpy(this->mm,_mm.c_str());
}

void citizen::set_yy(std::string _yy){
    this->yy = new char[_yy.length() + 1];
    strcpy(this->yy,_yy.c_str());
}

citizen::~citizen(){
    delete [] this->dd;
    delete [] this->mm;
    delete [] this->yy;
}
