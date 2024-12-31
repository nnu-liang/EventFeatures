import os
import subprocess

input_dir = "output/"
output_file = "results.txt"
thresholds = [0, 0.5, 0.7, 0.9]
program = "./set-threshold-calculate-all-labels-predict-hh"

root_files = sorted([f for f in os.listdir(input_dir) if f.endswith(".root")])

with open(output_file, "w") as out:
    all_val_file = "all-val-output.root"
    if all_val_file in root_files:
        root_files.remove(all_val_file)  
        out.write(f"# Results for {all_val_file}\n")
        for threshold in thresholds:
            command = [program, os.path.join(input_dir, all_val_file), str(threshold)]
            try:
                result = subprocess.check_output(command, text=True)
                out.write(f"Threshold: {threshold}\n")
                out.write(result)
                out.write("\n")
            except subprocess.CalledProcessError as e:
                print(f"Error processing {all_val_file} with threshold {threshold}: {e}")

    out.write("# Results for other files\n")
    for root_file in root_files:
        out.write(f"# Results for {root_file}\n")
        for threshold in thresholds:
            command = [program, os.path.join(input_dir, root_file), str(threshold)]
            try:
                result = subprocess.check_output(command, text=True)
                hh_4b_line = next((line for line in result.splitlines() if line.startswith("hh_4b:")), None)
                if hh_4b_line:
                    out.write(f"{root_file}, Threshold: {threshold}, {hh_4b_line}\n")
            except subprocess.CalledProcessError as e:
                print(f"Error processing {root_file} with threshold {threshold}: {e}")

