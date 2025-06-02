import matplotlib.pyplot as plt

kappa_lambda = list(range(-5, 11)) 

cross_section = [
    0.2352, 0.178, 0.1286, 0.08751, 0.05488, 0.03046, 0.01448, 0.006702, 0.007261, 0.0161, 0.03332, 0.0588, 0.0926, 0.1347, 0.1848, 0.2438
]

plt.figure(figsize=(8, 7))
plt.plot(kappa_lambda, cross_section, marker='o', linestyle='-', color='red', linewidth=2)
plt.xlabel(r'$\kappa_{\lambda}$', fontsize=25)
plt.ylabel(r'$\sigma_{HH}$ (pb)', fontsize=25)
plt.grid(True, linestyle='--', alpha=0.6)
plt.xticks(fontsize=25)
plt.yticks(fontsize=25)
plt.tight_layout()
plt.show() 
