import re
import numpy as np
import argparse
import matplotlib.pyplot as plt
import seaborn as sns

def extract_confusion_matrices(log_file_path):
    with open(log_file_path, 'r') as f:
        log_data = f.readlines()

    confusion_matrices = []
    inside_matrix = False
    current_matrix = []
    current_row = []

    for line in log_data:
        if "- confusion_matrix:" in line:
            inside_matrix = True
            current_matrix = []
            current_row = []
            continue

        if inside_matrix:
            numbers = re.findall(r"[-+]?\d+\.\d+e[-+]?\d+|\d+\.\d+", line)
            if numbers:
                current_row.extend([float(num) for num in numbers])

            if len(current_row) >= 9:
                current_matrix.append(current_row[:9]) 
                current_row = current_row[9:] 

            if len(current_matrix) == 9:
                confusion_matrices.append(np.array(current_matrix))
                inside_matrix = False  
                current_matrix = []  

    return confusion_matrices

def plot_confusion_matrix(matrix, labels, epoch):
    plt.figure(figsize=(14, 12)) 

    def custom_formatter(x):
        return f'{x:.2e}' 

    vmin = np.min(matrix) 
    vmax = np.max(matrix) 

    sns.heatmap(matrix, annot=False, cmap='Blues', xticklabels=labels, yticklabels=labels, cbar=True,
                cbar_kws={"format": '%.2e'}, linewidths=0.5, linecolor='gray', vmin=vmin, vmax=vmax)

    cbar = plt.gcf().axes[-1]  
    cbar.tick_params(labelsize=16) 

    diag_values = [matrix[i, i] for i in range(len(matrix))]
    smallest_diag_indices = np.argsort(diag_values)[:3]

    for i in range(matrix.shape[0]):
        for j in range(matrix.shape[1]):
            if i == j:
                if i in smallest_diag_indices:  
                    color = 'black'
                else:
                    color = 'white'
            else:
                color = 'black'  
            plt.text(j + 0.5, i + 0.5, custom_formatter(matrix[i, j]),
                     horizontalalignment='center',
                     verticalalignment='center',
                     color=color,
                     fontsize=16)  
    plt.xlabel('Predicted', fontsize=18, labelpad=20)  
    plt.ylabel('True', fontsize=18, labelpad=20)
    plt.xticks(fontsize=16)
    plt.yticks(fontsize=16, rotation=45)  
    plt.tight_layout()

    plt.show()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Extract confusion matrices from a log file and plot the matrix for a specific epoch.")
    parser.add_argument('log_file_path', type=str, help='Path to the log file containing the confusion matrices')
    parser.add_argument('--epoch', type=int, help='The epoch number to plot the confusion matrix for')
    parser.add_argument('--last', action='store_true', help='Plot the last confusion matrix in the log file')
    args = parser.parse_args()
    log_file_path = args.log_file_path
    confusion_matrices = extract_confusion_matrices(log_file_path)
        if args.last:
        target_epoch = len(confusion_matrices)  
        selected_matrix = confusion_matrices[-1]
    elif args.epoch:
        if args.epoch > len(confusion_matrices) or args.epoch < 1:
            print(f"Error: The epoch {args.epoch} is out of range. Available epochs are from 1 to {len(confusion_matrices)}.")
            exit(1)
        else:
            target_epoch = args.epoch
            selected_matrix = confusion_matrices[target_epoch - 1] 
    else:
        print("Error: You must specify either --epoch or --last.")
        exit(1)

    labels = ['hh_4b', 'tt_2b4j', 'tth_4b4j', 'ttbb_4b4j', 'hbb_4b', '4b', '2b2j', 'zz_4b', 'zh_4b']

    plot_confusion_matrix(selected_matrix, labels, target_epoch)

