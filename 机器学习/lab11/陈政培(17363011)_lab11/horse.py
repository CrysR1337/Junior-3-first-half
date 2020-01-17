# -*- coding:UTF-8 -*-
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import GridSearchCV
from sklearn.model_selection import RandomizedSearchCV

frTrain = open('horseColicTraining.txt') 
frTest = open('horseColicTest.txt')
trainingSet = []; trainingLabels = []
testSet = []; testLabels = []
for line in frTrain.readlines():
    currLine = line.strip().split('\t')
    lineArr = []
    for i in range(len(currLine)-1):
        lineArr.append(float(currLine[i]))
    trainingSet.append(lineArr)
    trainingLabels.append(float(currLine[-1]))
for line in frTest.readlines():
    currLine = line.strip().split('\t')
    lineArr =[]
    for i in range(len(currLine)-1):
        lineArr.append(float(currLine[i]))
    testSet.append(lineArr)
    testLabels.append(float(currLine[-1]))
lr = LogisticRegression(solver='liblinear',max_iter=5000)
classifier = LogisticRegression(C=10, solver='saga',max_iter=5000).fit(trainingSet, trainingLabels)
test_accurcy = classifier.score(testSet, testLabels) * 100
print('正确率:%f%%' % test_accurcy)
tuned_parameters = [{'solver': ['sag'], 'C': [1, 10, 100, 1000]},
                {'solver': ['lbfgs'], 'C': [1, 10, 100, 1000]},
                {'solver': ['newton-cg'], 'C': [1, 10, 100, 1000]},
                {'solver': ['saga'], 'C': [1, 10, 100, 1000]},
                {'solver': ['liblinear'], 'C': [1, 10, 100, 1000]}]
clf = GridSearchCV(classifier, tuned_parameters)
clf.fit(trainingSet, trainingLabels)
print("Best parameters: ",clf.best_params_)
tuned_parameters = [{'solver': ['sag'], 'C': [1, 10, 100, 1000]},
                {'solver': ['lbfgs'], 'C': [1, 10, 100, 1000]},
                {'solver': ['newton-cg'], 'C': [1, 10, 100, 1000]},
                {'solver': ['saga'], 'C': [1, 10, 100, 1000]},
                {'solver': ['liblinear'], 'C': [1, 10, 100, 1000]}]
clf = RandomizedSearchCV(lr, tuned_parameters)
clf.fit(trainingSet, trainingLabels)
print("Best parameters: ",clf.best_params_)