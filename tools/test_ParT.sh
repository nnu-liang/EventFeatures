if [ "$#" -ne 4 ]; then
    echo "用法: $0 <dR> <eta> <delphes.root的路径> <label>"
    exit 1
fi

dR=$1
eta=$2
delphes_root_path=$3
label=$4

config_file="config_file"

sed -i "s/^FAT_JET_ANTIKT_dR = .*/FAT_JET_ANTIKT_dR = $dR/" $config_file
sed -i "s/^FAT_JET_ETA_MAX = .*/FAT_JET_ETA_MAX = $eta/" $config_file

./Root2ParT.X $label $config_file output.root $delphes_root_path

weaver --predict \
       --data-test 'output.root' \
       --data-config /home/liang/Workingspace/PartTransformer/particle_transformer-main/data/JetClass/JetClass_full.yaml \
       --network-config /home/liang/Workingspace/PartTransformer/particle_transformer-main/networks/example_ParticleTransformer.py \
       --model-prefix /home/liang/Workingspace/PartTransformer/particle_transformer-main/models/ParT_full.pt \
       --gpus 0 \
       --batch-size 256 \
       --predict-output /home/liang/Workingspace/PartTransformer/particle_transformer-main/prediction.root

./cout_class.X /home/liang/Workingspace/PartTransformer/particle_transformer-main/prediction.root

