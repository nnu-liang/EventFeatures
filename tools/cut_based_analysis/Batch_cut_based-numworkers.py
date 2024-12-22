import os
import subprocess
import sys
from multiprocessing import Pool, Manager

def process_root_file(args):
    file_path, cut_based_program = args
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
            return os.path.dirname(file_path), percentage
    except Exception as e:
        print(f"Error processing file {file_path}: {e}")
    return None


def calculate_probabilities_parallel(base_dir, output_file, cut_based_program, num_workers):
    manager = Manager()
    results = manager.dict()

    root_files = []
    for root, _, files in os.walk(base_dir):
        for file in files:
            if file.endswith('.root'):
                root_files.append((os.path.join(root, file), cut_based_program))

    with Pool(processes=num_workers) as pool:
        for result in pool.imap_unordered(process_root_file, root_files):
            if result:
                directory, probability = result
                if directory not in results:
                    results[directory] = []
                results[directory].append(probability)

    with open(output_file, 'w') as out_file:
        for directory, probabilities in results.items():
            avg_probability = sum(probabilities) / len(probabilities)
            out_file.write(f"{directory}: {avg_probability:.2f}%\n")
            print(f"Directory: {directory}, Average Probability: {avg_probability:.2f}%")


if __name__ == "__main__":
    if len(sys.argv) != 5:
        print("Usage: python3 calculate_probabilities.py <base_dir> <output_file> <cut_based_program> <num_workers>")
        sys.exit(1)

    base_dir = sys.argv[1]
    output_file = sys.argv[2]
    cut_based_program = sys.argv[3]
    num_workers = int(sys.argv[4])

    calculate_probabilities_parallel(base_dir, output_file, cut_based_program, num_workers)

