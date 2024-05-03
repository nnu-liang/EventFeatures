source_dir="/home/liang/Workingspace/Tagging/tools/input"
target_dir="/home/liang/Workingspace/Tagging/tools/output"

converter="/home/liang/Workingspace/Tagging/tools/Convert2Event-Jet_2.x"

max_jobs=30

process_file() {
    local file="$1"
    local filename=$(basename -- "$file")
    local filename_noext="${filename%.*}"
    local output_filename="${target_dir}/${filename_noext}_Event-Jet.root"
    
    # 调用 Convert2Event-Jet_2.x 程序处理文件
    "$converter" "$file" "$output_filename"
}

# 导出处理函数，以便在 parallel 中使用
export -f process_file

# 并行处理源目录中的所有 root 文件
for file in "$source_dir"/*.root; do
    process_file "$file" &
    (( $(jobs -r | wc -l) >= max_jobs )) && wait -n
done
wait

