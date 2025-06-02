import uproot
import numpy as np
import matplotlib.pyplot as plt
from sklearn.metrics import roc_curve, auc

file = uproot.open("kappa-no1-mini-epo27-alltest-output.root")  
tree = file["Events"]  

data = tree.arrays(library="np")

classes = [
    "hh_4b", "tt_2b4j", "tth_4b4j", "ttbb_4b4j",
    "hbb_4b", "4b", "2b2j", "zz_4b", "zh_4b"
]

label_map = {
    "hh_4b": r"$HH$",
    "4b": r"$4b$",
    "2b2j": r"$2b2j$",
    "tt_2b4j": r"$tt$",
    "ttbb_4b4j": r"$ttbb$",
    "tth_4b4j": r"$ttH$",
    "hbb_4b": r"$bbH$",
    "zz_4b": r"$ZZ$",
    "zh_4b": r"$ZH$"
}

true_labels = {}
scores = {}

for class_name in classes:
    true_labels[class_name] = data[f"label_{class_name}"]
    scores[class_name] = data[f"score_label_{class_name}"]

plt.rcParams.update({
    'font.size': 18,
    'axes.titlesize': 18,
    'axes.labelsize': 18,
    'xtick.labelsize': 25,
    'ytick.labelsize': 25,
    'legend.fontsize': 18,
})

plt.figure(figsize=(10, 10))

for class_name in classes:
    fpr, tpr, _ = roc_curve(true_labels[class_name], scores[class_name])
    roc_auc = auc(fpr, tpr)

    display_name = label_map.get(class_name, class_name)
    plt.plot(fpr, tpr, lw=2, label=f'{display_name} (AUC = {roc_auc:.2f})')

plt.plot([0, 1], [0, 1], 'k--', lw=2)
plt.xlim([0.0, 1.0])
plt.ylim([0.0, 1.05])
plt.xlabel('FPR', fontsize=25)
plt.ylabel('TPR', fontsize=25)
plt.legend(loc="lower right", fontsize=18)
plt.tight_layout()
plt.show()

