import matplotlib.pyplot as plt
import numpy as np

input_file = "/home/liang/Workingspace/EvenTF-code/set-threshold-code/merged-num-realnum.txt"

thresholds = []
real_event_numbers = {i: [] for i in range(9)} 

label_names = [
    "hh_4b",
    "tt_2b4j",
    "tth_4b4j",
    "ttbb_4b4j",
    "hbb_4b",
    "4b",
    "2b2j",
    "zz_4b",
    "zh_4b"
]

with open(input_file, "r") as file:
    lines = file.readlines()
    for line in lines[1:]:
        parts = line.strip().split()
        threshold = float(parts[0])
        label = int(parts[1])
        real_event_number = float(parts[3]) 

        if label == 0 and threshold not in thresholds:
            thresholds.append(threshold)
        real_event_numbers[label].append(real_event_number)

start_index = thresholds.index(0.5)
thresholds = thresholds[start_index:]  
for label in range(9):
    real_event_numbers[label] = real_event_numbers[label][start_index:]  

font_size = 25
plt.rc('font', size=font_size)        
plt.rc('axes', titlesize=font_size)  
plt.rc('axes', labelsize=font_size) 
plt.rc('xtick', labelsize=font_size) 
plt.rc('ytick', labelsize=font_size) 
plt.rc('legend', fontsize=font_size) 
plt.rc('figure', titlesize=font_size) 

plt.figure(figsize=(16, 12))
handles = []
for label in range(9):
    line, = plt.plot(thresholds, real_event_numbers[label], label=label_names[label])  
    handles.append(line)  

plt.yscale("log")

plt.legend(
    title="Labels",
    loc="lower left", 
    fontsize=font_size - 3,
    ncol=2,  
    frameon=True
)
plt.xlabel("Threshold")
plt.ylabel("Real Event Numbers (Log Scale)")
plt.grid(True, linestyle="--", linewidth=0.5)  
plt.tight_layout()
plt.savefig("real_event_numbers_all_labels_log_scale_lower_left.png")
plt.show()

