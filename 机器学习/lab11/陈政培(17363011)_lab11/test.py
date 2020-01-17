from sklearn.datasets import load_iris
from sklearn.preprocessing import MinMaxScaler
from sklearn.model_selection import train_test_split
from sklearn.model_selection import GridSearchCV
from sklearn.svm import SVC
import matplotlib.pyplot as plt 
import numpy as np

iris = load_iris().data 
iris_target = load_iris().target

MinMax = MinMaxScaler()
MinMax.fit(iris)
iris_transf = MinMax.transform(iris)

X_train, X_test, y_train, y_test = train_test_split(iris_transf, iris_target, random_state=14)

# svc = SVC()
# svc.fit(X_train, y_train)

# print("test_svc.score: {:.3f}".format(svc.score(X_test, y_test)))
# print("train_svc.score: {:.3f}".format(svc.score(X_train, y_train)))

# predict_labels = svc.predict([[0.13888889, 0.58333333, 0.15254237, 0.04166667]])
# print(predict_labels)

# tuned_parameters = [{'kernel': ['rbf'], 'gamma': [1e-3, 1e-4],
#                     'C': [1, 10, 100, 1000]},
#                     {'kernel': ['linear'], 'C': [1, 10, 100, 1000]}]
# clf = GridSearchCV(svc, tuned_parameters)
# clf.fit(X_train, y_train)

# print("Best parameters: ",clf.best_params_)

# svc = SVC(C=1, kernel='linear')
# svc.fit(X_train, y_train)
# print("test_svc.score: {:.3f}".format(svc.score(X_test, y_test)))
# print("train_svc.score: {:.3f}".format(svc.score(X_train, y_train)))


Cs = []
score = []
for c in range(10, 100, 10):
    svc = SVC(C=c, gamma=0.001, kernel='rbf')
    svc.fit(X_train, y_train)
    accuracy = svc.score(X_train, y_train)
    print("the accuracy of svc model with C= {0} is {1}".format(c, accuracy))
    Cs.append(c)
    score.append(accuracy)

plt.title("SVC model: argument: C")
plt.xlabel("C")
plt.ylabel("Accuracy")
plt.plot(Cs, score, 'b')
plt.show()