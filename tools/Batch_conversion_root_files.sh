LABEL="12"
CONFIG_FILE="/home/lxiao/Tagging/inputs/FatJet_config.conf"

INPUT_DIR="/data/lxiao/Our_train_datasets/Zbb_train"

OUTPUT_DIR="/data/lxiao/Our_train_datasets/Zbb_train_ParTF"

mkdir -p "$OUTPUT_DIR"

for FILE in "$INPUT_DIR"/*.root; do
    FILENAME=$(basename -- "$FILE")
    OUTPUT_FILE="$OUTPUT_DIR/${FILENAME%.root}_ParTF.root"
    ./Root2ParTFormat.x "$LABEL" "$CONFIG_FILE" "$OUTPUT_FILE" "$FILE"
done

echo "所有文件处理完成。"

