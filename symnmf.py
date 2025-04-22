from typing import List, Tuple
import sys
import os
import numpy as np
from symnmf import sym, ddg, norm
from calc_symnmf import calc_symnmf, Goal, VALID_GOALS


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



if __name__ == "__main__":    
    np.random.seed(1234)
    k, goal, filename = parse_args(sys.argv[1:])
    res = calc_symnmf(k, filename, goal)
    print_result(res)        
