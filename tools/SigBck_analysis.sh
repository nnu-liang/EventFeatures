if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <path-to-input-root-files>"
    exit 1
fi

CONFIG_FILE="FatJet_config.conf"
DATA_CONFIG="/home/liang/Workingspace/PartTransformer/particle_transformer-main/data/JetClass/JetClass_full.yaml"
NETWORK_CONFIG="/home/liang/Workingspace/PartTransformer/particle_transformer-main/networks/example_ParticleTransformer.py"
MODEL_PREFIX="/home/liang/Workingspace/PartTransformer/particle_transformer-main/models/ParT_full.pt"
PREDICTION_OUTPUT="/home/liang/Workingspace/input_file/output_prediction.root"
INPUT_DIR=$1
CONFIG_FILE="/home/liang/Workingspace/Tagging/FatJet_config.conf"
LABEL="1"  
OUTPUT_FILE="/home/liang/Workingspace/input_file/output.root"

input_files=()
for file in "$INPUT_DIR"/*.root; do
    input_files+=("$file")
done

./Root2ParTFormat.x "$LABEL" "$CONFIG_FILE" "$OUTPUT_FILE" "${input_files[@]}"

weaver --predict \
       --data-config "$DATA_CONFIG" \
       --data-test "$OUTPUT_FILE" \
       --network-config "$NETWORK_CONFIG" \
       --model-prefix "$MODEL_PREFIX" \
       --predict-output "$PREDICTION_OUTPUT" \
       --batch-size 32 \
       --gpus 0
       
./SigBkgAnalysis "$OUTPUT_FILE" "$PREDICTION_OUTPUT"
