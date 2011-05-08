echo "switching config files to testbeam configuration"
DIR=${PERDAIXANA_PATH}/conf
cp -f ${DIR}/testbeam_setup.conf ${DIR}/setup.conf
#cp -f ${DIR}/testbeam_TRDCorrections.conf ${DIR}/TRDCorrections.conf  
echo "done"
