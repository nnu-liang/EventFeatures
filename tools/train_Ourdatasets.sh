start_lr=$1
num_epochs=$2
log_name=$3
tensorboard_run_name=$4
model_prefix_name=$5

data_config="/home/lxiao/Workingspace/PartTransformer/particle_transformer-main/data/JetClass/JetClass_full.yaml"
network_config="/home/lxiao/Workingspace/PartTransformer/particle_transformer-main/networks/example_ParticleTransformer.py"
model_prefix="/home/lxiao/Workingspace/PartTransformer/particle_transformer-main/models/${model_prefix_name}.pt"
model_weights="/home/lxiao/Workingspace/PartTransformer/particle_transformer-main/models/ParT_full.pt"
log_path="/home/lxiao/Workingspace/PartTransformer/particle_transformer-main/logs/${log_name}.log"
tensorboard_path="/home/lxiao/Workingspace/PartTransformer/particle_transformer-main/runs/${tensorboard_run_name}"

weaver --data-train \
    "HToBB:/data/lxiao/JetClass_add_label_Zbb/train_25/HToBB*.root" \
    "TTBar:/data/lxiao/JetClass_add_label_Zbb/train_25/TTBar_*.root" \
    "TTBarLep:/data/lxiao/JetClass_add_label_Zbb/train_25/TTBarLep_*.root" \
    "ZJetsToNuNu:/data/lxiao/JetClass_add_label_Zbb/train_25/ZJetsToNuNu_*.root" \
    "Zbb:/data/lxiao/Our_train_datasets/Zbb_train_ParTF/Zbb*.root"\
    --data-val \
    "HToBB:/data/lxiao/JetClass_add_label_Zbb/val_25/HToBB*.root" \
    "TTBar:/data/lxiao/JetClass_add_label_Zbb/val_25/TTBar_*.root" \
    "TTBarLep:/data/lxiao/JetClass_add_label_Zbb/val_25/TTBarLep_*.root" \
    "ZJetsToNuNu:/data/lxiao/JetClass_add_label_Zbb/val_25/ZJetsToNuNu_*.root" \
    "Zbb:/data/lxiao/Our_train_datasets/Zbb_val_ParTF/Zbb*.root"\
    --data-test \
    "HToBB:/data/lxiao/JetClass_add_label_Zbb/test_25/HToBB_*.root" \
    "TTBar:/data/lxiao/JetClass_add_label_Zbb/test_25/TTBar_*.root" \
    "TTBarLep:/data/lxiao/JetClass_add_label_Zbb/test_25/TTBarLep_*.root" \
    "ZJetsToNuNu:/data/lxiao/JetClass_add_label_Zbb/test_25/ZJetsToNuNu_*.root" \
    "Zbb:/data/lxiao/Our_train_datasets/Zbb_test_ParTF/Zbb*.root"\
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

