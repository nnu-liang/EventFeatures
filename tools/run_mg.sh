if [ "$#" -ne 5 ]; then
    echo "Usage: $0 <process> <output_dir> <process_name> <launch_times> <root_file_dir>"
    exit 1
fi

MG_DIR="/home/liang/Workingspace/MC2/MG5_aMC_v3_5_3"
PROCESS="$1"
OUTPUT_DIR="$2"
PROCESS_NAME="$3"
N="$4"
ROOT_FILE_DIR="$5"
DELPHES_CARD_PATH="/home/liang/Workingspace/Tagging/inputs/delphes_card_CMS.dat"
RUN_CARD_PATH="/home/liang/Workingspace/Tagging/inputs/run_card.dat"

if [ ! -d "$ROOT_FILE_DIR" ]; then
    mkdir -p "$ROOT_FILE_DIR"
fi

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

cd $MG_DIR
./bin/mg5_aMC<<EOF
generate $PROCESS
output $PROCESS_DIR
exit
EOF
cp /home/liang/Workingspace/Tagging/inputs/trackResolutionCMS.tcl $PROCESS_DIR/Cards/
cd $MG_DIR

ROOT_FILE_NUM=1
if ls ${ROOT_FILE_DIR}/${PROCESS_NAME}_*.root 1> /dev/null 2>&1; then
    MAX_NUM=$(ls ${ROOT_FILE_DIR}/${PROCESS_NAME}_*.root | awk -F '_' '{print $NF}' | sort -n | tail -1 | sed 's/.root$//')
    if [[ "$MAX_NUM" =~ ^[0-9]+$ ]]; then
        ROOT_FILE_NUM=$((MAX_NUM + 1))
    fi
fi

for ((i=1; i<=$N; i++))
do
    if [ $i -eq 1 ]; then
        ./bin/mg5_aMC<<EOF
launch $PROCESS_DIR
1
2
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
    RUN_DIR="$PROCESS_DIR/Events/run_$(printf "%02d" $i)/"
if [ -d "$RUN_DIR" ]; then
    cd $RUN_DIR
    if [ -f "tag_1_delphes_events.root" ]; then
        mv tag_1_delphes_events.root "${ROOT_FILE_DIR}/${PROCESS_NAME}_$(printf "%02d" $ROOT_FILE_NUM).root"
        let ROOT_FILE_NUM+=1
    fi
    cd -  
fi
done
rm -rf "${PROCESS_DIR}/Events"
rm -f "${PROCESS_DIR}/HTML/results.pkl"
find "${PROCESS_DIR}/HTML/" -type d -name "*run*" -exec rm -rf {} +
echo "完成：数据已在 $PROCESS_DIR 中生成，tag_1_delphes_events.root 文件已移至 $ROOT_FILE_DIR"

