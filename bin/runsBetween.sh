#!/bin/bash
FIRST=$(echo $1 | sed "s/[^0-9]//g" | egrep "^..........$")
LAST=$(echo $2 | sed "s/[^0-9]//g" | egrep "^..........$")
if [[ -z $LAST ]]
then
    LAST=9999999999
fi
FILELIST=( $(find ${PERDAIXDATA_PATH}/testbeam/ -name "run_*.root" | sort) )
UNIXTIMES=( $(echo ${FILELIST[*]} | sed 's/[0-9a-zA-Z\/]*\/run_//g' | sed 's/\.root//g') )
N=${#FILELIST[*]}
for (( i=0; i<${N}; i++ ))
do
    FILE=${FILELIST[i]}
    UNIXTIME=${UNIXTIMES[i]}
    if [ ${FIRST} -le ${UNIXTIME} -a ${UNIXTIME} -le ${LAST} ]
    then
        echo $FILE
    fi
done
