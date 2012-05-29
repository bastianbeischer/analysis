#!/bin/bash
HEADER_ENDING=h
SOURCE_ENDING=cpp
ORIG_CLASS=$(basename $1 | sed -e "s/\.$//g" | sed -e "s/\.${SOURCE_ENDING}$//g" | sed -e "s/\.${HEADER_ENDING}$//g")
ORIG_PATH=$(dirname $1)
DEST_CLASS=$(basename $2 | sed -e "s/\.$//g" | sed -e "s/\.${SOURCE_ENDING}$//g" | sed -e "s/\.${HEADER_ENDING}$//g")
DEST_PATH=$(dirname $2)
echo "${ORIG_PATH}/${ORIG_CLASS}.${HEADER_ENDING} -> ${DEST_PATH}/${DEST_CLASS}.${HEADER_ENDING}"
cat ${ORIG_PATH}/${ORIG_CLASS}.${HEADER_ENDING} | sed -e "s/${ORIG_CLASS}/${DEST_CLASS}/g" >> ${DEST_PATH}/${DEST_CLASS}.${HEADER_ENDING}
echo "${ORIG_PATH}/${ORIG_CLASS}.${SOURCE_ENDING} -> ${DEST_PATH}/${DEST_CLASS}.${SOURCE_ENDING}"
cat ${ORIG_PATH}/${ORIG_CLASS}.${SOURCE_ENDING} | sed -e "s/${ORIG_CLASS}/${DEST_CLASS}/g" >> ${DEST_PATH}/${DEST_CLASS}.${SOURCE_ENDING}
