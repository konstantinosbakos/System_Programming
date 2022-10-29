#include <iostream>

#include "threads.hpp"
#include "../functions/functions.hpp"

int  thread_readers = 0;     //Variable that counts how many threads are on their CS.
bool local_lock     = false;
bool flag_init_1    = true;  //Flag used to isolate threads will the first file is being processed.
bool flag_init_2    = false; //Flag used to isolate threads will the first file is being processed.
bool flag_init_3    = true;  //Flag used to isolate threads will the first file is being processed.

void thread_read_cs(){                      //The CS of the threads (reading).
    if(!flag_init_1 && flag_init_2){        //Block all the threads, except the first one.
        cond_wait(&cond_lock,&mutex_threads);
     }

    for(int i=0; i<cyclic_buffer_size; i++){
        if(cyclic_buffer[i] != ""){
            if(flag_init_1 && flag_init_2){ //The first thread, initializes the structures alone, so that after the initialization all the threads can run simultaneously.
                flag_init_1 = false;
                flag_init_2 = false;
                add_to_data_base(bloom_size,cyclic_buffer[i],&vacced_bloom,&vaccine_skip_l,information_check,citizen_info,Sentinel_Dummy);
                cond_broadcast(&cond_lock);
             }
            else{                           //Add the file records to the data base.
                add_new_records(cyclic_buffer[i],bloom_size,vaccine_skip_l,vacced_bloom,information_check,citizen_info,Sentinel_Dummy);
             }
            cyclic_buffer[i] = "";
            break;
         }
     }
}

void *thread_read(void *arg){    //Many threads can run at the same time, adding records.
    while(continue_loop_thread){

        mutex_lock(&mutex_threads);
        while(write_bool){       //If the main thread is filling the cyclic_buffer, wait.
            cond_wait(&cond_read, &mutex_threads);
         }
        thread_readers++;
        
        thread_read_cs();

        thread_readers--;
        if(thread_readers == 0){ //If no one is reading, signal to the main thread to fill the cyclic_buffer again.
            if(local_lock){      //If local lock is initiated, check if the buffer is empty. If it is empty, signal that the 
                                 //main thread can proceed.
                int empty = 0;
                for(int i=0; i<cyclic_buffer_size; i++){
                    if(cyclic_buffer[i] == ""){
                        empty++;
                    }
                 }
                if(empty == cyclic_buffer_size){ //Make the flags false, if the case is met for the signal to be sent.
                    local_lock = false;
                    flag_main_lock = false;
                    cond_signal(&main_lock);
                 }
             }
            cond_signal(&cond_write);
         }
        mutex_unlock(&mutex_threads);
     }
    return NULL;
}

void thread_write_cs(folder_db **monitor_folders){                  //The write CS of the main thread.
    int             not_reg_num = 0;
    std::string    *file_list   = NULL;
    folder_records *list        = (*monitor_folders)->get_folder_list();

    for(int i=0; i<cyclic_buffer_size; i++){
        if(cyclic_buffer[i] != ""){
            continue;
         }
        folder_records *temp = list;
        while(temp){
            not_reg_num = 0;
            file_list = temp->get_not_registered(not_reg_num);      //If an unregistered file is found,
            if(not_reg_num){
                cyclic_buffer[i] = file_list[0];                    //put it in the cyclic buffer
                temp->set_one_registered(file_list[0]);             //and set it as registered.
                delete [] file_list;
                file_list = NULL;
                break;
            }
            temp = temp->get_next();
        }
     }
}

void thread_write(int num_of_folders, folder_db **monitor_folders){ //One thread (the main thread) is the write thread.
    int             not_reg_num = 0;
    int             flag        = 0;
    std::string    *file_list   = NULL;
    folder_records *folder_list = (*monitor_folders)->get_folder_list();
    folder_records *temp        = NULL;

    while(1){
        mutex_lock(&mutex_threads);
        while(thread_readers > 0 || write_bool){                    //Wait for every thread to freeze (empty buffer), before writting.
            cond_wait(&cond_write, &mutex_threads);
         }
        write_bool = true;                                          //Inform that the main thread is about to write.
        mutex_unlock(&mutex_threads);

        thread_write_cs(monitor_folders);                           //Write.

        mutex_lock(&mutex_threads);
        write_bool = false;                                         //Inform tha writing is done.

        if(flag_init_3){                                            //Initialize flags (only at the first cyclic_buffer fill).
            flag_init_2 = true;
            flag_init_3 = false;
         }

        cond_broadcast(&cond_read);                                 //Allow threads to read again.
        mutex_unlock(&mutex_threads);

        flag = 0;
        temp = folder_list;

        while(temp){
            not_reg_num = 0;
            file_list = temp->get_not_registered(not_reg_num);
            if(!not_reg_num){
                flag++;
             }
            else{
                delete [] file_list;
                file_list = NULL;
             }
            temp = temp->get_next();
         }

        if(flag == num_of_folders){                                 //If there are no more folders left, break.
            local_lock     = true;                                  //Initiate local and main lock in order to proceed only if the cyclic buffer is empty.
            flag_main_lock = true;
            break;
         }
     }
}
