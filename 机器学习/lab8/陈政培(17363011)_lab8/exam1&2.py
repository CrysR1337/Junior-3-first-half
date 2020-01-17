import numpy as np
from bayes import *
from sklearn.naive_bayes import BernoulliNB 
from sklearn.metrics import accuracy_score 

def spamTest():
    fullTest = []; docList = []; classList= []
    wordList = textParse(open('example1.csv',encoding="ISO-8859-1").read())
    # wordList = textParse(open('example2.csv',encoding="ISO-8859-1").read())
    classList.append(1)
    # create vocabulary
    vocabList = createVocabList(docList)   
    trainSet = list(range(50));testSet=[]
    # choose 10 sample to test ,it index of trainMat
    for i in range(10):
        randIndex = int(np.random.uniform(0,len(trainSet)))#num in 0-49
        testSet.append(trainSet[randIndex])
        del(trainSet[randIndex])
    trainMat = []; trainClass = []
    testMat = []; testClass = []
    for docIndex in trainSet:
        trainMat.append(bagOfWords2Vec(vocabList,docList[docIndex]))
        trainClass.append(classList[docIndex])
    for docIndex in testSet:
        testMat.append(bagOfWords2Vec(vocabList,docList[docIndex]))
        testClass.append(classList[docIndex])
    ac = NB_Accuracy(np.array(trainMat),np.array(trainClass),np.array(testMat),np.array(testClass))

    print (("Predict accuracy ") , ac)


def NB_Accuracy(features_train, labels_train,features_test, labels_test): 

  ### 创建分类器 
  clf = BernoulliNB() 
  
  ### 训练分类器 
  X=features_train 
  Y=labels_train 
  clf.fit(X,Y) 
  
  ### 用训练好的分类器去预测测试集的标签值 
  pred =clf.predict(features_test) 
  
  ### 计算并返回在测试集上的准确率 
  y_pred =pred 
  y_true =labels_test
  accuracy_score(y_true, y_pred) 
  
  return accuracy_score(y_true, y_pred,normalize=False)

if __name__ == '__main__':
    spamTest()