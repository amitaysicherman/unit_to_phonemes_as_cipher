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


phonemes_bi_grams=pd.read_csv("texts/phonemes_bi_grams.csv",header=None).values.astype(int)
phonemes = get_uncode_values("phonemes")
phonemes_uniqe=np.unique(phonemes)

for s in [50,100,200,500,1000]:
    print("--------------------")
    print(f"--------{s}---------")
    print("--------------------")
    
    code = get_uncode_values(f"code{s}")
    code_uniqe=np.unique(code)
    superv_map=[""]*len(code_uniqe)
    superv_count=[0]*len(phonemes_uniqe)
    for c in code_uniqe:
        phonemes_in_code=phonemes[code==c]
        max_phonemes=np.unique(phonemes_in_code,return_counts=True)[0][np.argmax(np.unique(phonemes_in_code,return_counts=True)[1])]
        superv_map[c]=str(max_phonemes)
        superv_count[max_phonemes]+=1
    with open(f"texts/superv_map_{s}.txt","w") as f:
        f.write(",".join(superv_map))

    with open(f"texts/superv_count_{s}.txt","w") as f:
        f.write(",".join([str(x) for x in superv_count]))


    code_bi_grams=pd.read_csv(f"texts/code{s}_bi_grams.csv",header=None).values.astype(int)
    
    code_to_phonemes=np.zeros_like(phonemes_bi_grams)
    for i in range(len(code_uniqe)):
        for j in range(len(code_bi_grams)):
            code_to_phonemes[int(superv_map[i]),int(superv_map[j])]+=code_bi_grams[i,j]
    print("Superv score",np.abs(code_to_phonemes-phonemes_bi_grams).sum())

    
    full_res=np.zeros((phonemes_uniqe.size,code_uniqe.size))
    correct=0
    for p,c in zip(phonemes,code):
        full_res[p,c]+=1
        if int(superv_map[c])==p:
            correct+=1
    print("Superv accuracy",1-correct/len(phonemes))

    pd.DataFrame(full_res).astype(int).astype(str).to_csv(f'texts/code{s}_superv.csv',header=False,index=False)

    print(full_res.sum())