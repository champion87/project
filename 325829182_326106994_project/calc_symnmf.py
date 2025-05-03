"""
This module provides the functionality to perform various calculations related to the SYMNMF algorithm.
"""

import numpy as np
from numpy.typing import NDArray
from enum import Enum
import symnmf


class Goal(Enum):
    SYMNMF = "symnmf"
    DDG = "ddg"
    NORM = "norm"
    SYM = "sym"


VALID_GOALS = [goal.value for goal in Goal]

DEFAULT_EPSILON = 1e-4
DEFAULT_MAX_ITER = 300
DEFAULT_BETA = 0.5


def init_H(k: int, W: NDArray) -> NDArray:
    """
    Initialize the H matrix for the SYMNMF algorithm.
    Args:
                k (int): The number of clusters.
                W (ndarray): The input matrix.
    Returns:
                ndarray: The initialized H matrix.
    """
    # assert len(W.shape) == 2 # TODO remove assertions before submission
    # assert W.shape[0] == W.shape[1]
    mean_W = np.mean(W)
    n = W.shape[0]
    H_init = np.random.uniform(0, 2 * ((mean_W / k) ** 0.5), (n, k))
    return H_init


def calc_symnmf(k: int, filename: str, goal: Goal = Goal.SYMNMF) -> NDArray:
    """
    Perform symnmf tasks on the given data file: sym, ddg, norm calculations, or the full symnmf algorithm.
        Args:
                k (int): The number of clusters.
                filename (str): The path to the input data file.
                goal (Goal): The goal for the symnmf algorithm: sym, ddg, norm, or symnmf for the full symnmf algorithm.
        Returns:
                ndarray: The result of the requested calculation.
    """
    X = np.loadtxt(filename, delimiter=",", ndmin=2)
    datapoints = X.tolist()
    if goal == Goal.SYM:
        res = A = symnmf.sym(datapoints)
    elif goal == Goal.DDG:
        res = D = symnmf.ddg(datapoints)
    elif goal == Goal.NORM:
        res = W = symnmf.norm(datapoints)
    elif goal == Goal.SYMNMF:
        W = symnmf.norm(datapoints)
        W_np = np.array(W)
        H_init = init_H(k, W_np).tolist()
        res = H_final = symnmf.symnmf(
            H_init, W, DEFAULT_EPSILON, DEFAULT_MAX_ITER, DEFAULT_BETA
        )

    return np.array(res)
