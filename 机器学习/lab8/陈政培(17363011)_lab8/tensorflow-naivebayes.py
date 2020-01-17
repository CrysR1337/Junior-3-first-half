# from IPython import embed
from matplotlib import colors
from matplotlib import pyplot as plt
from sklearn import datasets
import numpy as np
import tensorflow as tf
from sklearn.utils.fixes import logsumexp
import numpy as np
from bayes import *

def spamTest():
    fullTest = []; docList = []; classList= []
    # it only 25 doc in every class
    for i in range(1,26): 
        wordList = textParse(open('email/spam/%d.txt' % i,encoding="ISO-8859-1").read())
        docList.append(wordList)
        fullTest.extend(wordList)
        classList.append(1)
        wordList = textParse(open('email/ham/%d.txt' % i,encoding="ISO-8859-1").read())
        docList.append(wordList)
        fullTest.extend(wordList)
        classList.append(0)
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

    tf_nb = TFNaiveBayesClassifier()
    tf_nb.fit(np.array(trainMat), np.array(trainClass))
    ac = tf_nb.predict(np.array(testMat), np.array(testClass))

    print (("the accuracy is ") , ac)

class TFNaiveBayesClassifier:
    dist = None

    def fit(self, X, y):
        # Separate training points by class (nb_classes * nb_samples * nb_features)
        unique_y = np.unique(y)
        points_by_class = np.array([
            [x for x, t in zip(X, y) if t == c]
            for c in unique_y])

        # Estimate mean and variance for each class / feature
        # shape: nb_classes * nb_features
        mean, var = tf.nn.moments(tf.constant(points_by_class), axes=[1])

        # Create a 3x2 univariate normal distribution with the 
        # known mean and variance
        self.dist = tf.distributions.Normal(loc=mean, scale=tf.sqrt(var))

    def predict(self, X):
        assert self.dist is not None
        nb_classes, nb_features = map(int, self.dist.scale.shape)

        # Conditional probabilities log P(x|c) with shape
        # (nb_samples, nb_classes)
        cond_probs = tf.reduce_sum(
            self.dist.log_prob(
                tf.reshape(
                    tf.tile(X, [1, nb_classes]), [-1, nb_classes, nb_features])),
            axis=2)

        # uniform priors
        priors = np.log(np.array([1. / nb_classes] * nb_classes))

        # posterior log probability, log P(c) + log P(x|c)
        joint_likelihood = tf.add(priors, cond_probs)

        # normalize to get (log)-probabilities
        norm_factor = tf.reduce_logsumexp(
            joint_likelihood, axis=1, keep_dims=True)
        log_prob = joint_likelihood - norm_factor
        # exp to get the actual probabilities
        return tf.exp(log_prob)

if __name__ == '__main__':
    spamTest()