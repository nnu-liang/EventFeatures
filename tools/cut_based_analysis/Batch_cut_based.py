import os
import subprocess

def calculate_probabilities(base_dir, output_file, cut_based_program):

    with open(output_file, 'w') as out_file:
        for root, dirs, files in os.walk(base_dir):
            probabilities = []  
            for file in files:
                if file.endswith('.root'):  
                    file_path = os.path.join(root, file)
                    try:
                        result = subprocess.run(
                            [cut_based_program, file_path],
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE,
                            text=True
                        )
                        output = result.stdout
                        if "Percentage of qualified entries:" in output:
                            percentage = float(output.split("Percentage of qualified entries:")[1].strip().replace('%', ''))
                            probabilities.append(percentage)
                    except Exception as e:
                        print(f"Error processing file {file_path}: {e}")
            
            if probabilities:
                avg_probability = sum(probabilities) / len(probabilities)
                out_file.write(f"{root}: {avg_probability:.2f}%\n")
                print(f"Directory: {root}, Average Probability: {avg_probability:.2f}%")

