#!/bin/bash

if [ ! -d ${1} ]; then
  echo "Directory ${1} does not exists"
  exit 1
fi

if [ ! -d ${2} ]; then
  echo "Directory ${2} does not exists"
  exit 1
fi

PATH1=$(cd ${1}; pwd; cd - > /dev/null)
PATH2=$(cd ${2}; pwd; cd - > /dev/null)

MISSING_FILES_1=$(rsync --prune-empty-dirs -rnvc --ignore-existing ${PATH1}/ ${PATH2}/ | tail -n +2 | head -n -3)
echo ""
echo "Missing files in ${PATH2}:"
for FILE in ${MISSING_FILES_1} ; do
  echo $FILE
done
  
MISSING_FILES_2=$(rsync --prune-empty-dirs -rnvc --ignore-existing ${PATH2}/ ${PATH1}/ | tail -n +2 | head -n -3)
echo ""
echo "Missing files in ${PATH1}:"
for FILE in ${MISSING_FILES_2} ; do
  echo $FILE
done

DIFFERING_FILES=$(rsync --prune-empty-dirs -rnvc --existing ${PATH2}/ ${PATH1}/ | tail -n +2 | head -n -3)
echo ""
echo "Differing files:"
for FILE in ${DIFFERING_FILES} ; do
  echo $FILE
done
echo ""
