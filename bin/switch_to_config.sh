if [ $# -ne 1 ]; then
    echo "please give the name of the configuration to switch to!"
    exit -1;
fi

echo "switching to ${1} config"
DIR=${PERDAIXANA_PATH}/conf
cd $DIR
for i in $(ls $1/*); do
    ln -sf ${i} .;
done

PLATFORM=$(uname -s )
if [ "$PLATFORM" == "Linux" ]; then
  LSCOLORFLAG="--color=auto"
elif [ "$PLATFORM" == "Darwin" ]; then
  LSCOLORFLAG="-G"
else
  echo "Script not optimized for your environement. Maybe it works anyway."
fi
ls -alh $LSCOLORFLAG ${PERDAIXANA_PATH}/conf/*.conf
echo "done"
