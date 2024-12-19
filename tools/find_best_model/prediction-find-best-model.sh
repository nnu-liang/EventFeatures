#!/bin/bash

WEAVER_DIR="/home/lxiao/weaver-main"
data_config="${WEAVER_DIR}/test/JetClass_full3.yaml"
network_config="${WEAVER_DIR}/test/example_ParticleTransformer.py"
models_dir="${WEAVER_DIR}/models"

start_epoch=160
end_epoch=200

cd $WEAVER_DIR

for epoch in $(seq $start_epoch $end_epoch); do
    model_prefix="${models_dir}/train14_epoch-${epoch}_state.pt"
    output_file="/data/lxiao/output_root/find-best-model-output-root-all/train_epoch-${epoch}.root"

    if [ -f "$model_prefix" ]; then
        echo "Using model: $model_prefix"
        echo "Output file: $output_file"

        python3 weaver/train.py \
            --predict \
            --data-test \
            "/data/lxiao/Sig_Bkg_EvenT/val/*EvenTF/*.root" \
            "/data/lxiao/Sig_Bkg_EvenT/val-add270/*EvenTF/*.root" \
            --data-config $data_config \
            --network-config $network_config \
            --model-prefix $model_prefix \
            --batch-size 512 \
            --gpus 0,1 \
            --predict-output $output_file
    else
        echo "Model file $model_prefix does not exist, skipping..."
    fi
done

