#!/usr/bin/env python 
# -*- coding:utf-8 -*-
import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from sklearn import preprocessing
from sklearn.model_selection import train_test_split
from sklearn.neighbors import KNeighborsClassifier

#转化文件格式 第一次执行后，在文件夹下会生成.csv文件，之后就不需要重复执行这段代码了

txt = np.loadtxt('datingTestSet2.txt')
txtDf = pd.DataFrame(txt)
txtDf.to_csv('datingTestSet2.csv', index=False) #no index


#load csv, learn more about it.
dataset = pd.read_csv('datingTestSet2.csv')
dataset.columns = ['miles', 'galons', 'percentage', 'label']
# print(dataset.head())
# print(dataset.dtypes)
# print(np.unique(dataset['label']))
# print(len(dataset))


#analyze our set through seaborn
# 绘制散点图 第一次执行后，三个特征对结果的影响就会有个印象，后面也可以不再执行
'''
sns.lmplot(x='galons', y='percentage', data=dataset, hue='label',fit_reg=False)
sns.lmplot(x='miles', y='percentage', data=dataset, hue='label',fit_reg=False)
sns.lmplot(x='miles', y='galons', data=dataset, hue='label',fit_reg=False)
plt.show()
'''


#cut dataset randomly
'''
dataset_data = dataset[['miles', 'galons', 'percentage']]
dataset_label = dataset['label']
print(dataset_data.head())
data_train, data_test, label_train, label_test = train_test_split(dataset_data, dataset_label, test_size=0.2, random_state=0)
'''
#cut dataset
dataset_data = dataset[['miles', 'galons', 'percentage']]
dataset_label = dataset['label']

data_train = dataset.loc[:800,['miles', 'galons', 'percentage']]  #我让训练集取前800个
# print(data_train.head())
label_train = np.ravel(dataset.loc[:800,['label']])
data_test = dataset.loc[800:,['miles', 'galons', 'percentage']]
label_test = np.ravel(dataset.loc[800:,['label']])

#preprocessing, minmaxscaler
min_max_scaler = preprocessing.MinMaxScaler()
data_train_minmax = min_max_scaler.fit_transform(data_train)
data_test_minmax = min_max_scaler.fit_transform(data_test)
print(data_train_minmax)

#training and scoring
knn = KNeighborsClassifier(n_neighbors=15)
knn.fit(data_train_minmax,label_train)
score = knn.score(X=data_test_minmax,y=label_test,sample_weight=None)
print(score)

#completion
def classifyperson(): #此为手动输入参数预测结果需要的函数
    percentage = float(input('percentage of time spent playing video games?'))
    ffMiles = float(input('frequent flier miles earned per year?'))
    iceCream = float(input('liters of ice-cream consumed per year?'))
    inArr = np.array([[percentage, ffMiles, iceCream]])
    inArr_minmax = min_max_scaler.fit_transform(inArr)
    return inArr_minmax

#inArr_minmax = classifyperson() 

classifyperson()

resultList = ['not at all','in small doses','in large doses']

label_predict = knn.predict(data_test_minmax) #此代码与之前人工切分数据集结合，用于人工校对正确率
print(label_predict)
print(resultList[label_predict[0]])