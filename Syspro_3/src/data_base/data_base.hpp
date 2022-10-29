#ifndef __DATA_BASE__
#define __DATA_BASE__

#include <string>

class skip_list;

class name{
    private:
        int   used_by;                                                  //How many citizens use the name/virus/country/surname.
        char *_name;                                                    //Name/virus/country/surname.

        name *next;
    public:
        name(std::string _name);
        ~name();

        inline int   get_used_by(){return this->used_by;};               //How many citizens use the name/virus/country/surname.

        inline void  increase_usage(){this->used_by++;};                 //Increase how many citizens use the name/virus/country/surname.
        inline void  decrease_usage(){this->used_by--;};                 //Decrease how many citizens use the name/virus/country/surname.
        inline void  set_next(name *next){this->next = next;};           //Set next node of the list.

        inline name *get_next(){return this->next;};                     //Get next node of the list.

        inline std::string get_name(){return std::string(this->_name);}; //Get the name/virus/country/surname.
};

class virus{
    private:
        int    used_by;
        char  *_virus;

        virus *next;
    public:
        virus(std::string _virus);
        ~virus();

        inline int  get_used_by(){return this->used_by;};

        inline void increase_usage(){this->used_by++;};
        inline void decrease_usage(){this->used_by--;};
        inline void set_next(virus *next){this->next = next;};

        inline virus *get_next(){return this->next;};

        inline std::string get_virus(){return std::string(this->_virus);};
};

class country{
    private:
        int      used_by;
        char    *_country;

        country *next;
    public:
        country(std::string _country);
        ~country();

        inline int  get_used_by(){return this->used_by;};

        inline void increase_usage(){this->used_by++;};
        inline void decrease_usage(){this->used_by--;};
        inline void set_next(country *next){this->next = next;};

        inline country *get_next(){return this->next;};

        inline std::string get_country(){return std::string(this->_country);};
};

class surname{
    private:
        int      used_by;
        char    *_surname;

        surname *next;
    public:
        surname(std::string _surname);
        ~surname();

        inline int  get_used_by(){return this->used_by;};

        inline void increase_usage(){this->used_by++;};
        inline void decrease_usage(){this->used_by--;};
        inline void set_next(surname *next){this->next = next;};     

        inline surname *get_next(){return this->next;};

        inline std::string  get_surname(){return std::string(this->_surname);};
};

class citizen{
    private:
        int      ID;
        int      _age;
        bool     is_vacc; //Is vaccinated.
        char    *dd;
        char    *mm;
        char    *yy;
        name    *_name;
        virus   *_virus;
        surname *_surname;
        country *_country;

        citizen *next;
        citizen *prev;
    public:
        citizen();
        ~citizen();

        void set_dd(std::string _dd);
        void set_mm(std::string _mm);
        void set_yy(std::string _yy);

        inline void set_ID(int ID){this->ID = ID;};
        inline void set_age(int _age){this->_age = _age;};
        inline void set_next(citizen *_next){this->next = _next;};
        inline void set_prev(citizen *_prev){this->prev = _prev;};
        inline void set_virus(virus *_virus){this->_virus = _virus;};
        inline void set_first_name(name *_name){this->_name = _name;};
        inline void set_is_vacc(bool is_vacc){this->is_vacc = is_vacc;};
        inline void set_surname(surname *_surname){this->_surname = _surname;};
        inline void set_country(country *_country){this->_country = _country;};

        inline int get_ID(){return this->ID;};
        inline int get_age(){return this->_age;};

        inline bool get_is_vacc(){return this->is_vacc;};

        inline name    *get_first_name_ptr(){return this->_name;};

        inline virus   *get_virus_ptr(){return this->_virus;};

        inline country *get_country_ptr(){return this->_country;};

        inline citizen *get_next(){return this->next;};
        inline citizen *get_prev(){return this->prev;};

        inline surname *get_surname_ptr(){return this->_surname;};

        inline std::string get_virus(){return this->_virus->get_virus();};
        inline std::string get_first_name(){return this->_name->get_name();};
        inline std::string get_country(){return this->_country->get_country();};
        inline std::string get_surname(){return this->_surname->get_surname();};
        inline std::string get_date(){return std::string(dd)+"-"+std::string(mm)+"-"+std::string(yy);};
};

class data_base{
    private:
        int         vrs_num;
        name       *nam_head;                   //List of all names/viruses/citizens/countries/surnames.
        virus      *vrs_head;
        citizen    *ctzn_head;
        citizen    *ctzn_tail;
        country    *cntr_head;
        surname    *surnam_head;

        void remove(name *ptr);                 //Remove name/virus/country/surname.
        void remove(virus *ptr);
        void remove(country *ptr);
        void remove(surname *ptr);
        
        name    *check_name(std::string _name); //Check if name/virus/country/surname exists, or create a new one.
        virus   *check_virus(std::string _virus);
        country *check_country(std::string _country);
        surname *check_surname(std::string _surname);

    public:
        data_base();
        ~data_base();

        void remove_citizen(citizen *ctzn);
        void add_citizen(std::string data, skip_list *information_check);

        citizen *add_citizen(int num_of_words, std::string *input_array, skip_list *information_check, int add_citizen_flag); //Add citizen to list and return the respective pointer.

        inline int      get_vrs_num(){return this->vrs_num;};
        inline virus   *get_vrs_ptr(){return this->vrs_head;};
        inline country *get_cntr_ptr(){return this->cntr_head;};
        inline citizen *get_citizen_ptr(){return this->ctzn_head;};
};

#endif