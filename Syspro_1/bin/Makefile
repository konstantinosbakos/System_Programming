#*****************************************************************************************
CXX      = g++
CXXFLAGS = -Wall -g -O3 -std=c++11
#*****************************************************************************************
vaccineMonitor: main.o  block.o functions.o void_functions.o\
					bool_functions.o bloom_filter.o data_base_public.o\
					skip_list_public.o skip_list_private.o data_base_private.o\
					hash_functions.o  class_init_destruct.o population_status.o\
				    misc_user_commands.o insert_and_vaccinate.o vaccine_user_commands.o
	$(CXX) $(CXXFLAGS) -o vaccineMonitor  main.o\
				       block.o functions.o void_functions.o\
				       bool_functions.o bloom_filter.o data_base_public.o\
				       skip_list_public.o skip_list_private.o data_base_private.o\
				       hash_functions.o  class_init_destruct.o population_status.o\
				       misc_user_commands.o insert_and_vaccinate.o vaccine_user_commands.o
#*****************************************************************************************
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

block.o: ../src/skip_list/block.cpp
		$(CXX) $(CXXFLAGS) -c ../src/skip_list/block.cpp
skip_list_public.o: ../src/skip_list/skip_list_public.cpp
	$(CXX) $(CXXFLAGS) -c ../src/skip_list/skip_list_public.cpp
skip_list_private.o: ../src/skip_list/skip_list_private.cpp
	$(CXX) $(CXXFLAGS) -c ../src/skip_list/skip_list_private.cpp

functions.o: ../src/functions/functions.cpp
	$(CXX) $(CXXFLAGS) -c ../src/functions/functions.cpp
bool_functions.o: ../src/functions/bool_functions.cpp
	$(CXX) $(CXXFLAGS) -c ../src/functions/bool_functions.cpp
void_functions.o: ../src/functions/void_functions.cpp
	$(CXX) $(CXXFLAGS) -c ../src/functions/void_functions.cpp

bloom_filter.o: ../src/bloom_filter/bloom_filter.cpp
	$(CXX) $(CXXFLAGS) -c ../src/bloom_filter/bloom_filter.cpp
hash_functions.o: ../src/bloom_filter/hash_functions.cpp
	$(CXX) $(CXXFLAGS) -c ../src/bloom_filter/hash_functions.cpp

class_init_destruct.o: ../src/data_base/class_init_destruct.cpp
	$(CXX) $(CXXFLAGS) -c ../src/data_base/class_init_destruct.cpp
data_base_public.o: ../src/data_base/data_base_public.cpp
		$(CXX) $(CXXFLAGS) -c ../src/data_base/data_base_public.cpp
data_base_private.o: ../src/data_base/data_base_private.cpp
		$(CXX) $(CXXFLAGS) -c ../src/data_base/data_base_private.cpp

population_status.o: ../src/user_commands/population_status.cpp
	$(CXX) $(CXXFLAGS) -c ../src/user_commands/population_status.cpp
misc_user_commands.o: ../src/user_commands/misc_user_commands.cpp
	$(CXX) $(CXXFLAGS) -c ../src/user_commands/misc_user_commands.cpp
insert_and_vaccinate.o: ../src/user_commands/insert_and_vaccinate.cpp
	$(CXX) $(CXXFLAGS) -c ../src/user_commands/insert_and_vaccinate.cpp
vaccine_user_commands.o: ../src/user_commands/vaccine_user_commands.cpp
	$(CXX) $(CXXFLAGS) -c ../src/user_commands/vaccine_user_commands.cpp

#*****************************************************************************************
clean:
	rm -f vaccineMonitor *o input.txt Initialization_Errors.txt