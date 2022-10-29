#********************************************************************************************
CXX      = g++
CXXFLAGS = -Wall -g -O3 -std=c++11 -fno-strict-overflow
CTHFLAGS = -Wall -g -O3 -std=c++11 -fno-strict-overflow -pthread
#********************************************************************************************
all: travelMonitorClient monitorServer

travelMonitorClient:   travelMonitorClient.o block.o folder_db.o\
							      monitor_db.o functions.o folder_records.o\
					 	              country_records.o bool_functions.o void_functions.o\
					 	              bloom_filter.o data_base_public.o skip_list_public.o\
					 	              travel_stats.o hash_functions.o data_base_private.o\
						              skip_list_private.o travel_request.o class_init_destruct.o\
							      init_delete_functions.o misc_user_commands.o search_vacc_status.o\
							      process_functions.o add_vacc_records_client.o bool_process_functions.o\
							      void_process_functions.o
	$(CXX) $(CXXFLAGS)   -o travelMonitorClient travelMonitorClient.o block.o folder_db.o\
							      monitor_db.o functions.o folder_records.o\
					 	              country_records.o bool_functions.o void_functions.o\
					 	              bloom_filter.o data_base_public.o skip_list_public.o\
				 		              travel_stats.o hash_functions.o data_base_private.o\
							      skip_list_private.o travel_request.o class_init_destruct.o\
							      init_delete_functions.o misc_user_commands.o search_vacc_status.o\
							      process_functions.o add_vacc_records_client.o bool_process_functions.o\
							      void_process_functions.o
#********************************************************************************************
monitorServer:         monitorServer.o block.o folder_db.o\
							               monitor_db.o functions.o folder_records.o\
				 		                       country_records.o bool_functions.o void_functions.o\
				 		                       bloom_filter.o data_base_public.o skip_list_public.o\
				 		                       travel_stats.o hash_functions.o data_base_private.o\
							               skip_list_private.o travel_request.o class_init_destruct.o\
							               init_delete_functions.o misc_user_commands.o search_vacc_status.o\
							               process_functions.o add_vacc_records_monitor.o bool_process_functions.o\
							               void_process_functions.o\
							               thread_errors.o thread_functions.o init_delete_threads.o init_monitor_structures.o
	$(CXX) $(CTHFLAGS)   -o monitorServer monitorServer.o block.o folder_db.o\
							               monitor_db.o functions.o folder_records.o\
				 	       		               country_records.o bool_functions.o void_functions.o\
				 	       		               bloom_filter.o data_base_public.o skip_list_public.o\
				 	       		               travel_stats.o hash_functions.o data_base_private.o\
							               skip_list_private.o travel_request.o class_init_destruct.o\
							               init_delete_functions.o misc_user_commands.o search_vacc_status.o\
							               process_functions.o add_vacc_records_monitor.o bool_process_functions.o\
							               void_process_functions.o\
							               thread_errors.o thread_functions.o init_delete_threads.o init_monitor_structures.o
#********************************************************************************************
block.o: ../src/skip_list/block.cpp
		$(CXX) $(CXXFLAGS)   -c ../src/skip_list/block.cpp
skip_list_public.o: ../src/skip_list/skip_list_public.cpp
	$(CXX) $(CXXFLAGS)   -c ../src/skip_list/skip_list_public.cpp
skip_list_private.o: ../src/skip_list/skip_list_private.cpp
	$(CXX) $(CXXFLAGS)   -c ../src/skip_list/skip_list_private.cpp

thread_errors.o: ../src/threads/thread_errors.cpp
	$(CXX) $(CTHFLAGS)   -c ../src/threads/thread_errors.cpp
thread_functions.o: ../src/threads/thread_functions.cpp
	$(CXX) $(CTHFLAGS)   -c ../src/threads/thread_functions.cpp
init_delete_threads.o: ../src/threads/init_delete_threads.cpp
	$(CXX) $(CTHFLAGS)   -c ../src/threads/init_delete_threads.cpp
init_monitor_structures.o: ../src/threads/init_monitor_structures.cpp
	$(CXX) $(CTHFLAGS)   -c ../src/threads/init_monitor_structures.cpp

functions.o: ../src/functions/functions.cpp
	$(CXX) $(CXXFLAGS)   -c ../src/functions/functions.cpp
bool_functions.o: ../src/functions/bool_functions.cpp
	$(CXX) $(CXXFLAGS)   -c ../src/functions/bool_functions.cpp
void_functions.o: ../src/functions/void_functions.cpp
	$(CXX) $(CXXFLAGS)   -c ../src/functions/void_functions.cpp
init_delete_functions.o: ../src/functions/init_delete_functions.cpp
	$(CXX) $(CXXFLAGS)   -c ../src/functions/init_delete_functions.cpp

monitor_db.o: ../src/process/monitor_db.cpp
		$(CXX) $(CXXFLAGS)   -c ../src/process/monitor_db.cpp
folder_db.o: ../src/process/folder_db.cpp
		$(CXX) $(CXXFLAGS)   -c ../src/process/folder_db.cpp
folder_records.o: ../src/process/folder_records.cpp
		$(CXX) $(CXXFLAGS)   -c ../src/process/folder_records.cpp
country_records.o: ../src/process/country_records.cpp
		$(CXX) $(CXXFLAGS)   -c ../src/process/country_records.cpp

bloom_filter.o: ../src/bloom_filter/bloom_filter.cpp
	$(CXX) $(CXXFLAGS)   -c ../src/bloom_filter/bloom_filter.cpp
hash_functions.o: ../src/bloom_filter/hash_functions.cpp
	$(CXX) $(CXXFLAGS)   -c ../src/bloom_filter/hash_functions.cpp

class_init_destruct.o: ../src/data_base/class_init_destruct.cpp
	$(CXX) $(CXXFLAGS)   -c ../src/data_base/class_init_destruct.cpp
data_base_public.o: ../src/data_base/data_base_public.cpp
		$(CXX) $(CXXFLAGS)   -c ../src/data_base/data_base_public.cpp
data_base_private.o: ../src/data_base/data_base_private.cpp
		$(CXX) $(CXXFLAGS)   -c ../src/data_base/data_base_private.cpp

travel_stats.o: ../src/user_commands/travel_stats.cpp
	$(CXX) $(CXXFLAGS)   -c ../src/user_commands/travel_stats.cpp
travel_request.o: ../src/user_commands/travel_request.cpp
	$(CXX) $(CXXFLAGS)   -c ../src/user_commands/travel_request.cpp
search_vacc_status.o: ../src/user_commands/search_vacc_status.cpp
	$(CXX) $(CXXFLAGS)   -c ../src/user_commands/search_vacc_status.cpp
misc_user_commands.o: ../src/user_commands/misc_user_commands.cpp
	$(CXX) $(CXXFLAGS)   -c ../src/user_commands/misc_user_commands.cpp
add_vacc_records_client.o: ../src/user_commands/add_vacc_records_client.cpp
	$(CXX) $(CXXFLAGS)   -c ../src/user_commands/add_vacc_records_client.cpp
add_vacc_records_monitor.o: ../src/user_commands/add_vacc_records_monitor.cpp
	$(CXX) $(CTHFLAGS)   -c ../src/user_commands/add_vacc_records_monitor.cpp

process_functions.o: ../src/process_functions/process_functions.cpp
	$(CXX) $(CXXFLAGS)   -c ../src/process_functions/process_functions.cpp
bool_process_functions.o: ../src/process_functions/bool_process_functions.cpp
	$(CXX) $(CXXFLAGS)   -c ../src/process_functions/bool_process_functions.cpp
void_process_functions.o: ../src/process_functions/void_process_functions.cpp
	$(CXX) $(CXXFLAGS)   -c ../src/process_functions/void_process_functions.cpp

#********************************************************************************************
clean:
	rm -f -r travelMonitorClient monitorServer *o log_files
clean_folders:
	rm -f -r log_files initialization_errors
