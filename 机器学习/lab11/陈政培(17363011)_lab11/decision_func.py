from sklearn.datasets import load_iris
from sklearn.preprocessing import MinMaxScaler
from sklearn.model_selection import train_test_split
from sklearn.model_selection import GridSearchCV
from sklearn.svm import SVC
import matplotlib.pyplot as plt 
import numpy as np

iris = load_iris().data 
iris_target = load_iris().target

MinMax = MinMaxScaler()
MinMax.fit(iris)
iris_transf = MinMax.transform(iris)

X_train, X_test, y_train, y_test = train_test_split(iris_transf, iris_target, random_state=14)

# from sklearn.datasets import make_blobs
# X_train, y_train = make_blobs(n_samples=50, centers=2, random_state=6)

from sklearn.decomposition import PCA
pca = PCA(n_components=2)
pca.fit(X_train)
X_train = pca.transform(X_train)

svc = SVC(C=1, kernel='linear')
svc.fit(X_train, y_train)
# print("test_svc.score: {:.3f}".format(svc.score(X_test, y_test)))
# print("train_svc.score: {:.3f}".format(svc.score(X_train, y_train)))

# plt.scatter(X_train[:, 0], X_train[:, 1], c=y_train, s=30, cmap=plt.cm.Paired)
# ax = plt.gca()
# xlim = ax.get_xlim()
# ylim = ax.get_ylim()

# xx = np.linspace(xlim[0], xlim[1], X_train.shape[0])
# yy = np.linspace(ylim[0], ylim[1], y_train.shape[0])
# YY, XX = np.meshgrid(yy, xx)

# xy = np.vstack([XX.ravel(), YY.ravel()]).T

# Z = svc.decision_function(xy).reshape(XX.shape)

# ax.contour(XX, YY, Z, colors='k', levels=[-1, 0, 1], alpha=0.5, linestyles=['--','-','--'])
# ax.scatter(svc.support_vectors_[:, 0], svc.support_vectors_[:, 1], s=100, linewidth=1, c='g', facecolors='none')
# plt.show()

print(X_train.shape)
x_min, x_max = X_train[:, 0].min() - .5, X_train[:, 0].max() + .5
y_min, y_max = X_train[:, 1].min() - .5, X_train[:, 1].max() + .5
h = 0.01
xx, yy = np.meshgrid(np.arange(x_min, x_max, h), np.arange(y_min, y_max, h))
print(xx.shape)
print(yy.shape)
xy = np.vstack([xx.ravel(), yy.ravel()]).T
print(xy.shape)

Z = svc.decision_function(xy)
print(Z.shape)
Z1 = Z[:, 0].reshape(xx.shape)
Z2 = Z[:, 1].reshape(xx.shape)
Z3 = Z[:, 2].reshape(xx.shape)
# # 用预测函数预测一下
# Z = pred_func(np.c_[xx.ravel(), yy.ravel()])
# Z = Z.reshape(xx.shape)

# 然后画出图
plt.contour(xx, yy, Z1, cmap=plt.cm.Spectral, linestyles=['--','-','--'])
plt.contour(xx, yy, Z2, cmap=plt.cm.Spectral, linestyles=['--','-','--'])
plt.contour(xx, yy, Z3, cmap=plt.cm.Spectral, linestyles=['--','-','--'])
plt.scatter(X_train[:, 0], X_train[:, 1], c=y_train, cmap=plt.cm.Spectral)
plt.show()