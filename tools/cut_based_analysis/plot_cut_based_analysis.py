import numpy as np
import math
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d

kappa_lambda = np.array([-17, -16, -15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25])

hh_accuracies_list = [
    [1.29, 1.29, 1.29, 1.29, 1.29, 1.29, 1.29, 1.29, 1.29, 1.29, 1.29, 1.29, 1.29, 1.33, 1.42, 1.48, 1.60, 1.88, 2.28, 2.60, 1.41, 0.75, 0.68, 0.69, 0.73, 0.78, 0.81, 0.83, 0.85, 0.86, 0.81, 0.88, 0.87, 0.90, 0.90, 0.95, 0.95, 0.93, 0.93, 0.93, 0.93, 0.93, 0.93],
]
hh_accuracies_list = [np.array([x / 100 for x in accuracies]) for accuracies in hh_accuracies_list]

ps_values_kappa1_list = [2.28 / 100]
BR_hh_4b = 0.58 * 0.58

confusion_matrices = [
    [0.0002, 0.0044, 0.0013, 0.0008, 0.0001, 0.0000033, 0.0001, 0.0008]
]

hh_cross_sections = (9.96265e-3 * kappa_lambda**2 - 4.84745e-2 * kappa_lambda + 7.32829e-2) * 1000
hh_cross_section_sm = (9.96265e-3 - 4.84745e-2 + 7.32829e-2) * 1000

g = interp1d(kappa_lambda, hh_cross_sections, kind="cubic", fill_value="extrapolate")

threshold_labels = [r""]#, r"$p_{th}=0.5$", r"$p_{th}=0.7$", r"$p_{th}=0.9$"]
threshold_labels2 = ["threshold=0"]#, "threshold=0.5", "threshold=0.7", "threshold=0.9"]
colors = ["red"]#, "green", "orange", "purple"]

configurations = [
    {"L": 3000, "chi_square_target": 1, "sigma_b": np.array([3.1318e+05, 9.370e+01, 5.996e+03, 2.506e+01, 3.952e+06, 7.371e+08, 2.135e+02, 4.647e+01]), "title": "$\\mathcal{L}_{\\mathrm{int}}=3000\\ \\mathrm{fb}^{-1}$ 68% CL"},
    {"L": 300, "chi_square_target": 3.841, "sigma_b": np.array([0, 0, 0, 0, 3.952e+06, 0, 0, 0]), "title": "$\\mathcal{L}_{\\mathrm{int}}=300\\ \\mathrm{fb}^{-1}$ 95% CL"},
    {"L": 138, "chi_square_target": 3.841, "sigma_b": np.array([3.1318e+05, 9.370e+01, 5.996e+03, 2.506e+01, 3.952e+06, 7.371e+08, 2.135e+02, 4.647e+01]), "title": "$\\mathcal{L}_{\\mathrm{int}}=138\\ \\mathrm{fb}^{-1}$ 95% CL"},
    {"L": 126, "chi_square_target": 3.841, "sigma_b": np.array([3.1318e+05, 9.370e+01, 5.996e+03, 2.506e+01, 3.952e+06, 7.371e+08, 2.135e+02, 4.647e+01]), "title": "$\\mathcal{L}_{\\mathrm{int}}=126\\ \\mathrm{fb}^{-1}$ 95% CL"}
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

    plt.savefig(file_name, bbox_inches='tight', dpi=300)
    plt.show()
