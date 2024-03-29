
MG_DIR="/home/liang/Workingspace/MC/MG5_aMC_v3_4_2"  # MadGraph 安装目录
echo "输入想要产生的过程："
read PROCESS
echo "请输入数据存放路径："
read OUTPUT_DIR
echo "请输入想要保存过程的名称"
read PROCESS_NAME
echo "请输入想要launch的次数"
read N
echo "请输入 delphes_card 路径："
read DELPHES_CARD_PATH
echo "请输入 run_card 路径："
read RUN_CARD_PATH
PROCESS_DIR="$OUTPUT_DIR/$PROCESS_NAME"   # 过程目录路径

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
    if [ $i -eq 1 ]; then
        ./bin/mg5_aMC<<EOF
launch $PROCESS_DIR
1
2
3
done
$DELPHES_CARD_PATH
$RUN_CARD_PATH
EOF
    else
        ./bin/mg5_aMC<<EOF
launch $PROCESS_DIR
done
$DELPHES_CARD_PATH
$RUN_CARD_PATH
EOF
    fi
done

echo "完成：数据已在 $PROCESS_DIR 中生成"

