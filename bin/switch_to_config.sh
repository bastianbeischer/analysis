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
ls -alh --color=auto ${PERDAIXANA_PATH}/conf/*.conf
echo "done"
