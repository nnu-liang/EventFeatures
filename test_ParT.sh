if [ "$#" -ne 4 ]; then
    echo "用法: $0 <dR> <eta> <delphes文件夹路径> <label>"
    exit 1
fi

dR=$1
eta=$2
delphes_folder_path=$3
label=$4

original_config_file="/home/liang/Workingspace/FatJet_config/FatJet_config.conf"
new_config_file="/home/liang/Workingspace/FatJet_config/FatJet_config_dR=${dR}eta=${eta}.conf"
cp "$original_config_file" "$new_config_file"
sed -i "s/^FAT_JET_ANTIKT_dR = .*/FAT_JET_ANTIKT_dR = $dR/" "$new_config_file"
sed -i "s/^FAT_JET_ETA_MAX = .*/FAT_JET_ETA_MAX = $eta/" "$new_config_file"
output_root_file="/home/liang/Workingspace/Output_root/output_dR=${dR}eta=${eta}.root"
predict_output_file="/home/liang/Workingspace/Tagging/prediction_dR=${dR}eta=${eta}.root"
input_files_str="${delphes_folder_path}*.root"

mkdir -p "/home/liang/Workingspace/Output_root/"


./Root2ParTFormat.x $label $new_config_file $output_root_file $input_files_str


weaver --predict \
       --data-test $output_root_file \
       --data-config /home/liang/Workingspace/PartTransformer/particle_transformer-main/data/JetClass/JetClass_full.yaml \
       --network-config /home/liang/Workingspace/PartTransformer/particle_transformer-main/networks/example_ParticleTransformer.py \
       --model-prefix /home/liang/Workingspace/PartTransformer/particle_transformer-main/models/ParT_full.pt \
       --gpus 0 \
       --batch-size 64 \
       --predict-output $predict_output_file


./cout_class.x $predict_output_file


