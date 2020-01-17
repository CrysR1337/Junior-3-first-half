from numpy import *
import re

import bayes as bayes
from sklearn.naive_bayes import GaussianNB 
from sklearn.metrics import accuracy_score 
import BernoulliNB as BernoulliNB

mySent = 'This book is the test book on Python on M.L. I have ever laid eyes upon.'
# print(mySent.split())

regEx = re.compile('\\W+')
listOfTokens = regEx.split(mySent)
# print(listOfTokens)

# print( [tok for tok in listOfTokens if len(tok) > 0] )
# print( [tok.lower() for tok in listOfTokens if len(tok) > 0] )

# emailText = open('email/ham/6.txt').read()
# listOfTokens = regEx.split(emailText)

listOPost, listClasses = bayes.loadDataSet()
myVocabList = bayes.createVocabList(listOPost)
# print(myVocabList)

# setOfWords2Vec0 = bayes.setOfWords2Vec(myVocabList, listOfTokens[0])
# print(myVocabList)
# print(listOfTokens[0])
# print(setOfWords2Vec0)
# setOfWords2Vec3 = bayes.setOfWords2Vec(myVocabList, listOfTokens[3])
# print(listOfTokens[3])
# print(setOfWords2Vec3)

# trainMat = []
# for postinDoc in listOPost:
#     trainMat.append(bayes.bagOfWords2Vec(myVocabList, postinDoc))
# p0V, p1V, pAb = bayes.train(trainMat, listClasses)
# print(pAb)
# print(p0V)
# print(p1V)

bayes.spamTest()

# BernoulliNB.spamTest()