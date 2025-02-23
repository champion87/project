import numpy as np
import numpy as np
import matplotlib.pyplot as plt

if __name__ == "__main__":
    K = 3
    centers = []
    datas = [np.random.normal(loc=center, scale=0.1, size=(30, 2)) for center in range(K)]
    
    X = np.concatenate(datas, axis=0)
    np.random.shuffle(X)
    np.savetxt("data.txt", X)
    
    

    plt.scatter(X[:, 0], X[:, 1], c='blue', marker='o', alpha=0.6)

    # Labels and title
    plt.xlabel("X-axis")
    plt.ylabel("Y-axis")
    plt.title("2D Scatter Plot of Points")
    plt.grid(True)

    # Show plot
    plt.savefig("scatter_plot.png", dpi=300, bbox_inches='tight')
