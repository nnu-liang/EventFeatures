import re
import numpy as np
import math

# Constants
BR_hh_4b = 0.58 * 0.58
hh_cross_section_sm = (9.96265e-3 - 4.84745e-2 + 7.32829e-2) * 1000
chi_square_target = 1
L = 3000
sigma_b = np.array([3.1318e+05, 9.370e+01, 5.996e+03, 2.506e+01, 3.952e+06, 7.371e+08, 2.135e+02, 4.647e+01])

# Function to parse the input file
def parse_file(input_file):
    data = {}
    with open(input_file, "r") as f:
        for line in f:
            match = re.match(r"^(\S+): (\S+): ([\d\.]+)%", line)
            if match:
                filename, category, value = match.groups()
                value = float(value) / 100  # Convert percentage to decimal
                if filename not in data:
                    data[filename] = {}
                data[filename][category] = value
    return data

# Perform calculations and save results
def process_data(data, output_file):
    with open(output_file, "w") as f:
        for filename, categories in data.items():
            try:
                hh_accuracies = categories.get("hh_4b_kappa", 0)
                ps_values_kappa1 = categories.get("hh_4b", 0)
                pb_keys = ["tt_2b4j", "tth_4b4j", "ttbb_4b4j", "hbb_4b", "4b", "2b2j", "zz_4b", "zh_4b"]
                pb = [categories.get(key, 0) for key in pb_keys]

                # Ensure all probabilities are present
                if not hh_accuracies or not ps_values_kappa1 or len(pb) != len(sigma_b):
                    raise ValueError("Missing probabilities or mismatch in background categories.")

                # Calculate sum of backgrounds
                sum_background = np.sum(np.array(pb) * sigma_b * L)

                # Calculate target cross-section
                numerator_part1 = chi_square_target * (ps_values_kappa1 * hh_cross_section_sm * BR_hh_4b * L + sum_background)
                sqrt_part = math.sqrt(numerator_part1)
                numerator_part2 = ps_values_kappa1 * hh_cross_section_sm * BR_hh_4b * L
                numerator_all = sqrt_part + numerator_part2
                denominator = hh_accuracies * BR_hh_4b * L
                result = numerator_all / (denominator * 1000)

                # Calculate relative error
                relative_error = abs(result - hh_cross_section_sm) / hh_cross_section_sm

                # Write to output file
                f.write(f"{filename}: Target Cross-Section: {result:.6f} pb, Relative Error: {relative_error:.6%}\n")
            except Exception as e:
                f.write(f"{filename}: Error in calculation - {str(e)}\n")

if __name__ == "__main__":
    input_file = "input.txt"  # Replace with your input file path
    output_file = "output_results.txt"  # Replace with your desired output file path

    # Parse input file
    data = parse_file(input_file)

    # Process data and save results
    process_data(data, output_file)

    print(f"Results saved to {output_file}")

