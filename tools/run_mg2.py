import os
import subprocess
import multiprocessing
from datetime import datetime
import random
import shlex
import re

MG_DIR = "/home/lxiao/Workingspace/MG5_aMC_v3_4_2"
DELPHES_CARD_PATH = "/home/lxiao/Tagging/inputs/delphes_card_CMS.dat"
RUN_CARD_PATH = "/home/lxiao/Tagging/inputs/run_card.dat"
RUN_CARD_PATH2 = "/home/lxiao/Tagging/inputs/run_card_2.dat"

def generate_seed():
    return random.randint(1, 904896640)

def modify_run_card(file_path):
    pt_start_marker = r"# Standard Cuts"
    pt_end_marker = r"# Minimum and maximum DeltaR distance                                *"

    new_pt_content = """
# Standard Cuts                                                      *
#*********************************************************************
# Minimum and maximum pt's (for max, -1 means no cut)                *
#*********************************************************************
 20.0  = ptb       ! minimum pt for the b 
 -1.0  = ptbmax    ! maximum pt for the b
 20.0  = ptj
 -1.0  = ptjmax
 {} = pt_min_pdg ! pt cut for other particles (use pdg code). Applied on particle and anti-particle
 {}    = pt_max_pdg ! pt cut for other particles (syntax e.g. {6: 100, 25: 50}) 
#
# For display option for energy cut in the partonic center of mass frame type 'update ecut'
#
#*********************************************************************
# Maximum and minimum absolute rapidity (for max, -1 means no cut)   *
#*********************************************************************
  4.0  = etab    ! max rap for the b
 -4.0  = etabmin ! min rap for the b
  4.0  = etaj
 -4.0  = etajmin
 {} = eta_min_pdg ! rap cut for other particles (use pdg code). Applied on particle and anti-particle
 {} = eta_max_pdg ! rap cut for other particles (syntax e.g. {6: 2.5, 23: 5})
#*********************************************************************
# Minimum and maximum DeltaR distance                                *
"""

    with open(file_path, 'r', encoding='utf-8') as file:
        data = file.read()

    pt_pattern = re.compile(rf"{re.escape(pt_start_marker)}.*?{re.escape(pt_end_marker)}", re.DOTALL)
    modified_data = pt_pattern.sub(new_pt_content.strip() + "\n", data)

    with open(file_path, 'w', encoding='utf-8') as file:
        file.write(modified_data)

def run_process(process, output_dir, process_name, n, root_file_dir, madspin, madspin_card_path, dummy_fct_path):
    try:
        os.makedirs(root_file_dir, exist_ok=True)
    except FileExistsError:
        pass
    timestamp = datetime.now().strftime("%H%M%S%f")
    seed = generate_seed()

    process_dir = f"{output_dir}/{process_name}_{timestamp}"

    if os.path.exists(process_dir):
        suffix = 1
        while os.path.exists(f"{process_dir}_{suffix}"):
            suffix += 1
        process_dir = f"{process_dir}_{suffix}"

    os.makedirs(process_dir, exist_ok=True)

    os.chdir(MG_DIR)
    subprocess.run(["./bin/mg5_aMC"], input=f"generate {process}\noutput {process_dir}\nexit\n", text=True)
    
    subprocess.run(["cp", "/home/lxiao/Tagging/inputs/trackResolutionCMS.tcl", f"{process_dir}/Cards/"])
    subprocess.run(["cp", dummy_fct_path, f"{process_dir}/SubProcesses/"])
        # 修改run_card.dat文件
    run_card_path = f"{process_dir}/Cards/run_card.dat"
    modify_run_card(run_card_path)
        # 复制run_card.dat为run_card_2.dat
    run_card_path = f"{process_dir}/Cards/run_card.dat"
    run_card_path2 = f"{process_dir}/Cards/run_card_2.dat"
    subprocess.run(["cp", run_card_path, run_card_path2])
    
        # 修改run_card.dat文件
    with open(run_card_path, 'r', encoding='utf-8') as file:
        data = file.readlines()

    print(f"Original file content:\n{''.join(data)}")  # 调试输出，检查文件内容

    # 定义正则表达式模式，匹配包含数字和 "iseed" 的行
    pattern = re.compile(r'^\s*[0-9]+\s*=\s*iseed', re.IGNORECASE)

    # 替换 iseed 行
    for i in range(len(data)):
        if pattern.search(data[i]):
            data[i] = re.sub(r'^\s*[0-9]+\s*=\s*iseed', f'{seed} = iseed', data[i])
            break

    print(f"Modified file content:\n{''.join(data)}")  # 调试输出，检查修改后的文件内容

    # 将修改后的内容写回文件
    with open(run_card_path, 'w', encoding='utf-8') as file:
        file.writelines(data)
    
    with open(f"{process_dir}/Cards/me5_configuration.txt", 'r') as file:
        config_data = file.readlines()
    for i in range(len(config_data)):
        if "# nb_core = None" in config_data[i]:
            config_data[i] = "nb_core = 4\n"
    with open(f"{process_dir}/Cards/me5_configuration.txt", 'w') as file:
        file.writelines(config_data)

    root_file_num = 1

    for i in range(1, int(n) + 1):
        if i == 1:
            if int(madspin) == 1:
                subprocess.run(["./bin/mg5_aMC"], input=f"launch {process_dir}\n1\n2\n4\ndone\n{madspin_card_path}\n{DELPHES_CARD_PATH}\nexit\n", text=True)
            else:
                subprocess.run(["./bin/mg5_aMC"], input=f"launch {process_dir}\n1\n2\ndone\n{DELPHES_CARD_PATH}\nexit\n", text=True)
        else:
            if int(madspin) == 1:
                subprocess.run(["./bin/mg5_aMC"], input=f"launch {process_dir}\ndone\n{madspin_card_path}\n{DELPHES_CARD_PATH}\n{RUN_CARD_PATH2}\nexit\n", text=True)
            else:
                subprocess.run(["./bin/mg5_aMC"], input=f"launch {process_dir}\ndone\n{DELPHES_CARD_PATH}\n{RUN_CARD_PATH2}\nexit\n", text=True)

        if int(madspin) == 1:
            run_dir = f"{process_dir}/Events/run_01_decayed_1/"
        else:
            run_dir = f"{process_dir}/Events/run_01/"
        
        if os.path.exists(run_dir):
            os.chdir(run_dir)
            if os.path.isfile("tag_1_delphes_events.root"):
                os.rename("tag_1_delphes_events.root", f"{root_file_dir}/{process_name}_{str(root_file_num).zfill(2)}_{timestamp}.root")
            os.chdir(MG_DIR)
            if os.path.isfile(f"{process_dir}/HTML/results.pkl"):
                os.remove(f"{process_dir}/HTML/results.pkl")
            subprocess.run(["find", f"{process_dir}/HTML/", "-type", "d", "-name", "*run*", "-exec", "rm", "-rf", "{}", "+"])
            subprocess.run(["find", f"{process_dir}/Events/", "-mindepth", "1", "-type", "d", "-exec", "rm", "-rf", "{}", "+"])
        
        root_file_num += 1

    subprocess.run(["rm", "-rf", process_dir])
    print(f"完成：数据已在 {process_dir} 中生成，tag_1_delphes_events.root 文件已移至 {root_file_dir}")
    print(f"当前的时间戳是: {timestamp}")

def main(config_file):
    with open(config_file, 'r') as file:
        lines = file.readlines()

    processes = []
    for line in lines:
        args = shlex.split(line.strip())
        if len(args) == 8:
            processes.append(tuple(args))
    
    if len(processes) == 0:
        print("No valid processes found in the configuration file.")
        return

    print(f"Found {len(processes)} processes in the configuration file.")

    with multiprocessing.Pool(processes=len(processes)) as pool:
        pool.starmap(run_process, processes)

if __name__ == "__main__":
    import sys
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <config_file>")
        sys.exit(1)
    
    config_file = sys.argv[1]
    main(config_file)

