import sys
import os
import numpy as np
import numpy.typing as npt
from sklearn.metrics import silhouette_score
from typing import List, Tuple

from kmeans_hw1 import calc_kmeans, process_input_file
from calc_symnmf import calc_symnmf


def parse_args(args: List[str]) -> Tuple[int, str, str]:
    """
    Parses command line arguments.
    Args:
        args (list): A list of command line arguments.
    Returns:
        tuple: A tuple containing the number of clusters (k) and the filename.
    """
    if len(args) != 2:
        # print("Usage: python symnmf.py k filename")
        print("An Error Has Occurred")
        sys.exit(1)

    k = int(args[0])
    filename = args[1]  # assuming a .txt existing file

    if k <= 0:
        # print(f"Invalid k: {k}")
        print("An Error Has Occurred")
        sys.exit(1)

    if not os.path.exists(filename) or not filename.endswith(".txt"):
        # print(f"Bad file: {filename}")
        print("An Error Has Occurred")
        sys.exit(1)

    return k, filename


def calc_kmeans_labels(
    points: List[npt.NDArray], centroids: npt.NDArray
) -> npt.NDArray:
    """
    Assigns labels to points based on the closest centroid.
    Args:
        points (list): A list of points.
        centroids (ndarray): An array of centroids.
    Returns:
        ndarray: An array of labels for each point.
    """
    points = np.array(points)
    diff = points[:, np.newaxis, :] - centroids[np.newaxis, :, :]
    dists = np.linalg.norm(diff, axis=2)
    labels = np.argmin(dists, axis=1)
    return labels


def calc_symnmf_labels(
    points: List[npt.NDArray], symnmf_result: npt.NDArray
) -> npt.NDArray:
    """
    Assigns labels to points based on the associasion matrix H (the symnmf's result).
    Args:
        points (list): A list of points.
        symnmf_result (ndarray): the H resulted by symnmf.
    Returns:
        ndarray: An array of labels for the points.
    """
    labels = np.argmax(symnmf_result, axis=1)
    return labels


if __name__ == "__main__":
    """
    Compare the results of kmeans and symnmf.
    """
    np.random.seed(1234)

    args = sys.argv[1:]
    k, filename = parse_args(args)
    points = process_input_file(filename)

    kmeans_centroids = calc_kmeans(k, filename)
    kmeans_labels = calc_kmeans_labels(points, kmeans_centroids)
    kmeans_score = silhouette_score(points, kmeans_labels)

    symnmf_result = calc_symnmf(k, filename)
    symnmf_labels = calc_symnmf_labels(points, symnmf_result)
    symnmf_score = silhouette_score(points, symnmf_labels)

    print(f"nmf: {symnmf_score:.4f}")
    print(f"kmeans: {kmeans_score:.4f}")
