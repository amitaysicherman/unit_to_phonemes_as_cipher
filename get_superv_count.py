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
phonemes_uniqe=np.unique(phonemes)

code = get_uncode_values("code")
code_uniqe=np.unique(code)
print(code_uniqe)
print(len(code_uniqe))
superv_map=[""]*len(code_uniqe)
superv_count=[0]*len(phonemes_uniqe)
print(len(superv_map))
for c in code_uniqe:
    phonemes_in_code=phonemes[code==c]
    max_phonemes=np.unique(phonemes_in_code,return_counts=True)[0][np.argmax(np.unique(phonemes_in_code,return_counts=True)[1])]
    superv_map[c]=str(max_phonemes)
    superv_count[max_phonemes]+=1
print(",".join(superv_map))
print(len(",".join(superv_map).split(",")))
with open("texts/superv_map.txt","w") as f:
    f.write(",".join(superv_map))

with open("texts/super_count.txt","w") as f:
    f.write(",".join([str(x) for x in superv_count]))



phonemes_bi_grams=pd.read_csv("texts/phonemes_bi_grams.csv",header=None).values.astype(int)
code_bi_grams=pd.read_csv("texts/code_bi_grams.csv",header=None).values.astype(int)

code_to_phonemes=np.zeros_like(phonemes_bi_grams)
for i in range(len(code_uniqe)):
    for j in range(len(code_bi_grams)):
        code_to_phonemes[int(superv_map[i]),int(superv_map[j])]+=code_bi_grams[i,j]
print("Superv score",np.abs(code_to_phonemes-phonemes_bi_grams).sum())

correct=0
for p,c in zip(phonemes,code):
    if int(superv_map[c])==p:
        correct+=1
print("Superv accuracy",1-correct/len(phonemes))