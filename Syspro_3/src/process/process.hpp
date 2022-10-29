#ifndef __PROCESS__
#define __PROCESS__

#include <string>

#define ACCEPTED_REQ  1 //Accepted request.
#define REJECTED_REQ  0 //Rejected request.

#define NOT_REG '0'     //File not registered to the monitor structures.
#define REG     '1'     //File registered to the monitor structures.

#define TRVL_CLNT_R  1  //Used by travelMonitor to determine the reading socket.
#define TRVL_CLNT_W  0  //Used by travelMonitor to determine the writing socket.

class virus;
class bloom_filter;
class monitor_db{       //A class used by travelMonitor, to store needed information about the Monitors.
    private:
        int           *ports;
        int           *sockets;                                                     //The file descriptor of the pipe that travelMonitor will be writting.
        int            monitor_num;                                                 //The number of the monitor in the monitor array.
        int            num_of_blooms;                                               //The number of bloom filters the monitor has.
        int            num_of_countries;                                            //The number of countries the monitor has.
        long           bloom_size;                                                  //The bloom size.
        virus         *vrs_list;                                                    //The virus list (viruses the monitor has).
        std::string   *cntr_array;                                                  //The countries of the Monitor.
        bloom_filter **bloom_filters;                                               //The bloom filters of the Monitor.

        virus *get_virus(std::string _virus);
    public:
        struct sockaddr_in *server_connections;

        monitor_db(int monitor_num, long bloom_size, std::string input);
        ~monitor_db();

        bool has_virus(std::string _virus);
        bool has_country(std::string _country);
        bool search_bloom_filter(std::string ID, std::string _virus);

        void add_bloom_filter(char *bloom_string, std::string _virus);
        void update_bloom_filter(char *bloom_string, std::string _virus);

        std::string *get_countries(int &num_of_countries);

        inline int  get_monitor_num(){return this->monitor_num;};                   //Get the number of the monitor (in the array of monitors).
        inline int  get_num_of_blooms(){return this->num_of_blooms;};               //Get the number of the bloom filters of the monitor.
        inline int  get_port_read(){return this->ports[TRVL_CLNT_R];};
        inline int  get_port_write(){return this->ports[TRVL_CLNT_W];};
        inline int  get_socket_read(){return this->sockets[TRVL_CLNT_R];};          //Get the pipe that the travelMonitor reads from (and the Monitor writes to).
        inline int  get_socket_write(){return this->sockets[TRVL_CLNT_W];};         //Get the pipe that the travelMonitor reads from (and the Monitor writes to).
        inline int  get_num_of_countries(){return this->num_of_countries;};         //Get the number of the countries of the monitor.

        inline void set_port_read(int port_read){this->ports[TRVL_CLNT_R]       = port_read;};      //Set the monitor pipe.
        inline void set_port_write(int port_write){this->ports[TRVL_CLNT_W]     = port_write;};     //Set the monitor pipe.
        inline void set_socket_read(int socket_read){this->sockets[TRVL_CLNT_R]   = socket_read;};  //Set the monitor pipe.
        inline void set_socket_write(int socket_write){this->sockets[TRVL_CLNT_W] = socket_write;}; //Set the monitor pipe.

        inline bloom_filter **get_bloom_ptr(){return this->bloom_filters;}; //Get the bloom filters' pointer.
};

class folder_records{ //A class used in order to store the names of the files that name been read (used by Monitor).
    private:
        int           num_of_files; //The number of files of the folder.
        std::string **file_array;   //The file array, [name_of_file][is_registered].
        std::string   folder_name;  //The name of the folder (country).

        folder_records *next;       //Next in list.
    public:
        folder_records(std::string folder_name);
        ~folder_records();

        bool update_file_array();

        void set_one_registered(std::string file_name);

        std::string  get_country();
        std::string *get_not_registered(int &num_of_not_reg);

        inline void set_next(folder_records *next){this->next = next;};

        inline std::string get_folder(){return this->folder_name;};

        inline folder_records *get_next(){return this->next;};
};

class folder_db{ //A class used by Monitor that has all the folders and files that the Monitor controls.
    private:
        folder_records *folder_list;
    public:
        folder_db(int num_of_folders, std::string *folder_array);
        ~folder_db();

        folder_records *get_folder(std::string _country);

        inline folder_records *get_folder_list(){return this->folder_list;};
};

class date_list{ //A class that is used in the class below, and stores all the requests that happened on each day (and for which virus), as well as if they were accepted or not.
    private:
        int          num_of_acc_req; //Number of accepted requests.
        int          num_of_rej_req; //Number of rejected requests.
        virus       *_virus;         //The virus of the requests.
        std::string  date;           //The date of the requests.
        
        date_list   *next;
    public: 
        date_list(std::string date, virus *_virus);
        ~date_list();

        std::string get_virus();

        inline int get_num_of_acc_req(){return this->num_of_acc_req;};  //Get number of accepted requests on the date.
        inline int get_num_of_rej_req(){return this->num_of_rej_req;};  //Get number of rejected requests on the date.

        inline void set_next(date_list *next){this->next = next;};
        inline void increase_num_of_acc_req(){this->num_of_acc_req++;}; //Increase the number of accepted requests of the date.
        inline void increase_num_of_rej_req(){this->num_of_rej_req++;}; //Increase the number of rejected requests of the date.

        inline date_list *get_next(){return this->next;};

        inline std::string get_date(){return this->date;};              //Get the date.
};

class country_records{ //A class used by moitor to save all the records for the accepted and rejected requests on each country and their respective dates.
    private:
        date_list   *dates;
        std::string  _country;
    public:
        country_records(std::string country);
        ~country_records();

        int get_num_of_accepted();
        int get_num_of_rejected();

        void update_date_stats(int req, std::string date, virus *_virus);
        void get_period_stats(int &accepted, int &rejected, std::string _virus, std::string date_start, std::string date_finish);

        date_list get_date_struct(std::string date);

        std::string *get_contents(int &num_of_files);

        inline std::string  get_country(){return this->_country;}; //Get the country.
};

#endif