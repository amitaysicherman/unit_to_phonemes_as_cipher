import os
import numpy as np
import pandas as pd


def get_uncode_values(name):
    """get_uncode_values [summary]"""
    with open(f"texts/{name}_sorted.txt", "r") as f:
        mapping = f.read().splitlines()
    mapping = {mapping[i]: i for i in range(len(mapping))}

    with open(f'texts/{name}.txt', "r") as f:
        symbols = f.read().splitlines()
    symbols = [mapping[p] for p in " ## ".join(symbols).split()]
    return np.array(symbols)




phonemes = get_uncode_values("phonemes")

base_scores="scores"
for file_name in os.listdir(base_scores):

    with open(f'{base_scores}/{file_name}') as f:
        mapping=f.read().strip().split(",")
    correct=0
    if file_name.startswith("best"):
        n=""
    else:
        n=int(file_name.split("_")[0])-1
    code = get_uncode_values(f"code{n}")

    for p,c in zip(phonemes,code):
        if int(mapping[c])==p:
            correct+=1
    print(file_name,' Phonemes Error:',1-correct/len(phonemes))