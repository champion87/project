from typing import List, Tuple
import sys
from enum import Enum
import os
import numpy as np
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

# def c_symnmf(
#     initial_H: List[List[float]],
#     W: List[List[float]],
#     epsilon: float = 1e-4,
#     max_iter: int = 300,
#     beta: float = 0.5,
# ) -> List[List[float]]:
#     pass


# def c_sym(datapoints: List[List[float]]) -> List[List[float]]:
#     pass


# def c_ddg(datapoints: List[List[float]]) -> List[List[float]]:
#     pass


# def c_norm(datapoints: List[List[float]]) -> List[List[float]]:
#     pass


def parse_args(args: List[str]) -> Tuple[int, str, str]:
    if len(args) != 3:
        print("Usage: python symnmf.py k goal filename")
        # print("An Error Has Occurred")
        sys.exit(1)
        
    k = int(args[0])
    goal = args[1]
    filename = args[2] # should be a .txt existing file

    if goal not in VALID_GOALS:
        print(f"Invalid goal: {goal}")
        print(f"Valid goals: {VALID_GOALS}")
        # print("An Error Has Occurred") # TODO retrive these prints
        sys.exit(1)
    else:
        goal = Goal(goal)
        
    if k <= 0:
        print(f"Invalid k: {k}")
        sys.exit(1)

    if not os.path.exists(filename) or not filename.endswith(".txt"):
        print(f"Bad file: {filename}")
        # print("An Error Has Occurred")
        sys.exit(1)
        
    return k, goal, filename

def print_result(res: List[List[float]]) -> None:
    for row in res:
        print(",".join(f"{x:.4f}" for x in row))

def init_H(k: int, W: np.ndarray) -> np.ndarray:
    assert len(W.shape) == 2 # TODO remove assertions before submission
    assert W.shape[0] == W.shape[1]
    mean_W = np.mean(W)
    n = W.shape[0]
    H_init = np.random.uniform(0, 2*((mean_W/k)**0.5), (n, k))
    return H_init

if __name__ == "__main__":
    np.random.seed(1234)
    k, goal, filename = parse_args(sys.argv[1:])
    X = np.loadtxt(filename) # TODO can we do this?
    datapoints = X.tolist()
    
    
    if goal == Goal.SYM:
        res = A = symnmf.sym(datapoints)
    elif goal == Goal.DDG:
        res = D = symnmf.ddg(datapoints)
    elif goal == Goal.NORM:
        res = W = symnmf.norm(datapoints)
    elif goal == Goal.SYMNMF:
        W = symnmf.norm(datapoints) # TODO replace this with the c-python call
        W_np = np.array(W)
        H_init = init_H(k, W_np).tolist()
        res = H_final = symnmf.symnmf(H_init, W, DEFAULT_EPSILON, DEFAULT_MAX_ITER, DEFAULT_BETA)

    print_result(res)        
