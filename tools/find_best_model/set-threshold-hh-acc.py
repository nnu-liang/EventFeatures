import os
import subprocess
import argparse

def process_root_files(directory, output_txt, cpp_program, threshold):
    """
    Process all .root files in a given directory using a C++ program.

    Parameters:
    - directory: str, the directory containing .root files
    - output_txt: str, the path to the output text file
    - cpp_program: str, the path to the C++ executable
    - threshold: float, the threshold value to set
    """
    # Open the output file in write mode
    with open(output_txt, "w") as output_file:
        # Iterate through all files in the specified directory
        for filename in os.listdir(directory):
            if filename.endswith(".root"):
                file_path = os.path.join(directory, filename)

                # Construct the command to run the C++ program
                command = [cpp_program, file_path, str(threshold)]

                try:
                    # Execute the command and capture the output
                    result = subprocess.run(command, capture_output=True, text=True, check=True)

                    # Extract the relevant output (e.g., the line containing "hh_4b")
                    for line in result.stdout.splitlines():
                        if "hh_4b" in line:
                            output_file.write(f"{filename}: {line}\n")
                            break
                except subprocess.CalledProcessError as e:
                    print(f"Error processing file {file_path}: {e}")

if __name__ == "__main__":
    # Set up argument parser
    parser = argparse.ArgumentParser(description="Process .root files using a C++ program.")
    parser.add_argument("directory", type=str, help="Directory containing .root files")
    parser.add_argument("output_txt", type=str, help="Path to the output text file")
    parser.add_argument("cpp_program", type=str, help="Path to the C++ program")
    parser.add_argument("threshold", type=float, help="Threshold value to set")

    # Parse arguments
    args = parser.parse_args()

    # Process the .root files
    process_root_files(args.directory, args.output_txt, args.cpp_program, args.threshold)

