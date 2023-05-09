import numpy as np
import pandas as pd
norm_to=10_000

def prep_data(name):
    with open(f'texts/{name}.txt') as f:
        lines = f.read().splitlines()
    lines=" ## ".join(lines)
    all_text=lines.split()
    print(len(all_text))
    chars,counts=np.unique(all_text,return_counts=True)
    chars=chars[np.argsort(counts)[::-1]]
    counts=counts[np.argsort(counts)[::-1]]
    print(chars)
    print(counts)
    with open(f'texts/{name}_sorted.txt','w') as f:
        f.write("\n".join(chars))
    counts=((counts/np.sum(counts))*norm_to).round().astype(int)
    with open(f'texts/{name}_counts.txt','w') as f:
        f.write(",".join(counts.astype(str)))

    bi_grams=pd.DataFrame(columns=chars,index=chars)
    bi_grams=bi_grams.fillna(0)
    for i in range(1,len(all_text)):
        bi_grams.loc[all_text[i-1],all_text[i]]+=1
    bi_grams=((bi_grams/np.sum(bi_grams.values))*norm_to).round().astype(int)
    print(bi_grams.shape,np.sum(bi_grams.values))
    
    bi_grams.astype(str).to_csv(f"texts/{name}_bi_grams.csv",index=False,header=False)
    return counts
p_counts=prep_data("phonemes")
c_counts=prep_data("code")


p_counts = np.round(p_counts/p_counts.sum() * c_counts.shape[0])
p_counts = np.clip(p_counts, 1, None)
diff = c_counts.shape[0] - p_counts.sum()
if diff < 0:
    p_counts[np.argsort(p_counts)[::-1][:int(-diff)]] -= 1
else:
    p_counts[np.argsort(p_counts)[::-1][:int(diff)]] += 1
p_counts = p_counts.astype(int)
assert p_counts.sum() == c_counts.shape[0]
with open("texts/freq_count.txt", "w") as f:
    f.write(",".join(p_counts.astype(str)))

