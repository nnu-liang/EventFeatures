#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <config_file>"
    exit 1
fi

CONFIG_FILE=$1
if [ ! -f "$CONFIG_FILE" ];then
    echo "error: no $CONFIG_FILE"
    exit 1
fi

BACKGROUND_DIR=$(grep '^background_dir=' "$CONFIG_FILE" | cut -d'=' -f2)
SIGNAL_SM_DIR=$(grep '^signal_SM_dir=' "$CONFIG_FILE" | cut -d'=' -f2)
SIGNAL_ENTRIES=($(grep '^signal_dir[0-9]*=' "$CONFIG_FILE"))

BACKGROUND_OUTPUT_DIR=$(grep '^background_output_dir=' "$CONFIG_FILE" | cut -d'=' -f2)
SIGNAL_OUTPUT_DIR=$(grep '^signal_output_dir=' "$CONFIG_FILE" | cut -d'=' -f2)

if [ ! -d "$BACKGROUND_DIR" ]; then
    echo "error: no $BACKGROUND_DIR"
    exit 1
fi

if [ ! -d "$SIGNAL_SM_DIR" ]; then
    echo "error: no $SIGNAL_SM_DIR"
    exit 1
fi

for ENTRY in "${SIGNAL_ENTRIES[@]}"; do
    SIGNAL_DIR=$(echo "$ENTRY" | cut -d' ' -f1 | cut -d'=' -f2)
    if [ ! -d "$SIGNAL_DIR" ]; then
        echo "error: no $SIGNAL_DIR"
        exit 1
    fi
done

mkdir -p "$BACKGROUND_OUTPUT_DIR"
mkdir -p "$SIGNAL_OUTPUT_DIR"

total_B=(0 0 0 0 0 0)
total_SM=(0 0 0 0 0 0)
total_S=()
total_chi_squared=()
kappa_lambda_values=()

for (( i=0; i<${#SIGNAL_ENTRIES[@]}; i++ )); do
    total_S[i]=(0 0 0 0 0 0)
    total_chi_squared[i]=0
    kappa_lambda_values[i]=$(echo "${SIGNAL_ENTRIES[i]}" | awk '{print $2}')
done

process_file() {
    local INPUT_ROOT_FILE=$1
    local OUTPUT_ROOT_FILE=$2
    local TMP_FILE=$3
    local SIG_TXT_FILE=$4

    ./prediction.sh "$INPUT_ROOT_FILE" "$OUTPUT_ROOT_FILE"

    if [ ! -f "$OUTPUT_ROOT_FILE" ]; then
        echo "error: no $OUTPUT_ROOT_FILE"
        exit 1
    fi

    ./SigBkg_analysis.x "$INPUT_ROOT_FILE" "$OUTPUT_ROOT_FILE" > "$SIG_TXT_FILE"

    if [ ! -f "$SIG_TXT_FILE" ]; then
        echo "error: no $SIG_TXT_FILE "
        exit 1
    fi

    local X=(0 0 0 0 0 0)

    while IFS= read -r line; do
        fields=($line)
        sum_jet_mass=0

        for ((i=1; i<${#fields[@]}; i++)); do
            sum_jet_mass=$(echo "$sum_jet_mass + ${fields[$i]}" | bc)
        done

        if (( $(echo "$sum_jet_mass >= 200 && $sum_jet_mass < 250" | bc -l) )); then
            ((X[0]++))
        elif (( $(echo "$sum_jet_mass >= 250 && $sum_jet_mass < 300" | bc -l) )); then
            ((X[1]++))
        elif (( $(echo "$sum_jet_mass >= 300 && $sum_jet_mass < 350" | bc -l) )); then
            ((X[2]++))
        elif (( $(echo "$sum_jet_mass >= 350 && $sum_jet_mass < 400" | bc -l) )); then
            ((X[3]++))
        elif (( $(echo "$sum_jet_mass >= 400 && $sum_jet_mass < 500" | bc -l) )); then
            ((X[4]++))
        elif (( $(echo "$sum_jet_mass >= 500" | bc -l) )); then
            ((X[5]++))
        fi
    done < "$SIG_TXT_FILE"

    echo "${X[@]}" > "$TMP_FILE"
}

export -f process_file

find "$BACKGROUND_DIR" -name '*.root' | xargs -I {} -P 4 bash -c '
    TMP_FILE=$(mktemp)
    SIG_TXT_FILE=$(mktemp)
    process_file "{}" "$BACKGROUND_OUTPUT_DIR/$(basename {})" "$TMP_FILE" "$SIG_TXT_FILE"
    cat "$TMP_FILE"
    rm "$TMP_FILE" "$SIG_TXT_FILE"
' | while read TMP_FILE; do
    read -r X1 X2 X3 X4 X5 X6 <<< "$TMP_FILE"
    total_B[0]=$((total_B[0] + X1))
    total_B[1]=$((total_B[1] + X2))
    total_B[2]=$((total_B[2] + X3))
    total_B[3]=$((total_B[3] + X4))
    total_B[4]=$((total_B[4] + X5))
    total_B[5]=$((total_B[5] + X6))
done

find "$SIGNAL_SM_DIR" -name '*.root' | xargs -I {} -P 4 bash -c '
    TMP_FILE=$(mktemp)
    SIG_TXT_FILE=$(mktemp)
    process_file "{}" "$SIGNAL_OUTPUT_DIR/$(basename {})" "$TMP_FILE" "$SIG_TXT_FILE"
    cat "$TMP_FILE"
    rm "$TMP_FILE" "$SIG_TXT_FILE"
' | while read TMP_FILE; do
    read -r X1 X2 X3 X4 X5 X6 <<< "$TMP_FILE"
    total_SM[0]=$((total_SM[0] + X1))
    total_SM[1]=$((total_SM[1] + X2))
    total_SM[2]=$((total_SM[2] + X3))
    total_SM[3]=$((total_SM[3] + X4))
    total_SM[4]=$((total_SM[4] + X5))
    total_SM[5]=$((total_SM[5] + X6))
done

for (( i=0; i<${#SIGNAL_ENTRIES[@]}; i++ )); do
    SIGNAL_DIR=$(echo "${SIGNAL_ENTRIES[i]}" | cut -d' ' -f1 | cut -d'=' -f2)
    find "$SIGNAL_DIR" -name '*.root' | xargs -I {} -P 4 bash -c '
        TMP_FILE=$(mktemp)
        SIG_TXT_FILE=$(mktemp)
        process_file "{}" "$SIGNAL_OUTPUT_DIR/$(basename {})" "$TMP_FILE" "$SIG_TXT_FILE"
        echo "$i $(cat $TMP_FILE)"
        rm "$TMP_FILE" "$SIG_TXT_FILE"
    ' _ {} | while read TMP_FILE; do
        read -r IDX X1 X2 X3 X4 X5 X6 <<< "$TMP_FILE"
        total_S[IDX][0]=$((total_S[IDX][0] + X1))
        total_S[IDX][1]=$((total_S[IDX][1] + X2))
        total_S[IDX][2]=$((total_S[IDX][2] + X3))
        total_S[IDX][3]=$((total_S[IDX][3] + X4))
        total_S[IDX][4]=$((total_S[IDX][4] + X5))
        total_S[IDX][5]=$((total_S[IDX][5] + X6))
    done
done

zeta_b=0.1

for (( i=0; i<${#SIGNAL_ENTRIES[@]}; i++ )); do
    for j in {0..5}; do
        S=${total_S[i][j]}
        SM=${total_SM[j]}
        B=${total_B[j]}
        total_chi_squared[i]=$(echo "${total_chi_squared[i]} + (($S - $SM)^2) / ($B + ($zeta_b * $B)^2)" | bc -l)
    done
done

python3 plot_chi_square_kappa_lambda.py "${total_chi_squared[@]}" "${kappa_lambda_values[@]}"


echo "over"

