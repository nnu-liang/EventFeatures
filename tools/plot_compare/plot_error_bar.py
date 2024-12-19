import matplotlib.pyplot as plt
import numpy as np

scenarios = ["2004.04240,3000 fb⁻¹,68%CL", "2401.14198,300 fb⁻¹,95%CL", "CMS,138 fb⁻¹,95%CL", "ATLAS,126 fb⁻¹,95%CL"]
x_positions = np.arange(len(scenarios))  # X-axis positions for scenarios

# Data for Threshold=0
threshold_0_errors_below = [2.47, 1.06, 10.30, 10.56]
threshold_0_errors_above = [6.79, 5.61, 15.70, 16.02]
threshold_0_ranges = [(-1.47, 7.79), (-0.06, 6.61), (-9.30, 16.70), (-9.56, 17.02)]

# Data for Threshold=0.5
threshold_05_errors_below = [2.39, 1.09, 10.01, 10.26]
threshold_05_errors_above = [6.94, 5.66, 15.87, 16.19]
threshold_05_ranges = [(-1.39, 7.94), (-0.09, 6.66), (-9.01, 16.87), (-9.26, 17.19)]

# Data for Threshold=0.7
threshold_07_errors_below = [2.16, 0.95, 9.17, 9.40]
threshold_07_errors_above = [6.90, 5.73, 15.46, 15.77]
threshold_07_ranges = [(-1.16, 7.90), (0.05, 6.73), (-8.17, 16.46), (-8.40, 16.77)]

# Data for Threshold=0.9
threshold_09_errors_below = [2.02, 0.72, 8.36, 8.57]
threshold_09_errors_above = [7.07, 5.91, 15.22, 15.52]
threshold_09_ranges = [(-1.02, 8.07), (0.28, 6.91), (-7.36, 16.22), (-7.57, 16.52)]

# Data for References
reference_errors_below = [1.8, 2.56, 3.3, 4.5]  
reference_errors_above = [5.6, 6.57, 8.4, 10.3]  
reference_ranges = [(-0.8, 6.6), (-1.56, 7.57), (-2.3, 9.4), (-3.5, 11.3)]  

# Bar widths and positions
bar_width = 0.15  
offsets = np.linspace(-2 * bar_width, 2 * bar_width, 5)  

colors = ['red', 'green', 'orange', 'purple', 'blue']
labels = [r"$\epsilon = 53\%$", r"$\epsilon = 50\%$", r"$\epsilon = 30\%$", r"$\epsilon = 20\%$", "References"]

# Create figure
plt.figure(figsize=(20, 10))  # Increased figure size for better readability

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
        plt.text(x, text_y, f"[{low:.2f}, {high:.2f}]", color=color, ha='center', va=va, fontsize=16)

plt.axhline(y=1, color='gray', linestyle='--', linewidth=2, label=r'SM Value ($\kappa_\lambda=1$)')  # Bold line
plt.xticks(x_positions, scenarios, rotation=15, fontsize=25)  # Increased x-axis tick font size
plt.yticks(fontsize=25)  # Increased y-axis tick font size
plt.ylabel(r"$\kappa_\lambda$", fontsize=25)  # Increased y-axis label font size
plt.legend(fontsize=20, loc='upper left')  # Increased legend font size
plt.grid(True, linestyle='--', alpha=0.6)

# Set Y-axis limits
plt.ylim(-11, 19)  

plt.tight_layout()

# Show plot
plt.show() 
