#!/bin/bash
if [ "$#" != 3 ]                   #Check if all the arguments required are given.
then
    echo " ____________________________________________________________________ "
    echo "|Error: Wrong number of arguments, to call this script               |"
    echo "|       please put the files  in  the ollowing  order:               |"
    echo "|       ./create_infiles.sh inputFile input_dir numFilesPerDirectory |"
    echo " -------------------------------------------------------------------- "
    exit 1
fi

inputFile=$1                       #The file that contains the information.
input_dir=$2                       #The name of the folder that will contain the country-folders
numFilesPerDirectory=$3            #The number of .txt files that each country will have.

if [[ -d "$input_dir" ]]           #If the folder exists then exit.
then
    echo "Error : Input folder already exists."
    exit
fi

sort -k4 "$inputFile" > sorted.txt #Create a sorted version of the input file, byt sorting the countries.
                                   #This will allow the script to iterate through the file only once.
mkdir "$input_dir"                 #Create the input folder.

dash="-"
slash="/"
txt=".txt"

country=""

i=1
while read line 
    do
        if [ $i -lt $numFilesPerDirectory ]                                 #This if is used in order to create the Round-Robin effect and to keep the variable i <= numFilesPerDirectory.
        then
            i=$((i+1))
        else
            i=1
        fi

        if [ "$country" != $(echo $line | cut -f4 -d' ') ]                  #Isolate the country of the line. If it is the same then continue.
        then
            country=$(echo $line | awk '{print $4}')                        #If it is not the same, create a new subfolder for the new country and the new .txt files.
            mkdir -p $input_dir$slash$country
            j=1
            while [ $j -lt $numFilesPerDirectory ]                          #Create numFilesPerDirectory files.
                do
                touch $input_dir$slash$country$slash$country$dash$j$txt
                j=$((j+1))
            done
            i=1
        fi
        
        echo $line >> $input_dir$slash$country$slash$country$dash$i$txt     #Echo the line to the current file.
    done < sorted.txt                                                       #The file that is being iterated.

rm sorted.txt                                                               #Remove sorted.txt because it is no longer needed.