LABEL="12"
CONFIG_FILE="/home/lxiao/Tagging/inputs/FatJet_config.conf"

INPUT_DIR="/data/lxiao/Our_train_datasets/Zbb_train"

OUTPUT_DIR="/data/lxiao/Our_train_datasets/Zbb_train_ParTF"

mkdir -p "$OUTPUT_DIR"

max_jobs=30

for FILE in "$INPUT_DIR"/*.root; do
    FILENAME=$(basename -- "$FILE")
    OUTPUT_FILE="$OUTPUT_DIR/${FILENAME%.root}_ParTF.root"
    ./Root2ParTFormat.x "$LABEL" "$CONFIG_FILE" "$OUTPUT_FILE" "$FILE" &
    (( $(jobs -r | wc -l) >= max_jobs )) && wait -n
done
wait
echo "所有文件处理完成。"

