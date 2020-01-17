
# -*- coding=utf-8 -*-

import operator
from numpy import *
import matplotlib
import matplotlib.pyplot as plt
import sklearn
import knn1
import knn2
import knn3
from sklearn import preprocessing

plt.rcParams['font.sans-serif']=['SimHei']

# group, labels = knn1.createDataSet()
# print(group)
# print(labels)
# ans = knn1.classify([0,1],group,labels,3)
# print(ans)

# datingDataMat, datingLabels = knn2.file2matrix('datingTestSet2.txt')
# print(datingDataMat)
# print(datingLabels[0:20])

# plt.scatter(datingDataMat[:,1], datingDataMat[:,2])
# plt.show()
# plt.scatter(datingDataMat[:,1], datingDataMat[:,2], 15.0*array(datingLabels),15.0*array(datingLabels))

# normMat, ranges, minVals = knn2.autoNorm(datingDataMat)
# print(normMat)
# print(ranges)
# print(minVals)
# print('k=6')
# knn2.datingClassTest(6)
# print('k=5')
# knn2.datingClassTest(5)
# print('k=4')
# knn2.datingClassTest(4)
# print('k=3')
# knn2.datingClassTest(3)
# print('k=2')
# knn2.datingClassTest(2)

# knn2.classifyPerson()
# knn2.classifyPerson()

# knn3.createDataSet()
# knn3.trainingDataSet()
knn3.handwritingTest()
