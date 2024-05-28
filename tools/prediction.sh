#!/bin/bash
if [ "$#" -ne 2 ]; then
    echo "用法: $0 <data_test_file> <predict_output_file>"
    exit 1
fi

DATA_TEST=$1
PREDICT_OUTPUT=$2

# 运行 weaver 命令
weaver --predict \
    --data-config '/home/lxiao/Workingspace/PartTransformer/particle_transformer-main/data/JetClass/JetClass_full.yaml' \
    --data-test "${DATA_TEST}" \
    --network-config '/home/lxiao/Workingspace/PartTransformer/particle_transformer-main/networks/example_ParticleTransformer.py' \
    --model-prefix '/home/lxiao/Workingspace/PartTransformer/particle_transformer-main/models/ParT_full.pt' \
    --predict-output "${PREDICT_OUTPUT}" \
    --batch-size 256 \
    --gpus 0

