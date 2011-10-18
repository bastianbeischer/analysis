#!/bin/bash

FROM=0
UNTIL=9999999999

if [ $# -eq 1 ]
then
  FROM=$(echo $1 | sed "s/[^0-9]//g" | egrep "^..........$")
fi

if [ $# -eq 2 ]
then
  FROM=$(echo $1 | sed "s/[^0-9]//g" | egrep "^..........$")
  UNTIL=$(echo $2 | sed "s/[^0-9]//g" | egrep "^..........$")
fi

echo "Parsing data between ${FROM} ($(date -d @${FROM})) and ${UNTIL} ($(date -d @${UNTIL}))."

FILES=$(find ${PERDAIXDATA_PATH} -name "run_*.dat" | sort)

EVENTCOUNTEREXECUTABLE=$(which eventcounter)
NUMBEROFFILES=0
for FILE in ${FILES}
do
  FILEBASE=${FILE:${#str}-18:14}
  UNIXTIME=${FILEBASE:4:10}
  if [ ${FROM} -le ${UNIXTIME} ] && [ ${UNIXTIME} -le ${UNTIL} ]
  then
    let NUMBEROFFILES=${NUMBEROFFILES}+1
    DATE=$(date -d @${UNIXTIME} +"%a %d %b %R")
    SIZE=$(du -h ${FILE} | sed "s/\t.*$//gi")
    if [ -n "${EVENTCOUNTEREXECUTABLE}" ]; then
      STATS=$(eventcounter $FILE 2> /dev/null)
      EVENTS=$(echo ${STATS} | sed "s/\([0-9]*\) particle events .*$/\1/g")
      let SUM=${SUM}+${EVENTS}
      echo "${DATE}   ${FILEBASE} ${STATS} ${SIZE}"
    else
      echo "${DATE}   ${FILEBASE} ${SIZE}"
    fi
  fi
done

if [ -n "${EVENTCOUNTEREXECUTABLE}" ]; then
  echo "${NUMBEROFFILES} files, ${SUM} particle events."
else
  echo "${NUMBEROFFILES} files."
fi
