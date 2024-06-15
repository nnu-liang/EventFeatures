import os
import subprocess
import multiprocessing
from datetime import datetime

MG_DIR = "/home/lxiao/Workingspace/MG5_aMC_v3_4_2"
DELPHES_CARD_PATH = "/home/lxiao/Tagging/inputs/delphes_card_CMS.dat"
RUN_CARD_PATH = "/home/lxiao/Tagging/inputs/run_card.dat"
RUN_CARD_PATH2 = "/home/lxiao/Tagging/inputs/run_card_2.dat"

def run_process(process, output_dir, process_name, n, root_file_dir, madspin, madspin_card_path, dummy_fct_path):
    if not os.path.exists(root_file_dir):
        os.makedirs(root_file_dir)

    timestamp = datetime.now().strftime("%H%M%S%f")
    seed = datetime.now().strftime("%H%M%S%f")
    
    with open(RUN_CARD_PATH2, 'r') as file:
        data = file.readlines()
    for i in range(len(data)):
        if "iseed" in data[i]:
            data[i] = f"{seed} = iseed\n"
    with open(RUN_CARD_PATH2, 'w') as file:
        file.writelines(data)

    process_dir = f"{output_dir}/{process_name}_{timestamp}"

    if os.path.exists(process_dir):
        suffix = 1
        while os.path.exists(f"{process_dir}_{suffix}"):
            suffix += 1
        process_dir = f"{process_dir}_{suffix}"

    os.makedirs(process_dir)

    os.chdir(MG_DIR)
    subprocess.run(["./bin/mg5_aMC"], input=f"generate {process}\noutput {process_dir}\nexit\n", text=True)
    
    subprocess.run(["cp", "/home/lxiao/Tagging/inputs/trackResolutionCMS.tcl", f"{process_dir}/Cards/"])
    subprocess.run(["cp", dummy_fct_path, f"{process_dir}/SubProcesses/"])
    
    with open(f"{process_dir}/Cards/me5_configuration.txt", 'r') as file:
        config_data = file.readlines()
    for i in range(len(config_data)):
        if "# nb_core = None" in config_data[i]:
            config_data[i] = "nb_core = 8\n"
    with open(f"{process_dir}/Cards/me5_configuration.txt", 'w') as file:
        file.writelines(config_data)

    root_file_num = 1

    for i in range(1, int(n) + 1):
        if i == 1:
            if int(madspin) == 1:
                subprocess.run(["./bin/mg5_aMC"], input=f"launch {process_dir}\n1\n2\n4\ndone\n{madspin_card_path}\n{DELPHES_CARD_PATH}\n{RUN_CARD_PATH2}\nexit\n", text=True)
            else:
                subprocess.run(["./bin/mg5_aMC"], input=f"launch {process_dir}\n1\n2\ndone\n{DELPHES_CARD_PATH}\n{RUN_CARD_PATH2}\nexit\n", text=True)
        else:
            if int(madspin) == 1:
                subprocess.run(["./bin/mg5_aMC"], input=f"launch {process_dir}\ndone\n{madspin_card_path}\n{DELPHES_CARD_PATH}\n{RUN_CARD_PATH}\nexit\n", text=True)
            else:
                subprocess.run(["./bin/mg5_aMC"], input=f"launch {process_dir}\ndone\n{DELPHES_CARD_PATH}\n{RUN_CARD_PATH}\nexit\n", text=True)

        if int(madspin) == 1:
            run_dir = f"{process_dir}/Events/run_01_decayed_1/"
        else:
            run_dir = f"{process_dir}/Events/run_01/"
        
        if os.path.exists(run_dir):
            os.chdir(run_dir)
            if os.path.isfile("tag_1_delphes_events.root"):
                os.rename("tag_1_delphes_events.root", f"{root_file_dir}/{process_name}_{str(root_file_num).zfill(2)}_{timestamp}.root")
            os.chdir(MG_DIR)
            os.remove(f"{process_dir}/HTML/results.pkl")
            subprocess.run(["find", f"{process_dir}/HTML/", "-type", "d", "-name", "*run*", "-exec", "rm", "-rf", "{}", "+"])
            subprocess.run(["find", f"{process_dir}/Events/", "-mindepth", "1", "-type", "d", "-exec", "rm", "-rf", "{}", "+"])
        
        root_file_num += 1

    subprocess.run(["rm", "-rf", process_dir])

def main(config_file):
    with open(config_file, 'r') as file:
        lines = file.readlines()

    processes = []
    for line in lines:
        args = line.strip().split()
        if len(args) == 8:
            processes.append(tuple(args))
    
    with multiprocessing.Pool(processes=len(processes)) as pool:
        pool.starmap(run_process, processes)

if __name__ == "__main__":
    import sys
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <config_file>")
        sys.exit(1)
    
    config_file = sys.argv[1]
    main(config_file)

