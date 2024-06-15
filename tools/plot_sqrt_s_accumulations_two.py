import uproot
import numpy as np
import matplotlib.pyplot as plt

def compute_sqrt_s(E1, px1, py1, pz1, E2, px2, py2, pz2):
    E_total = E1 + E2
    px_total = px1 + px2
    py_total = py1 + py2
    pz_total = pz1 + pz2
    sqrt_s = np.sqrt(E_total**2 - (px_total**2 + py_total**2 + pz_total**2))
    return sqrt_s

def plot_sqrt_s(filenames):
    for filename in filenames:
        file = uproot.open(filename)
        tree = file["Delphes"] 
        
        Particle_size = tree["Particle_size"].array()
        Status = tree["Particle.Status"].array()
        E = tree["Particle.E"].array()
        Px = tree["Particle.Px"].array()
        Py = tree["Particle.Py"].array()
        Pz = tree["Particle.Pz"].array()
        
        sqrt_s_values = []
        for i in range(len(Particle_size)):
            E1 = px1 = py1 = pz1 = 0
            E2 = px2 = py2 = pz2 = 0
            found_first = found_second = False
            
            for j in range(Particle_size[i]):
                if Status[i][j] == 21:  
                    if not found_first:
                        E1 = E[i][j]
                        px1 = Px[i][j]
                        py1 = Py[i][j]
                        pz1 = Pz[i][j]
                        found_first = True
                    else:
                        E2 = E[i][j]
                        px2 = Px[i][j]
                        py2 = Py[i][j]
                        pz2 = Pz[i][j]
                        found_second = True
                        break
            
            if found_first and found_second:
                sqrt_s = compute_sqrt_s(E1, px1, py1, pz1, E2, px2, py2, pz2)
                sqrt_s_values.append(sqrt_s)
        
        sqrt_s_values = np.array(sqrt_s_values)
        sqrt_s_values.sort()
        normalized_cdf = np.arange(1, len(sqrt_s_values)+1) / len(sqrt_s_values)
        base_filename = os.path.basename(filename)
        plt.figure(figsize=(10, 6))
        plt.plot(sqrt_s_values, normalized_cdf, label=filename)
        
        percentiles = [0.2, 0.4, 0.6, 0.8, 1.0]
        for percentile in percentiles:
            index = int(percentile * len(sqrt_s_values)) - 1
            plt.scatter(sqrt_s_values[index], normalized_cdf[index], marker='o')
            plt.text(sqrt_s_values[index], normalized_cdf[index], f'{percentile*100:.0f}% ({sqrt_s_values[index]:.2f} GeV)', fontsize=12, ha='left', va='center')

        plt.xlim(0, 1500) 
        plt.xlabel('sqrt(s) [GeV]')
        plt.ylabel('Cumulative Distribution')
        plt.title(f'Cumulative sqrt(s) Distribution for {filename}')
        plt.legend()
        output_filename_cdf = filename.replace('.root', '_sqrt_s_cdf_distribution.png')
        plt.savefig(output_filename_cdf)
        plt.close()

        plt.figure(figsize=(10, 6))
        plt.hist(sqrt_s_values, bins=100, range=(0, 1500), histtype='step', label=filename)
        
        for percentile in percentiles:
            index = int(percentile * len(sqrt_s_values)) - 1
            plt.axvline(x=sqrt_s_values[index], color='r', linestyle='--')

        plt.xlim(0, 1500) 
        plt.xlabel('sqrt(s) [GeV]')
        plt.ylabel('Counts')
        plt.title(f'sqrt(s) Distribution for {filename}')
        plt.legend()
        output_filename_hist = filename.replace('.root', '_sqrt_s_hist_distribution.png')
        plt.savefig(output_filename_hist)
        plt.close()

if __name__ == "__main__":
    import sys
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <root file 1> <root file 2> ...")
        sys.exit(1)
    
    filenames = sys.argv[1:]
    plot_sqrt_s(filenames)

