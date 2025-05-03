import os
import numpy as np
import numpy.typing as npt
from typing import List
import sys

EPSILON = 0.001


def process_input_file(filepath: str) -> List[npt.NDArray]:
    """
    Processes an input file containing vectors.
    Each line in the file should contain the same amount comma-separated values.

    Args:
        filepath (str): The path to the input file.
    Returns:
        list: A list of numpy arrays, each representing a vector from the file.
    """

    points = []
    with open(filepath, "r") as file:
        for line in file:
            point = np.fromstring(line.strip(), sep=",")
            if len(points) == 0:
                dimension = point.size
            elif point.size != dimension:
                print("An Error Has Occurred.")
                exit(1)
            points.append(point)
    return points


def print_centroids(centroids: npt.NDArray) -> None:
    """
    Print the final cluster assignments.

    Args:
        centroids (npt.NDArray): The final centroids.
    Returns:
        None
    """
    for centroid in centroids:
        print(",".join(f"{coord:.4f}" for coord in centroid))


def assign_clusters(points: List[npt.NDArray], centroids: List[npt.NDArray]) -> List[int]:
    """
    Assign each point to the nearest centroid.

    Args:
        points (list): A list of numpy arrays, each representing a vector.
        centroids (list): A list of numpy arrays, each representing a centroid.
    Returns:
        list: A list of integers, each representing the index of the nearest centroid.
    """
    clusters = [[] for _ in range(len(centroids))]
    for point in points:
        distances = [np.linalg.norm(point - centroid) for centroid in centroids]
        cluster = distances.index(min(distances))
        clusters[cluster].append(point)

    return clusters


def calculate_new_centroids(clusters: List[List[npt.NDArray]]) -> List[npt.NDArray]:
    """
    Calculate the new centroids based on the points in each cluster.

    Args:
        clusters (list): A list of lists, each containing the points in a cluster.
    Returns:
        list: A list of numpy arrays, each representing a new centroid.
    """
    return [np.mean(cluster, axis=0) for cluster in clusters]


def calc_kmeans(
    K: int,
    input_data: str,
    iter: int = 200,
    eps=EPSILON,
    initial_centroids=None,
    input_points=None,
) -> npt.NDArray:
    """
    Perform K-means clustering on the given input data.
    Returns the final cluster assignments.

    Parameters:
    K (int): The number of clusters to form.
    iter (int): The number of iterations to run the algorithm.
    input_data (str): The path to the input data file.

    Returns:
    None
    """
    points = process_input_file(input_data) if input_points is None else input_points
    if not (1 < K < len(points)):
        # print("Invalid number of clusters!")
        print("An Error Has Occurred.")
        exit(1)
    if not (1 < iter < 1000):
        # print("Invalid maximum iteration!")
        print("An Error Has Occurred.")
        exit(1)

    # Initializations
    centroids = (
        np.array(points[:K])
        if initial_centroids is None
        else np.array(initial_centroids)
    )
    i = 0
    has_converged = False

    # Main loop
    while i < iter and not has_converged:
        i += 1

        clusters = assign_clusters(points, centroids)
        new_centroids = calculate_new_centroids(clusters)

        centroid_difference = np.linalg.norm(
            np.array(new_centroids) - np.array(centroids), axis=1
        )
        is_converged_vec = centroid_difference < eps
        if is_converged_vec.all():
            has_converged = True

        centroids = np.array(new_centroids)

    return centroids


def fit(initial_centroids, points, k, eps, iter):
    return calc_kmeans(k, points, iter, eps, initial_centroids, points)


def kmeans_main() -> npt.NDArray:
    if not len(sys.argv) in [3, 4]:
        # print("Usage: python kmeans.py <K> [<iter>] <input_data>")
        print("An Error Has Occurred.")
        sys.exit(1)

    try:
        K = int(sys.argv[1])
    except ValueError as e:
        # print("Invalid number of clusters!")
        print("An Error Has Occurred.")
        exit(1)

    filename = str(sys.argv[-1])
    if not os.path.exists(filename):
        print("An Error Has Occurred.")
        exit(1)

    if len(sys.argv) == 3:
        return calc_kmeans(K, filename)

    elif len(sys.argv) == 4:
        try:
            iter = int(sys.argv[2])
        except ValueError as e:
            # print("Invalid maximum iteration!")
            print("An Error Has Occurred.")
            exit(1)

        return calc_kmeans(K, filename, iter)


if __name__ == "__main__":
    print(kmeans_main())
