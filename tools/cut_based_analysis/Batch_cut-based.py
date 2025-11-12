import os
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor, as_completed

def process_root_file(file_path, cut_based_program):
    """
    Process a single ROOT file using the cut_based program.

    :param file_path: Path to the .root file.
    :param cut_based_program: Path to the external program.
    :return: Directory and percentage if successful, None otherwise.
    """
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

def calculate_probabilities_parallel(base_dir, output_file, cut_based_program, num_threads=4):
    """
    Recursively traverses directories, processes ROOT files in parallel, calculates average probabilities, and saves results.

    :param base_dir: Root directory containing subdirectories and ROOT files.
    :param output_file: Path to the text file where results will be saved.
    :param cut_based_program: Path to the external program to process ROOT files.
    :param num_threads: Number of threads to use for parallel processing.
    """
    root_files = []

    # Collect all .root files
    for root, _, files in os.walk(base_dir):
        for file in files:
            if file.endswith('.root'):
                root_files.append(os.path.join(root, file))

    results = {}

    # Use ThreadPoolExecutor for parallel processing
    with ThreadPoolExecutor(max_workers=num_threads) as executor:
        future_to_file = {
            executor.submit(process_root_file, file_path, cut_based_program): file_path
            for file_path in root_files
        }

        # Process results as they complete
        for future in as_completed(future_to_file):
            result = future.result()
            if result:
                directory, probability = result
                if directory not in results:
                    results[directory] = []
                results[directory].append(probability)

    # Write results to output file
    with open(output_file, 'w') as out_file:
        for directory, probabilities in results.items():
            avg_probability = sum(probabilities) / len(probabilities)
            out_file.write(f"{directory}: {avg_probability:.2f}%\n")
            print(f"Directory: {directory}, Average Probability: {avg_probability:.2f}%")

if __name__ == "__main__":
    if len(sys.argv) != 5:
        print("Usage: python3 calculate_probabilities.py <base_dir> <output_file> <cut_based_program> <num_threads>")
        sys.exit(1)

    base_dir = sys.argv[1]
    output_file = sys.argv[2]
    cut_based_program = sys.argv[3]
    num_threads = int(sys.argv[4])

    calculate_probabilities_parallel(base_dir, output_file, cut_based_program, num_threads)

