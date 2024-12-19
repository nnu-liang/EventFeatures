import numpy as np
import matplotlib.pyplot as plt

plt.rcParams.update({'font.size': 40})

data = np.loadtxt("thresholds_errors_acc-160.txt", skiprows=1)
thresholds = data[:, 0]
errors = data[:, 1] * 100  
accuracies = data[:, 2] * 100 

min_error_index = np.argmin(errors)
min_threshold = thresholds[min_error_index]
min_error = errors[min_error_index]
min_accuracy = accuracies[min_error_index]

processed_thresholds = np.concatenate((thresholds[:min_error_index:10], thresholds[min_error_index::3]))
processed_errors = np.concatenate((errors[:min_error_index:10], errors[min_error_index::3]))
processed_accuracies = np.concatenate((accuracies[:min_error_index:10], accuracies[min_error_index::3]))

fig, ax1 = plt.subplots(figsize=(12, 8))

ax1.plot(processed_thresholds, processed_errors, marker='o', linestyle='-', color='blue', label='Relative Error (%)')
ax1.set_xlabel('Threshold') 
ax1.set_ylabel('Relative Error (%)', color='blue')  
ax1.tick_params(axis='y', labelcolor='blue')  
ax1.grid()  

ax2 = ax1.twinx()
ax2.plot(processed_thresholds, processed_accuracies, marker='s', linestyle='--', color='green', label='Signal Efficiency (%)')
ax2.set_ylabel('Signal Efficiency (%)', color='green')  
ax2.tick_params(axis='y', labelcolor='green')  

for x in [0, 0.426, 0.788, 0.903]:
    ax1.axvline(x=x, color='purple', linestyle='--', linewidth=3)
for y in [53.4493, 50, 30, 20]:
    ax2.axhline(y=y, color='orange', linestyle='--', linewidth=3)
lines1, labels1 = ax1.get_legend_handles_labels()
lines2, labels2 = ax2.get_legend_handles_labels()
combined_lines = lines1 + lines2
combined_labels = labels1 + labels2
combined_lines.append(plt.Line2D([0], [0], color='none')) 
combined_labels.append(r"$L=3000 \, \mathrm{fb}^{-1}, \, 68\% \, \mathrm{CL}$")
ax1.legend(combined_lines, combined_labels, loc='lower left', bbox_to_anchor=(0, 0))

plt.title('Relative Error and Signal Efficiency vs Threshold')

plt.show()

