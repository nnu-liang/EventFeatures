import numpy as np
import math
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d

# different kappa_lambda
kappa_lambda = np.array([ -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18])

# The efficiency of hh_4b corresponding to different thresholds（different kappa_lambda）
hh_accuracies_list = [
    [35.5, 35.5, 35.5, 35.5, 35.5, 35.5, 35.5, 37.3, 38.8, 40.3, 43, 54, 55.3194, 58, 30.8, 19.24, 18.8, 20.46, 22.23, 23.57, 23.71, 24.09, 24.09, 24.09, 24.09, 24.09, 24.09, 24.09, 24.09, 24.09],
    [29.55, 29.55, 29.55, 29.55, 29.55, 29.55, 29.55, 30.72, 32.44, 33.86, 36.1, 48, 46.65, 52.03, 27.17, 14.81, 13.97, 15.39, 16.57, 18.1, 18.49, 18.64, 18.64, 18.64, 18.64, 18.64, 18.64, 18.64, 18.64, 18.64],
    [21.6, 21.6, 21.6, 21.6, 21.6, 21.6, 21.6, 21.93, 23.39, 24.68, 26.56, 30.14, 35.5759, 41.48, 21.76, 10.48, 9.3, 10.32, 11.05, 12.23, 12.38, 12.4, 12.4, 12.4, 12.4, 12.4, 12.4, 12.4, 12.4, 12.4],
    [11.6, 11.6, 11.6, 11.6, 11.6, 11.6, 11.6, 11.41, 12.13, 12.97, 14.03, 16.52, 18.1508, 25.3, 13.16, 5.48, 4.64, 4.89, 5.1, 5.61, 5.81, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9]
]
hh_accuracies_list = [np.array([x / 100 for x in accuracies]) for accuracies in hh_accuracies_list]

# The efficiency of hh_4b corresponding to different thresholds(kappa_lambda=1)
ps_values_kappa1_list = [55.3194 / 100, 48 / 100, 35.5759 / 100, 18.1508 / 100]
BR_hh_4b = 0.58 * 0.58
# The background misclassification rate corresponding to different thresholds
confusion_matrices = [
    [0.00255467, 0.00902733, 0.00315608, 0.057771, 0.0142256, 0.000392905, 0.012417, 0.0605947],
    [0.00138333, 0.00889333, 0.00232883, 0.01424, 0.00198321, 0.000191198, 0.00325333, 0.02443],
    [0.00045, 0.00378, 0.00072309, 0.00664, 0.000736524, 0.0000616986, 0.000996667, 0.0106067],
    [0.0000256667, 0.000179, 0.0000186429, 0.00275933, 0.000354775, 0.00000711907, 0.0000963333, 0.00237267]
]
# The cross-section corresponding to different kappa_lambda values
hh_cross_sections = (9.96265e-3 * kappa_lambda**2 - 4.84745e-2 * kappa_lambda + 7.32829e-2) * 1000
hh_cross_section_sm = (9.96265e-3 - 4.84745e-2 + 7.32829e-2) * 1000

g = interp1d(kappa_lambda, hh_cross_sections, kind="cubic", fill_value="extrapolate")
threshold_labels = [r"$p_{th}=0.0$", r"$p_{th}=0.5$", r"$p_{th}=0.7$", r"$p_{th}=0.9$"]
threshold_labels2 = ["threshold=0", "threshold=0.5", "threshold=0.7", "threshold=0.9"]
colors = ["red", "green", "orange", "purple"]

configurations = [
    {"L": 3000, "chi_square_target": 1, "sigma_b": np.array([3.1318e+05, 9.370e+01, 5.996e+03, 2.506e+01, 3.952e+06, 7.371e+08, 2.135e+02, 4.647e+01]), "title": "$\\mathcal{L}_{\\text{int}}=3000\\ \\mathrm{fb}^{-1}$ 68% CL"},
    {"L": 300, "chi_square_target": 3.841, "sigma_b": np.array([0, 0, 0, 0, 3.952e+06, 0, 0, 0]), "title": "$\\mathcal{L}_{\\text{int}}=300\\ \\mathrm{fb}^{-1}$ 95% CL"},
    {"L": 138, "chi_square_target": 3.841, "sigma_b": np.array([3.1318e+05, 9.370e+01, 5.996e+03, 2.506e+01, 3.952e+06, 7.371e+08, 2.135e+02, 4.647e+01]), "title": "$\\mathcal{L}_{\\text{int}}=138\\ \\mathrm{fb}^{-1}$ 95% CL"},
    {"L": 126, "chi_square_target": 3.841, "sigma_b": np.array([3.1318e+05, 9.370e+01, 5.996e+03, 2.506e+01, 3.952e+06, 7.371e+08, 2.135e+02, 4.647e+01]), "title": "$\\mathcal{L}_{\\text{int}}=126\\ \\mathrm{fb}^{-1}$ 95% CL"}
]

for config in configurations:
    L = config["L"]
    chi_square_target = config["chi_square_target"]
    sigma_b = config["sigma_b"]
    title = config["title"]
    plt.figure(figsize=(10, 8))
    ymin = float('inf')
    ymax = float('-inf')
    all_intersection_points = []  
    all_intersection_points_flat = []
    for i, (confusion_matrix_new, hh_accuracies, ps_values_kappa1) in enumerate(zip(confusion_matrices, hh_accuracies_list, ps_values_kappa1_list)):
        pb = confusion_matrix_new
        sum_background = np.sum(pb * sigma_b * L)

        target_cross_sections = []
        for j, sigma_di in enumerate(hh_cross_sections):
            ps_kappa = hh_accuracies[j]
            numerator_part1 = chi_square_target * (ps_values_kappa1 * hh_cross_section_sm * BR_hh_4b * L + sum_background)
            sqrt_part = math.sqrt(numerator_part1)
            numerator_part2 = ps_values_kappa1 * hh_cross_section_sm * BR_hh_4b * L
            numerator_all = sqrt_part + numerator_part2
            denominator = ps_kappa * BR_hh_4b * L
            result = numerator_all / (denominator * 1000)
            target_cross_sections.append(result)

        target_cross_sections = np.array(target_cross_sections)

        hh_at_data = hh_cross_sections / 1000
        diff_values = target_cross_sections - hh_at_data
        sign_changes = np.where(np.diff(np.sign(diff_values)))[0]

        intersection_points = []
        for idx in sign_changes:
            x1, x2 = kappa_lambda[idx], kappa_lambda[idx + 1]
            y1, y2 = diff_values[idx], diff_values[idx + 1]
            x_cross = x1 - y1 * (x2 - x1) / (y2 - y1)
            intersection_points.append(x_cross)
        all_intersection_points_flat.extend(intersection_points)
        all_intersection_points.append(intersection_points)
        if len(intersection_points) == 2:
            intersection_text = f"({intersection_points[0]:.2f}, {intersection_points[1]:.2f})"
        else:
            intersection_text = f"({', '.join([f'{x:.2f}' for x in intersection_points])})"
        ymax = max(ymax, np.max(target_cross_sections))
        plt.plot(kappa_lambda, target_cross_sections, linestyle='-', color=colors[i],
                 label=f"{threshold_labels[i]} $\\kappa_\\lambda$ in {intersection_text}")
    print(all_intersection_points)
    kappa_fine = np.linspace(kappa_lambda.min(), kappa_lambda.max(), 1000)
    hh_cross_sections_fine = g(kappa_fine) / 1000
    ymin = np.min(hh_cross_sections_fine)
    plt.plot(kappa_fine, hh_cross_sections_fine, color="black", linestyle="-", linewidth=1.5,
             label="HH Cross Sections")
    if all_intersection_points_flat: 
        xmin = min(all_intersection_points_flat) - 1  
        xmax = max(all_intersection_points_flat) + 1
    #plt.ylim(0, 0.8)
    plt.ylim(ymin * 0.2, ymax * 2.5)
    plt.xlim(xmin, xmax)
    plt.xlabel(r'$\kappa_\lambda$', fontsize=25)
    plt.ylabel(r'$\sigma(gg \to hh)\ \mathrm{[pb]}$', fontsize=25)
    plt.xticks(fontsize=25)
    plt.yticks(fontsize=25)
    plt.grid(True)
    plt.legend(fontsize=25, bbox_to_anchor=(0.5, 0.7), loc="center", title=title, title_fontsize=25)
    L_value = title.split('=')[1].split('\\')[0]  
    CL_value = title.split()[-2]  
    file_name = f"L={L_value}_{CL_value}CL.png"

    with open("/Users/lxiao/Desktop/plot/FIG/all-compare/output.txt", "a") as f:  
        f.write(f"File: {file_name}\n")  
        for threshold_label, intersection_points in zip(threshold_labels2, all_intersection_points):
            if isinstance(intersection_points, (list, np.ndarray)):  
                formatted_points = " ".join([f"{x:.2f}" for x in intersection_points])  
            else:  
                formatted_points = f"({intersection_points:.2f})"
            f.write(f"{threshold_label}: {formatted_points}\n")
        f.write("\n")  

    plt.savefig(f"/Users/lxiao/Desktop/plot/FIG/all-compare/{file_name}", bbox_inches='tight', dpi=300)
    plt.show()

