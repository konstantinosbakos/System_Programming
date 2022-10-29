#include <signal.h>

#include "process_functions.hpp"

void monitor_signal_handler(int signal){        //A signal handler used by Monitor.
    switch(signal){
        case SIGINT:                            //How to intepret SIGINT signal.
            command  = EXPORT_LOG;
            no_print = 1;
            break;
        case SIGQUIT:                           //How to intepret SIGQUIT signal.
            command  = EXPORT_LOG;
            no_print = 1;
            break;
        case SIGUSR1:                           //How to intepret SIGUSR1 signal.
            command  = ADD_VAC_REC;
            no_print = 1;
            break;
        default:
            std::cout << "~> Error: Signal handling (Monitor)." << std::endl;
            break;
     }
}

void travel_monitor_signal_handler(int signal){ //A signal handler used by travelMonitor.
    switch(signal){
        case SIGINT:                            //How to intepret SIGINT signal.
            command  = TERMINATE;
            no_print = 1;
            break;
        case SIGQUIT:                           //How to intepret SIGQUIT signal.
            command  = TERMINATE;
            no_print = 1;
            break;
        case SIGCHLD:                           //How to intepret SIGCHLD signal.
            command  = CHLD_CRASH;
            no_print = 1;
            break;
        default:
            std::cout << "~> Error: Signal Handling (traveMonitor)." << std::endl;
            break;
    }
}
