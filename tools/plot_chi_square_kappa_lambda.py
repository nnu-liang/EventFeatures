import matplotlib.pyplot as plt
import numpy as np
import sys

def find_intersections(xs, ys, y_value):
    intersections = []
    for i in range(1, len(xs)):
        if (ys[i-1] < y_value and ys[i] >= y_value) or (ys[i-1] >= y_value and ys[i] < y_value):
            slope = (ys[i] - ys[i-1]) / (xs[i] - xs[i-1])
            x_intersect = xs[i-1] + (y_value - ys[i-1]) / slope
            intersections.append((x_intersect, y_value))
    return intersections

if len(sys.argv) < 3 or (len(sys.argv) - 1) % 2 != 0:
    print("Usage: {} <chi_squared1> <kappa_lambda1> <chi_squared2> <kappa_lambda2> ...".format(sys.argv[0]))
    sys.exit(1)

num_pairs = (len(sys.argv) - 1) // 2
chi_squared_values = [float(sys.argv[i + 1]) for i in range(num_pairs)]
kappa_lambda_values = [float(sys.argv[i + 1 + num_pairs]) for i in range(num_pairs)]

plt.plot(kappa_lambda_values, chi_squared_values, 'o-', label="χ² vs κλ")
plt.title("χ² - κλ Curve")
plt.xlabel("κλ (kappa_lambda)")
plt.ylabel("χ²")

plt.axhline(y=1.0, color='r', linestyle='--', label="χ² = 1")
plt.axhline(y=3.84, color='g', linestyle='--', label="χ² = 3.84")

intersections1 = find_intersections(kappa_lambda_values, chi_squared_values, 1.0)
intersections3_84 = find_intersections(kappa_lambda_values, chi_squared_values, 3.84)

for point in intersections1:
    plt.plot(point[0], point[1], 'ro')
    plt.annotate(f"{point[0]:.2f}", xy=(point[0], 0), xytext=(0, 5), textcoords='offset points', ha='center', color='red')

for point in intersections3_84:
    plt.plot(point[0], point[1], 'go')
    plt.annotate(f"{point[0]:.2f}", xy=(point[0], 0), xytext=(0, 5), textcoords='offset points', ha='center', color='green')

plt.legend()
plt.show()

