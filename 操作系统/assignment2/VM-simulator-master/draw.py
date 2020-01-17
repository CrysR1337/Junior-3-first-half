import matplotlib.pyplot as plt 

res = []
with open('addresses-locality.txt', 'r', encoding='utf-8') as f:
    lines = f.readlines()
    for line in lines:
        res.append(int(line.split('\n')[0]))

plt.scatter(range(len(res)), res)
plt.show()