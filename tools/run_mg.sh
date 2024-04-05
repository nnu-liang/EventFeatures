if [ "$#" -ne 8 ]; then
    echo "Usage: $0 <process> <output_dir> <process_name> <launch_times> <root_file_dir> <madspin> <madspin_card_path> <dummy_fct_path>"
    exit 1
fi

MG_DIR="/home/liang/Workingspace/MC2/MG5_aMC_v3_5_3"
PROCESS="$1"
OUTPUT_DIR="$2"
PROCESS_NAME="$3"
N="$4"
ROOT_FILE_DIR="$5"
MADSPIN="$6"
MADSPIN_CARD_PATH="$7"
DUMMY_FCT_PATH="$8"
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
cp "$DUMMY_FCT_PATH" $PROCESS_DIR/SubProcesses/
cd $MG_DIR

ROOT_FILE_NUM=1

for ((i=1; i<=$N; i++))
do
    if [ $i -eq 1 ]; then
        if [ "$MADSPIN" -eq 1 ]; then
            ./bin/mg5_aMC<<EOF
launch $PROCESS_DIR
1
2
4
done
$MADSPIN_CARD_PATH
$DELPHES_CARD_PATH
$RUN_CARD_PATH
exit
EOF
        else
            ./bin/mg5_aMC<<EOF
launch $PROCESS_DIR
1
2
done
$DELPHES_CARD_PATH
$RUN_CARD_PATH
exit
EOF
        fi
    else
        if [ "$MADSPIN" -eq 1 ]; then
            ./bin/mg5_aMC<<EOF
launch $PROCESS_DIR
done
$MADSPIN_CARD_PATH
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
    fi

    if [ "$MADSPIN" -eq 1 ]; then
        RUN_DIR="${PROCESS_DIR}/Events/run_01_decayed_1/"
    else
        RUN_DIR="${PROCESS_DIR}/Events/run_01/"
    fi
    if [ -d "$RUN_DIR" ]; then
        cd $RUN_DIR
        if [ -f "tag_1_delphes_events.root" ]; then
            mv tag_1_delphes_events.root "${ROOT_FILE_DIR}/${PROCESS_NAME}_$(printf "%02d" $ROOT_FILE_NUM)_${TIMESTAMP}.root"
        fi
        cd -  
        rm -f "${PROCESS_DIR}/HTML/results.pkl"
        find "${PROCESS_DIR}/HTML/" -type d -name "*run*" -exec rm -rf {} +
        find "${PROCESS_DIR}/Events/" -mindepth 1 -type d -exec rm -rf {} +
    fi
    let ROOT_FILE_NUM+=1
done
rm -rf "${PROCESS_DIR}"
echo "完成：数据已在 $PROCESS_DIR 中生成，tag_1_delphes_events.root 文件已移至 $ROOT_FILE_DIR"

