import kmeans
from numpy import *

dataMat = kmeans.loadDataSet('Restaurant_Data_Beijing.txt')
# print(type(dataMat))
# print(dataMat)

# print(dataMat)

kmeans.clusterPlaces(3)
kmeans.clusterPlaces(4)
kmeans.clusterPlaces(5)
kmeans.clusterPlaces(6)