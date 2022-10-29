#ifndef __THREADS__
#define __THREADS__

#include <stdlib.h>
#include <pthread.h>

#include "../user_commands/user_commands.hpp"

extern int          cyclic_buffer_size;
extern bool         write_bool;
extern bool         flag_main_lock;
extern bool         continue_loop_thread;
extern long         bloom_size;
extern std::string *cyclic_buffer;

extern citizen        *Sentinel_Dummy;
extern data_base      *citizen_info;
extern skip_list    ***vaccine_skip_l;
extern skip_list      *information_check;
extern bloom_filter  **vacced_bloom;

extern pthread_cond_t cond_lock;
extern pthread_cond_t cond_read;
extern pthread_cond_t main_lock;
extern pthread_cond_t cond_write;

extern pthread_mutex_t mutex_threads;
extern pthread_mutex_t main_lock_mutex;
extern pthread_mutex_t insert_citizen_mutex;
extern pthread_mutex_t insert_citizen_new_virus_mutex;

//---------thread_functions.cpp---------

//----------------read------------------
void thread_read_cs();
void *thread_read(void *arg);
//----------------write-----------------
void thread_write_cs(folder_db **monitor_folders);
void thread_write(int num_of_folders, folder_db **monitor_folders);

//----------threads_errors.cpp----------

//---------------Thread-----------------
void thread_join(pthread_t thread);
void thread_init(pthread_t *thread, void *(*__start_routine)(void *));

//---------------Cond-------------------
void cond_init(pthread_cond_t *cond);
void cond_signal(pthread_cond_t *cond);
void cond_destroy(pthread_cond_t *cond);
void cond_broadcast(pthread_cond_t *cond);
void cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);

//---------------Mutex------------------
void mutex_init(pthread_mutex_t *mutex);
void mutex_lock(pthread_mutex_t *mutex);
void mutex_unlock(pthread_mutex_t *mutex);
void mutex_destroy(pthread_mutex_t *mutex);

//-------init_delete_threads.cpp--------
void init_cond_mutex_variables();
void close_cond_mutex_variables();
void delete_threads(int num_of_threads, pthread_t *threads, std::string *cyclic_buffer);

//------init_monitor_structures.cpp-----
void add_new_records(std::string file_name, long bloom_size, skip_list ***vaccine_skip_l, bloom_filter **vacced_bloom, skip_list *information_check, data_base *citizen_info, citizen *Sentinel_Dummy);

#endif