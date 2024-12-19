#!/bin/bash

PROGRAM_PATH="/home/lxiao/weaver-main/analysis_code/calculate-error-by-outputroot"  
ROOT_DIR="/data/lxiao/output_root/find-best-model-output-root/"                
OUTPUT_FILE="/data/lxiao/output_root/find-best-model-output-root/relative_errors.txt"     

echo "" > $OUTPUT_FILE

epochs=()
relativeError95s=()
relativeError68s=()

for root_file in ${ROOT_DIR}train_epoch_*.root; do
    if [ -f "$root_file" ]; then
        epoch=$(basename "$root_file" | grep -oP '(?<=train_epoch-)\d+(?=\.root)')
        
        result=$($PROGRAM_PATH "$root_file")
        relativeError95=$(echo "$result" | grep "95% CL Relative error" | awk '{print $NF}')
        relativeError68=$(echo "$result" | grep "68% CL Relative error" | awk '{print $NF}')
        
        epochs+=("$epoch")
        relativeError95s+=("$relativeError95")
        relativeError68s+=("$relativeError68")
    fi
done

echo "Epochs: ${epochs[*]}" >> $OUTPUT_FILE
echo "RelativeError95: ${relativeError95s[*]}" >> $OUTPUT_FILE
echo "RelativeError68: ${relativeError68s[*]}" >> $OUTPUT_FILE

echo "Results saved to $OUTPUT_FILE"

