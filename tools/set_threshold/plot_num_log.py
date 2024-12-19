import matplotlib.pyplot as plt
import numpy as np

input_file = "/home/liang/Workingspace/EvenTF-code/set-threshold-code/merged-num-realnum.txt"

thresholds = []
predicted_counts = {i: [] for i in range(9)} 

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
        count = int(parts[2]) 

        if label == 0 and threshold not in thresholds:
            thresholds.append(threshold)
        predicted_counts[label].append(count)

start_index = thresholds.index(0.5)
thresholds = thresholds[start_index:]  
for label in range(9):
    predicted_counts[label] = predicted_counts[label][start_index:] 

font_size = 25  
legend_font_size = 18  
plt.rc('font', size=font_size)        
plt.rc('axes', titlesize=font_size)  
plt.rc('axes', labelsize=font_size) 
plt.rc('xtick', labelsize=font_size) 
plt.rc('ytick', labelsize=font_size) 
plt.rc('figure', titlesize=font_size) 

plt.figure(figsize=(14, 10))
handles = []
for label in range(9): 
    line, = plt.plot(thresholds, predicted_counts[label], label=label_names[label]) 
    handles.append(line)  

plt.yscale("log")

plt.legend(
    handles=handles,
    title="Labels",
    loc="upper center",  
    fontsize=legend_font_size,
    ncol=3,  
    bbox_to_anchor=(0.5, -0.15), 
    frameon=True
)
plt.xlabel("Threshold")
plt.ylabel("Predicted Counts (Log Scale)")
plt.grid(True, linestyle="--", linewidth=0.5) 
plt.tight_layout()
plt.savefig("predicted_counts_all_labels_log_no_minor_grid.png", bbox_inches="tight") 
plt.show()

