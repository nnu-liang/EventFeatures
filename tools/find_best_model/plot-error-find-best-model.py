import matplotlib.pyplot as plt
import sys

def plot_relative_errors(input_file, output_image):
    with open(input_file, 'r') as f:
        lines = f.readlines()
    
    x_epochs = [int(epoch) for epoch in lines[0].strip().split(':')[1].split()]
    y_relativeError95 = [float(val) for val in lines[1].strip().split(':')[1].split()]
    y_relativeError68 = [float(val) for val in lines[2].strip().split(':')[1].split()]
    
    plt.figure(figsize=(10, 6))
    plt.plot(x_epochs, y_relativeError95, label='95% CL Relative Error', marker='o', linestyle='-', linewidth=2)
    plt.plot(x_epochs, y_relativeError68, label='68% CL Relative Error', marker='s', linestyle='--', linewidth=2)
    
    plt.title("Relative Errors vs Epochs", fontsize=16)
    plt.xlabel("Epoch", fontsize=14)
    plt.ylabel("Relative Error", fontsize=14)
    
    plt.legend(fontsize=12)
    
    plt.grid(alpha=0.5)
    
    plt.savefig(output_image, dpi=300)
    print(f"Plot saved as {output_image}")
    
    plt.show()

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python plot_relative_errors.py <input_file> <output_image>")
        sys.exit(1)
    
    input_file = sys.argv[1]   
    output_image = sys.argv[2] 
    
    plot_relative_errors(input_file, output_image)

