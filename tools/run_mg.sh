MG_DIR="/home/liang/Workingspace/MC2/MG5_aMC_v3_5_3"  
echo "输入想要产生的过程："
read PROCESS
echo "请输入数据存放路径："
read OUTPUT_DIR
echo "请输入想要保存过程的名称："
read PROCESS_NAME
echo "请输入想要launch的次数："
read N
echo "请输入 delphes_card 路径："
read DELPHES_CARD_PATH
echo "请输入 run_card 路径："
read RUN_CARD_PATH
echo "请输入存放tag_1_delphes_events.root的路径："
read ROOT_FILE_DIR


TIMESTAMP=$(date +%Y%m%d_%H%M%S)
PROCESS_DIR="${OUTPUT_DIR}/${PROCESS_NAME}_${TIMESTAMP}"

if [ -d "$PROCESS_DIR" ]; then
    SUFFIX=1
    while [ -d "${PROCESS_DIR}_${SUFFIX}" ]; do
        let SUFFIX+=1
    done
    PROCESS_DIR="${PROCESS_DIR}_${SUFFIX}"
fi

mkdir -p "$PROCESS_DIR"
mkdir -p "$ROOT_FILE_DIR"

cd $MG_DIR
./bin/mg5_aMC<<EOF
generate $PROCESS
output $PROCESS_DIR
exit
EOF
cp /home/liang/Workingspace/PartTransformer/Tagging-main7/Tagging-main/inputs/trackResolutionCMS.tcl $PROCESS_DIR/Cards/
cd $MG_DIR
for ((i=1; i<=$N; i++))
do
    FORMATTED_INDEX=$(printf "%02d" $i)
    if [ $i -eq 1 ]; then
        ./bin/mg5_aMC<<EOF
launch $PROCESS_DIR
1
2
3
done
$DELPHES_CARD_PATH
$RUN_CARD_PATH
exit
EOF
    else
        ./bin/mg5_aMC<<EOF
launch $PROCESS_DIR
done
$DELPHES_CARD_PATH
$RUN_CARD_PATH
exit
EOF
    fi
    RUN_DIR="$PROCESS_DIR/Events/run_$FORMATTED_INDEX/"
    cd $RUN_DIR
    find . -type f -not -name "tag_1_delphes_events.root" -exec rm {} +
    mv tag_1_delphes_events.root "${ROOT_FILE_DIR}/tag_1_delphes_events_${FORMATTED_INDEX}.root"
    cd -  
done
echo "完成：数据已在 $PROCESS_DIR 中生成，tag_1_delphes_events.root 文件已移至 $ROOT_FILE_DIR"

