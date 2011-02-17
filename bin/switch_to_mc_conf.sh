echo "switching config files to mc configuration"
DIR=${PERDAIXANA_PATH}/conf
cp -f ${DIR}/mc_coordinates.conf ${DIR}/perdaix_coordinates.conf
cp -f ${DIR}/mc_setup.conf ${DIR}/setup.conf
cp -f ${DIR}/mc_TRDCorrections.conf ${DIR}/TRDCorrections.conf  
echo "done"
