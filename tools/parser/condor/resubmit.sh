#! /bin/bash

for i in $(find STD/ -name "*.STD"); do 
    match=$(tail -n1 $i | grep "Finished conversion")
    if [ -z "${match}" ]; then
        filestem=$(echo $i | sed 's/STD\/\(run_[0-9]*\).STD/\1/');
        condor_submit condor/${filestem}.condor
    fi;
done
