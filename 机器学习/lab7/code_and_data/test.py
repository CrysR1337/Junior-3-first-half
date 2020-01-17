import kmeans
from numpy import *

dataset = mat(kmeans.loadDataSet('testSet.txt'))

print(min(dataset[:,0]))
print(min(dataset[:,1]))
print(max(dataset[:,1]))
print(max(dataset[:,0]))

print(kmeans.randCent(dataset, 2))
print(kmeans.distEclud(dataset[0], dataset[1]))

myCentroids, clustAssing = kmeans.kMeans(dataset, 4)
print(myCentroids)
print('\n')
print(clustAssing)

dataMat = mat(kmeans.loadDataSet('Restaurant_Data_Beijing.txt'))
print(dataMat)
kmeans.clusterPlaces(3) 