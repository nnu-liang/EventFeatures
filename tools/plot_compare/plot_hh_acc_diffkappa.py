import matplotlib.pyplot as plt

x = [-5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
y1 = [35.5, 37.29, 38.8, 40.3, 43, 51, 53.6, 58, 30.8, 19.24, 18.8, 20.46, 22.23, 23.57, 23.71, 24.09]

y2 = [53.849, 53.849, 54.0557, 54.3443, 54.7737, 55.3483, 56.5783, 58.0587, 57.937, 51.1317, 49.0733, 49.3593, 49.9487, 50.3517, 50.6877, 51.124]

plt.figure(figsize=(8, 8)) 

plt.plot(x, y1, marker='o', linestyle='-', color='blue', label=r"$\kappa_{\lambda}=1$")

plt.plot(x, y2[:len(x)], marker='s', linestyle='--', color='red', label=r"$\kappa_{\lambda}=-1,1,4,6,8$")

plt.xlabel(r"$\kappa_{\lambda}$", fontsize=40)
plt.ylabel(r"$HH$ efficiency(%)", fontsize=30)

plt.xticks(range(-5, 11), fontsize=30)
plt.yticks(fontsize=30)

plt.grid(alpha=0.3)
plt.legend(fontsize=30)

plt.tight_layout()

plt.show()
