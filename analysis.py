import sys
import os
import numpy as np
from sklearn.metrics import silhouette_score
from typing import List, Tuple

from kmeans_hw1 import calc_kmeans, process_input_file
from symnmf_py import calc_symnmf

def parse_args(args: List[str]) -> Tuple[int, str, str]:
    if len(args) != 2:
        print("Usage: python symnmf.py k filename")
        # print("An Error Has Occurred")
        sys.exit(1)
        
    k = int(args[0])
    filename = args[1] # should be a .txt existing file
    
    if k <= 0:
        print(f"Invalid k: {k}")
        sys.exit(1)
        
    if not os.path.exists(filename) or not filename.endswith(".txt"):
        print(f"Bad file: {filename}")
        # print("An Error Has Occurred")
        sys.exit(1)
        
    return k, filename

def calc_kmeans_labels(points:List[np.ndarray] , centroids:np.ndarray) -> np.ndarray:
    points = np.array(points)
    diff = points[:, np.newaxis, :] - centroids[np.newaxis, :, :]
    dists = np.linalg.norm(diff, axis=2)
    labels = np.argmin(dists, axis=1)
    return labels

def calc_symnmf_labels(points:List[np.ndarray] , symnmf_result:np.ndarray) -> np.ndarray:
    labels = np.argmax(symnmf_result, axis=1)
    return labels
        
if __name__ == "__main__":
    np.random.seed(1234)
    
    args = sys.argv[1:]
    k, filename = parse_args(args)
    points = process_input_file(filename)
    
    kmeans_centroids = calc_kmeans(k, filename)
    kmeans_labels = calc_kmeans_labels(points, kmeans_centroids)
    kmeans_score = silhouette_score(points, kmeans_labels)
    print("done kmeans")
    
    symnmf_result = calc_symnmf(k, filename)
    print("calculated symnmf")    
    symnmf_labels = calc_symnmf_labels(points, symnmf_result)
    symnmf_score = silhouette_score(points, symnmf_labels)
    
    print(f"nmf: {symnmf_score:.4f}")
    print(f"kmeans: {kmeans_score:.4f}")
    
    
    