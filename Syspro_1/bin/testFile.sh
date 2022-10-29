#!/bin/bash
if [ "$#" != 4 ]                         #Check if all the arguments required are given.
then
    echo " __________________________________________________________________________ "
    echo "|Error: Wrong   number  of  arguments,  to   call  this  script            |"
    echo "|       please   put  the   files  in   the  following   order:            |"
    echo "|       ./testFile.sh virusesFile countriesFile numLines duplicatesAllowed |"
    echo "|       duplicatesAllowed  should be 0 or 1. If it is zero, the            |"
    echo "|       maximum number of lines that can be produced is 10.000.            |"
    echo " -------------------------------------------------------------------------- "
    exit 1
fi

if [[ -f "input.txt" ]]                  #Delete file if it exists already.
then
    rm input.txt
fi

array_vrs=($(<$1))                       #Get virus file.
array_cntr=($(<$2))                      #Get country file.
array_first_names=($(<first_names.txt))  #First name file.
array_last_names=($(<last_names.txt))    #Last name file.

num_vrs=${#array_vrs[@]}                 #Count the rows of the virus array.
num_cntr=${#array_cntr[@]}               #Count the rows of the country array.
num_first_names=${#array_first_names[@]} #Count the rows of the first name array.
num_last_names=${#array_last_names[@]}   #Count the rows of the last name array.

num_of_lines=$3                          #Number of lines of the output file.
dups_allowed=$4                          #If duplicated IDs are allowed.

NO='NO'
YES='YES'

dash='-'
space=' '

number_of_line=0

if [ "$dups_allowed" == 0 ] && [ "$num_of_lines" -gt 10000 ] 
then
    echo "Error: Max citizen_ID = 9999. Cannot create more than 10000 non duplicated citizens."
elif [ "$dups_allowed" == 0 ]
then
    while [ "$number_of_line" -lt "$num_of_lines" ]
        do
        
        ID="$number_of_line" 
        first_name=${array_first_names[$(($RANDOM % $num_first_names))]}
        last_name=${array_last_names[$(($RANDOM % $num_last_names))]}
        country=${array_cntr[$(($RANDOM % $num_cntr))]}
        age=$((($RANDOM % 120)+1))
        virus=${array_vrs[$(($RANDOM % $num_vrs))]}
        vaccinated=$(($RANDOM % 2))

        if [ $vaccinated == 1 ]   #If vaccinated then get a date.
        then
            dd=$((($RANDOM % 30)  + 1))
            mm=$((($RANDOM % 12)  + 1))
            yy=$((($RANDOM % 120) + 1900))
            
            echo $ID$space$first_name$space$last_name$space$country$space$age$space$virus$space$YES$space$dd$dash$mm$dash$yy >> input.txt
        else                      #Else do not calculate date.
            echo $ID$space$first_name$space$last_name$space$country$space$age$space$virus$space$NO >> input.txt
        fi

        number_of_line=$((number_of_line+1))
    done
    shuf -o input.txt < input.txt #Shufle output file in order to be mixed.
    shuf -o input.txt < input.txt #Done twice for a better result.
else
    id_num=0
    least=$(($num_of_lines/10000))
    while [ "$number_of_line" -lt "$num_of_lines" ]
        do
        
        ID=$id_num
        first_name=${array_first_names[$(($RANDOM % $num_first_names))]}
        last_name=${array_last_names[$(($RANDOM % $num_last_names))]}
        country=${array_cntr[$(($RANDOM % $num_cntr))]}
        age=$((($RANDOM % 120) + 1))

        dup_num=$((($RANDOM % 3) + $least))                          #Choose a random number for duplication between 1 and 8.

        dup_iter=0
        while [ "$dup_iter" -lt "$dup_num" ] && [ "$number_of_line" -lt "$num_of_lines" ] #Create duplications.
            do
            virus=${array_vrs[$((($RANDOM * $dup_iter) % $num_vrs))]}
            vaccinated=$(($RANDOM % 2))
            
            if [ $vaccinated == 1 ]
            then
                dd=$((($RANDOM % 30)  + 1))
                mm=$((($RANDOM % 12)  + 1))
                yy=$((($RANDOM % 120) + 1900))

                echo $ID$space$first_name$space$last_name$space$country$space$age$space$virus$space$YES$space$dd$dash$mm$dash$yy >> input.txt
            else
                echo $ID$space$first_name$space$last_name$space$country$space$age$space$virus$space$NO >> input.txt
            fi
            
            number_of_line=$((number_of_line+1))
            dup_iter=$((dup_iter+1))
        done
        
        id_num=$((id_num+1))
    done

    shuf -o input.txt < input.txt
    shuf -o input.txt < input.txt
fi
