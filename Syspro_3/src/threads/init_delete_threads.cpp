#include "threads.hpp"

void init_cond_mutex_variables(){        //Initialize all mutexes and cond variables.
    cond_init(&cond_lock);
    cond_init(&cond_read);
    cond_init(&cond_write);
    cond_init(&main_lock);
    mutex_init(&mutex_threads);
    mutex_init(&main_lock_mutex);
    mutex_init(&insert_citizen_mutex);
    mutex_init(&insert_citizen_new_virus_mutex);
}

void close_cond_mutex_variables(){       //Close (destroy) all mutexes and cond variables.
    mutex_unlock(&main_lock_mutex);      //Unlock before deleting.

    cond_destroy(&cond_lock);
    cond_destroy(&cond_read);
    cond_destroy(&cond_write);
    cond_destroy(&main_lock);
    mutex_destroy(&mutex_threads);
    mutex_destroy(&main_lock_mutex);
    mutex_destroy(&insert_citizen_mutex);
    mutex_destroy(&insert_citizen_new_virus_mutex);
}

void delete_threads(int num_of_threads, pthread_t *threads, std::string *cyclic_buffer){
    write_bool           = false;        //To unblock the threads.
    cond_broadcast(&cond_read);          //To wake up the threads.
    continue_loop_thread = false;        //To exit the threads.

    for(int i=0; i<num_of_threads; i++){ //Wait the threads to exit.
        thread_join(threads[i]);
     }
    close_cond_mutex_variables();        //Close mutexes and cond variables.

    delete [] threads;
    threads = NULL;

    delete [] cyclic_buffer;
    cyclic_buffer = NULL;
}
