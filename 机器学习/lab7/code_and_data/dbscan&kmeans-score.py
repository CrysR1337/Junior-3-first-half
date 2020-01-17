import numpy as np
from sklearn.cluster import DBSCAN
from sklearn.cluster import KMeans
from sklearn import metrics
import seaborn as sns
import pandas as pd
from sklearn.datasets.samples_generator import make_blobs
from sklearn.preprocessing import StandardScaler
import matplotlib.pyplot as plt
import kmeans

data = kmeans.loadDataSet('Restaurant_Data_Beijing.txt')

data = pd.DataFrame(data)
data.columns=['x','y']
# sns.relplot(x='x',y='y',data=data)
# plt.show()

km = KMeans(n_clusters=3).fit(data)
data['labels'] = km.labels_
labels = km.labels_
# labels = np.array(labels).reshape(1,-1)
raito = data.loc[data['labels']==-1].x.count()/data.x.count() #labels=-1的个数除以总数，计算噪声点个数占总数的比例
print('KMeans:')
print('噪声比:', format(raito, '.2%'))
n_clusters_ = len(set(labels)) - (1 if -1 in labels else 0) # 获取分簇的数目
print('分簇的数目: %d' % n_clusters_)
print("轮廓系数: %0.3f" % metrics.silhouette_score(data, labels, metric='euclidean')) #轮廓系数评价KMeans的好坏
sns.relplot(x="x",y="y", hue="labels",data=data)

data = kmeans.loadDataSet('Restaurant_Data_Beijing.txt')

data = pd.DataFrame(data)
data.columns=['x','y']

db = DBSCAN(eps=0.2, min_samples=2).fit(data) #DBSCAN聚类方法 还有参数，matric = ""距离计算方法
data['labels'] = db.labels_ #和X同一个维度，labels对应索引序号的值 为她所在簇的序号。若簇编号为-1，表示为噪声，我们把标签放回到data数据集中方便画图
labels = db.labels_
raito = data.loc[data['labels']==-1].x.count()/data.x.count() #labels=-1的个数除以总数，计算噪声点个数占总数的比例
print('DBSCAN:')
print('噪声比:', format(raito, '.2%'))
n_clusters_ = len(set(labels)) - (1 if -1 in labels else 0) # 获取分簇的数目
print('分簇的数目: %d' % n_clusters_)
print("轮廓系数: %0.3f" % metrics.silhouette_score(data, labels)) #轮廓系数评价聚类的好坏
sns.relplot(x="x",y="y", hue="labels",data=data)
plt.show()