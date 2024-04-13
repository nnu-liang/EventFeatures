start_lr=$1
num_epochs=$2
log_name=$3
tensorboard_run_name=$4
model_prefix_name=$5

DATA_DIR_TRAIN="/data/lxiao/JetClass_add_label_Zbb/train_25"
DATA_DIR_VAL="/data/lxiao/JetClass_add_label_Zbb/val_25"
DATA_DIR_TEST="/data/lxiao/JetClass_add_label_Zbb/test_25"
DATA_DIR_OURDATASETS="/data/lxiao/Our_train_datasets"
Tagging_DIR="/home/lxiao/Tagging"

data_config="${Tagging_DIR}/data_config/JetClass_full.yaml"
network_config="${Tagging_DIR}/networks/example_ParticleTransformer.py"
model_prefix="${Tagging_DIR}/models/${model_prefix_name}.pt"
model_weights="${Tagging_DIR}/models/ParT_full.pt"
log_path="${Tagging_DIR}/logs/${log_name}.log"
tensorboard_path="${Tagging_DIR}/runs/${tensorboard_run_name}"

weaver --data-train \
    "HToBB:${DATA_DIR_TRAIN}/HToBB*.root" \
    "TTBar:${DATA_DIR_TRAIN}/TTBar_*.root" \
    "TTBarLep:${DATA_DIR_TRAIN}/TTBarLep_*.root" \
    "ZJetsToNuNu:${DATA_DIR_TRAIN}/ZJetsToNuNu_*.root" \
    "Zbb:${DATA_DIR_OURDATASETS}/Zbb_train_ParTF/Zbb*.root"\
    --data-val \
    "HToBB:${DATA_DIR_VAL}/HToBB*.root" \
    "TTBar:${DATA_DIR_VAL}/TTBar_*.root" \
    "TTBarLep:${DATA_DIR_VAL}/TTBarLep_*.root" \
    "ZJetsToNuNu:${DATA_DIR_VAL}/ZJetsToNuNu_*.root" \
    "Zbb:${DATA_DIR_OURDATASETS}/Zbb_val_ParTF/Zbb*.root"\
    --data-test \
    "HToBB:${DATA_DIR_TEST}/HToBB_*.root" \
    "TTBar:${DATA_DIR_TEST}/TTBar_*.root" \
    "TTBarLep:${DATA_DIR_TEST}/TTBarLep_*.root" \
    "ZJetsToNuNu:${DATA_DIR_TEST}/ZJetsToNuNu_*.root" \
    "Zbb:${DATA_DIR_OURDATASETS}/Zbb_test_ParTF/Zbb*.root"\
    --data-config $data_config \
    --network-config $network_config \
    --model-prefix $model_prefix \
    --gpus 0 \
    --batch-size 512 \
    --start-lr $start_lr \
    --num-epochs $num_epochs \
    --optimizer ranger \
    --log $log_path \
    --load-model-weights $model_weights \
    --tensorboard $tensorboard_path \
    --exclude-model-weights 'mod.fc.0.weight,mod.fc.0.bias'\

echo "Training started with log at $log_path and TensorBoard at $tensorboard_path"

