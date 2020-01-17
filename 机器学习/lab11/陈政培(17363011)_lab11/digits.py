from sklearn.datasets import load_digits
from sklearn import svm
import matplotlib.pyplot as plt 

digits = load_digits()

svc = svm.SVC(gamma = 0.001,C = 100)

for i in range(1,11):
    plt.subplot(5,2,i)
    plt.imshow(digits.images[i],cmap = plt.cm.gray_r,interpolation = 'nearest')

svc.fit(digits.data[1:1790], digits.target[1:1790])

plt.show()

print("实际数字：",digits.target[:10])
print("预测数字：",svc.predict(digits.data[:10]))

print("实际数字：",digits.target[1791:])
print("预测数字：",svc.predict(digits.data[1791:]))