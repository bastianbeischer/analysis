#!/bin/bash

HH=$(egrep -R " $" * | grep "hh:" | sed -e "s/\.hh:.*$/.hh/g")
CC=$(egrep -R " $" * | grep "cc:" | sed -e "s/\.cc:.*$/.cc/g")
FILES=$(echo ${HH} && echo ${CC})

for FILE in ${FILES}; do
  echo $FILE
  cat ${FILE} | sed -e "s/ *$//g" > ${FILE}.tmpRemoveSpaces
  mv ${FILE}.tmpRemoveSpaces ${FILE}
done
