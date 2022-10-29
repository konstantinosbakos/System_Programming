/*
This file contains  encapsulations of the pthread functions,
in order to check for errors and provide cleaner code where
these functions are needed.
*/

#include "threads.hpp"

void thread_join(pthread_t thread){
    if(pthread_join(thread,NULL) != 0){
        perror(("\n~> Error: Join thread (" + std::to_string(errno) + ").").c_str());
     }
}

void thread_init(pthread_t *thread, void *(*__start_routine)(void *)){
    if(pthread_create(&(*thread),NULL,__start_routine,NULL) != 0){
        perror(("\n~> Error: Thread create (" + std::to_string(errno) + ").").c_str());
     }
}

void cond_init(pthread_cond_t *cond){
    if(pthread_cond_init(&(*cond),NULL) != 0){
        perror(("\n~> Error: Condition create (" + std::to_string(errno) + ").").c_str());
     }
}

void cond_signal(pthread_cond_t *cond){
    if(pthread_cond_signal(&(*cond)) != 0){
        perror(("\n~> Error: Cond signal (" + std::to_string(errno) + ").").c_str());
     }
}

void cond_destroy(pthread_cond_t *cond){
    if(pthread_cond_destroy(&(*cond)) != 0){
        perror(("\n~> Error: Condition destroy (" + std::to_string(errno) + ").").c_str());
     }
}

void cond_broadcast(pthread_cond_t *cond){
    if(pthread_cond_broadcast(&(*cond)) != 0){
        perror(("\n~> Error: Cond wait (" + std::to_string(errno) + ").").c_str());
     }
}

void cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex){
    if(pthread_cond_wait(&(*cond),&(*mutex)) != 0){
        perror(("\n~> Error: Cond wait (" + std::to_string(errno) + ").").c_str());
     }
}

void mutex_init(pthread_mutex_t *mutex){
    if(pthread_mutex_init(&(*mutex), NULL) != 0){
        perror(("\n~> Error: Mutex create (" + std::to_string(errno) + ").").c_str());
     }
}

void mutex_lock(pthread_mutex_t *mutex){
    if(pthread_mutex_lock(&(*mutex)) != 0){
        perror(("\n~> Error: Mutex lock (" + std::to_string(errno) + ").").c_str());
     }
}

void mutex_unlock(pthread_mutex_t *mutex){
    if(pthread_mutex_unlock(&(*mutex)) != 0){
        perror(("\n~> Error: Mutex unlock (" + std::to_string(errno) + ").").c_str());
     }
}

void mutex_destroy(pthread_mutex_t *mutex){
    if(pthread_mutex_destroy(&(*mutex)) != 0){
        perror(("\n~> Error: Mutex destroy (" + std::to_string(errno) + ").").c_str());
     }
}
