#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "process_functions.hpp"

char *get_ip_address(){ //Return the machine's ip address (*not* the local IP address).
    char            *ip_address = new char[IP_BUFFER];
    char 	         hostname[IP_BUFFER];
    struct hostent  *this_pc;
    struct in_addr **this_pc_address;

    gethostname(hostname,IP_BUFFER);
    memset(ip_address,'\0',IP_BUFFER);

    if((this_pc = gethostbyname(hostname)) == NULL){
        std::cout << "~> Error: Getting hostname." << std::endl;

        return NULL;
     }
    else{
        this_pc_address = (struct in_addr **) this_pc->h_addr_list;
        memcpy(ip_address,inet_ntoa((*this_pc_address)[0]),IP_BUFFER);

        return ip_address;
     }
}
