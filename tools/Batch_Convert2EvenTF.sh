#!/bin/bash

CONFIG_FILE="/home/lxiao/EventFeatures-main/inputs/EventJet_config.conf"

CONFIG_PATH="$1"

mkdir -p "$OUTPUT_DIR"
max_jobs=60

while IFS=': ' read -r LABEL PATHS; do
    INPUT_DIR=$(echo "$PATHS" | awk '{print $1}')
    OUTPUT_DIR=$(echo "$PATHS" | awk '{print $2}')

    mkdir -p "$OUTPUT_DIR"

    for FILE in "$INPUT_DIR"/*.root; do
        FILENAME=$(basename -- "$FILE")
        OUTPUT_FILE="$OUTPUT_DIR/${FILENAME%.root}_EvenTF.root"
        /home/lxiao/EventFeatures-main/Root2EvenTFormat.x "$LABEL" "$CONFIG_FILE" "$OUTPUT_FILE" "$FILE"&
        (( $(jobs -r | wc -l) >= max_jobs )) && wait -n
    done
done < "$CONFIG_PATH"
wait

echo "over"

