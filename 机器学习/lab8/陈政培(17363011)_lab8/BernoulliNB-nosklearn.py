import re

class trainBernoulli:
    def __init__(self, trainSet):
        self.vect = []
        self.train = trainSet
        self.vocab = []

    # preprocess all the sentences
    def preprocess(self):
        train = []
        for words, cla in self.train:
            word_list = [word.lower() for word in re.findall(r"[a-zA-Z]+", words)]
            train.append((' '.join(word_list), cla))
        self.train = train

    # return a set of all the words in the training set.
    def getVocab(self):
        longString = ""
        for text in self.train:
            longString += text[0].lower() + " "
        self.vocab = set(re.findall(r"[a-zA-Z]+", longString))

    # get prior probabilitoes of positive and negative sentences
    def priorClassProb(self):
        positives = [train[1] for train in self.train].count("pos")
        negatives = [train[1] for train in self.train].count("neg")
        return positives, negatives

    # get frequency of words in each class
    # get prior probabilities for the words
    def tokensInClass(self):
        posWords = {}
        negWords = {}
        for sentence, cla in self.train:
            if cla == "pos":
                for word in self.vocab:
                    if word in sentence:
                        if word not in posWords.keys():
                            posWords[word] = 1
                        else:
                            posWords[word] += 1
            if cla == "neg":
                for word in self.vocab:
                    if word in sentence:
                        if word.lower not in negWords.keys():
                            negWords[word] = 1
                        else:
                            negWords[word] += 1
        return posWords, negWords

    # This is the conditional probability
    def conditionalProb(self, posClasses, negClasses):
        posProb = {}
        negProb = {}
        posWords, negWords = self.tokensInClass()
        for word in self.vocab:
            if word in posWords.keys():
                posProb[word] = (posWords[word] + 1.)/(posClasses + 2.)
            else:
                posProb[word] = 1 / (posClasses + 2.)
            if word in negWords.keys():
                negProb[word] = (negWords[word] + 1.)/(negClasses + 2.)
            else:
                negProb[word] = 1 / (negClasses + 2.)
        return posProb, negProb


    def trainAlgo(self):
        self.preprocess()
        self.getVocab()

        posClasses, negClasses = self.priorClassProb()
        totalSentences = posClasses + negClasses
        # proior class probs
        priorPos = posClasses / float(totalSentences)
        priorNeg = negClasses / float(totalSentences)

        posProb, negProb = self.conditionalProb(posClasses, negClasses)

        return self.vocab, priorPos, priorNeg, posProb, negProb

