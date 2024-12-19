import re
import numpy as np
import argparse
import matplotlib.pyplot as plt

sigma_sm_di_higgs = 3.4896e+01
BR_hh_4b = 0.58 * 0.58
L = 3000
sigma_b = np.array([3.1318e+05, 9.370e+01, 5.996e+03, 2.506e+01, 3.952e+06, 7.371e+08, 2.135e+02, 4.647e+01])
def calculate_with_confusion_matrix(confusion_matrix, cl_value):
    ps = confusion_matrix[0, 0]
    pb = confusion_matrix[1:, 0]

    numerator = ps * sigma_sm_di_higgs * BR_hh_4b * L + np.sum(pb * sigma_b * L)

    denominator = ps * BR_hh_4b * L * sigma_sm_di_higgs

    result = (np.sqrt(cl_value * numerator) / denominator) * 100

    return result

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

def extract_loss(log_file):
    train_metrics = []
    val_metrics = []

    with open(log_file, 'r') as file:
        lines = file.readlines()

    for line in lines:
        if "Epoch" in line and "training" in line:
            epoch_number = re.search(r'Epoch #(\d+)', line)
            if epoch_number:
                epoch_num = epoch_number.group(1)

                for next_line in lines[lines.index(line):]:
                    if "Train AvgLoss" in next_line:
                        avg_loss = re.search(r'Train AvgLoss: ([\d.]+)', next_line)
                        if avg_loss:
                            train_metrics.append({
                                'epoch': int(epoch_num),
                                'AvgLoss': float(avg_loss.group(1))
                            })
                            break

        if "Epoch" in line and "validating" in line:
            epoch_number = re.search(r'Epoch #(\d+)', line)
            if epoch_number:
                epoch_num = epoch_number.group(1)

                for next_line in lines[lines.index(line):]:
                    if "AvgLoss" in next_line:
                        avg_loss = re.search(r'AvgLoss ([\d.]+)', next_line)
                        if avg_loss:
                            val_metrics.append({
                                'epoch': int(epoch_num),
                                'AvgLoss': float(avg_loss.group(1))
                            })
                            break

    return train_metrics, val_metrics

def plot_results(results_95, results_68, accuracies, train_loss, val_loss):
    epochs = list(range(1, len(results_95) + 1))  

    fig, axs = plt.subplots(3, 1, figsize=(10, 15))

    axs[0].plot(epochs, results_95, marker='o', linestyle='-', color='b', label='95% CL')
    axs[0].plot(epochs, results_68, marker='s', linestyle='--', color='r', label='68% CL')
    axs[0].set_xlabel('Epoch')  
    axs[0].set_ylabel('Relative Error (%)') 
    axs[0].set_title('Relative Error over Epochs')
    axs[0].legend(loc='upper right') 
    axs[0].grid(True)
    axs[0].set_xticks(epochs[::5])  

    train_epochs = [m['epoch'] + 1 for m in train_loss]
    train_avg_loss = [m['AvgLoss'] for m in train_loss]
    val_epochs = [m['epoch'] + 1 for m in val_loss]
    val_avg_loss = [m['AvgLoss'] for m in val_loss]
    
    axs[1].plot(train_epochs, train_avg_loss, marker='o', linestyle='-', color='g', label='Train Loss')
    axs[1].plot(val_epochs, val_avg_loss, marker='s', linestyle='--', color='m', label='Validation Loss')
    axs[1].set_xlabel('Epoch')  
    axs[1].set_ylabel('Loss') 
    axs[1].set_title('Training and Validation Loss over Epochs')
    axs[1].legend(loc='upper right')  
    axs[1].grid(True)
    axs[1].set_xticks(range(1, max(train_epochs) + 1, 5))  

    axs[2].plot(epochs, accuracies, marker='o', linestyle='-', color='g', label='Accuracy (label_hh_4b)')
    axs[2].set_xlabel('Epoch')  
    axs[2].set_ylabel('Accuracy')  
    axs[2].set_title('Accuracy of label_hh_4b over Epochs')
    axs[2].legend(loc='upper right') 
    axs[2].grid(True)
    axs[2].set_xticks(epochs[::5]) 

    plt.tight_layout()
    plt.show()

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Extract confusion matrices and metrics from a log file and perform calculations.")

    parser.add_argument('log_file_path', type=str, help='Path to the log file containing the confusion matrices')

    args = parser.parse_args()

    confusion_matrices = extract_confusion_matrices(args.log_file_path)

    train_loss, val_loss = extract_loss(args.log_file_path)

    results_95 = []  
    results_68 = []  
    accuracies = [] 

    for i, matrix in enumerate(confusion_matrices):
        result_95 = calculate_with_confusion_matrix(matrix, 3.841) 
        result_68 = calculate_with_confusion_matrix(matrix, 1.0)   
        accuracy = matrix[0, 0] 
        
        results_95.append(result_95)
        results_68.append(result_68)
        accuracies.append(accuracy)
        
        print(f"Result for Confusion Matrix {i+1} (95% CL): {result_95}%")
        print(f"Result for Confusion Matrix {i+1} (68% CL): {result_68}%")
        print(f"Accuracy for Confusion Matrix {i+1}: {accuracy}")

    plot_results(results_95, results_68, accuracies, train_loss, val_loss)

