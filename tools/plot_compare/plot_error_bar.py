import matplotlib.pyplot as plt
import numpy as np

scenarios = ["DNN,3000 fb⁻¹,68%CL", "SPA-NET,300 fb⁻¹,95%CL", "cut-based,3000 fb⁻¹,68%CL"]
x_positions = np.arange(len(scenarios)) * 0.8  # X-axis positions for scenarios

# Data for Threshold=0
threshold_0_errors_below = [2.37, 3.39, 2.37]
threshold_0_errors_above = [5.44, 6.46, 5.44]
threshold_0_ranges = [(-1.37, 6.44), (-2.39, 7.46), (-1.37, 6.44)]

# Data for Threshold=0.5
threshold_05_errors_below = [2.11, 3.12, 2.11]
threshold_05_errors_above = [5.23, 6.24, 5.23]
threshold_05_ranges = [(-1.11, 6.23), (-2.12, 7.24), (-1.11, 6.23)]

# Data for Threshold=0.7
threshold_07_errors_below = [1.78, 2.90, 1.78]
threshold_07_errors_above = [5.03, 6.12, 5.03]
threshold_07_ranges = [(-0.78, 6.03), (-1.90, 7.12), (-0.78, 6.03)]

# Data for Threshold=0.9
threshold_09_errors_below = [1.53, 2.66, 1.53]
threshold_09_errors_above = [5.01, 6.12, 5.01]
threshold_09_ranges = [(-0.53, 6.01), (-1.66, 7.12), (-0.53, 6.01)]

# Data for References
reference_errors_below = [1.8, 2.56, 3.47]  
reference_errors_above = [5.6, 6.57, 8.10]  
reference_ranges = [(-0.8, 6.6), (-1.56, 7.57), (-2.47, 9.10)]  

# Bar widths and positions
bar_width = 0.15  
offsets = np.linspace(-1.8 * bar_width, 1.8 * bar_width, 5)  

colors = ['red', 'green', 'orange', 'purple', 'blue']
labels = [r"$p_{th} = 0$", r"$p_{th} = 0.5$", r"$p_{th} = 0.7$", r"$p_{th} = 0.9$", "References"]

# Create figure
plt.figure(figsize=(28, 15))  # Increased figure size for better readability

for i, (errors_below, errors_above, ranges, label, color) in enumerate(
    zip(
        [threshold_0_errors_below, threshold_05_errors_below, threshold_07_errors_below, threshold_09_errors_below, reference_errors_below],
        [threshold_0_errors_above, threshold_05_errors_above, threshold_07_errors_above, threshold_09_errors_above, reference_errors_above],
        [threshold_0_ranges, threshold_05_ranges, threshold_07_ranges, threshold_09_ranges, reference_ranges],
        labels,
        colors,
    )
):

    plt.errorbar(
        x_positions + offsets[i],
        [1] * len(scenarios),
        yerr=[errors_below, errors_above],
        fmt='o',
        capsize=10,  
        label=label,
        color=color,
        linestyle=''
    )

    # Annotate ranges with alternating positions
    for j, (x, (low, high)) in enumerate(zip(x_positions + offsets[i], ranges)):
        if (i + j) % 2 == 0:
            # Place annotation above the error bar
            text_y = 1 + errors_above[j] + 0.8
            va = 'bottom'  # Vertical alignment
        else:
            # Place annotation below the error bar
            text_y = 1 - errors_below[j] - 0.8
            va = 'top'
        plt.text(x, text_y, f"[{low:.2f}, {high:.2f}]", color=color, ha='center', va=va, fontsize=25)

plt.axhline(y=1, color='gray', linestyle='--', linewidth=2)#, label=r'SM Value ($\kappa_\lambda=1$)')  # Bold line
plt.xticks(x_positions, scenarios, rotation=15, fontsize=35)  # Increased x-axis tick font size
plt.yticks(fontsize=40)  # Increased y-axis tick font size
plt.ylabel(r"$\kappa_\lambda$", fontsize=40)  # Increased y-axis label font size
plt.legend(fontsize=34, loc='upper left')  # Increased legend font size
plt.grid(True, linestyle='--', alpha=0.6)

# Set Y-axis limits
plt.ylim(-6, 19)  

plt.tight_layout()

# Show plot
plt.show() 
