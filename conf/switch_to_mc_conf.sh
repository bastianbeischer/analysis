echo "switching config files to Monte Carlo configuration"
rm perdaix_coordinates.conf setup.conf TRDCorrections.conf
mv mc_coordinates.conf perdaix_coordinates.conf
mv mc_setup.conf setup.conf
mv mc_TRDCorrections.conf TRDCorrections.conf  
echo "done"
