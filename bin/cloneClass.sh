#!/bin/bash
ORIG_CLASS=$(basename $1 | sed -e "s/\.$//g" | sed -e "s/\.cc$//g" | sed -e "s/\.hh$//g")
ORIG_PATH=$(dirname $1)
DEST_CLASS=$(basename $2 | sed -e "s/\.$//g" | sed -e "s/\.cc$//g" | sed -e "s/\.hh$//g")
DEST_PATH=$(dirname $2)
echo "${ORIG_PATH}/${ORIG_CLASS}.hh -> ${DEST_PATH}/${DEST_CLASS}.hh"
cat ${ORIG_PATH}/${ORIG_CLASS}.hh | sed -e "s/${ORIG_CLASS}/${DEST_CLASS}/g" >> ${DEST_PATH}/${DEST_CLASS}.hh
echo "${ORIG_PATH}/${ORIG_CLASS}.cc -> ${DEST_PATH}/${DEST_CLASS}.cc"
cat ${ORIG_PATH}/${ORIG_CLASS}.cc | sed -e "s/${ORIG_CLASS}/${DEST_CLASS}/g" >> ${DEST_PATH}/${DEST_CLASS}.cc
