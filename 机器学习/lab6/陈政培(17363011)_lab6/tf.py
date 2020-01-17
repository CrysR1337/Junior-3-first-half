import tensorflow as tf
import numpy as np

def file2Mat(testFileName, parammterNumber):
    fr = open(testFileName)
    lines = fr.readlines()
    lineNums = len(lines)
    resultMat = np.zeros((lineNums, parammterNumber))
    classLabelVector = []
    for i in range(lineNums):
        line = lines[i].strip()
        itemMat = line.split('\t')
        resultMat[i, :] = itemMat[0:parammterNumber]
        classLabelVector.append(itemMat[-1])
    fr.close()
    return resultMat, classLabelVector

# 为了防止某个属性对结果产生很大的影响，所以有了这个优化，比如:10000,4.5,6.8 10000就对结果基本起了决定作用
def autoNorm(dataSet):
    minVals = dataSet.min(0)
    maxVals = dataSet.max(0)
    ranges = maxVals - minVals
    normMat = np.zeros(np.shape(dataSet))
    size = normMat.shape[0]
    normMat = dataSet - np.tile(minVals, (size, 1))
    normMat = normMat / np.tile(ranges, (size, 1))
    return normMat, minVals, ranges

if __name__=='__main__':

    trainigSetFileName = 'datingTestSet.txt'
    testFileName = 'datingTestSet2.txt'

    # 读取训练数据
    trianingMat, classLabel = file2Mat(trainigSetFileName, 3)
    # 都数据进行归一化的处理
    autoNormTrianingMat, minVals, ranges = autoNorm(trianingMat)
    # 读取测试数据
    testMat, testLabel = file2Mat(testFileName, 3)
    autoNormTestMat = []
    for i in range(len(testLabel)):
        autoNormTestMat.append((testMat[i] - minVals) / ranges)

    # 循环迭代计算每一个测试数据的预测值，并且和真正的值进行对比，并计算精确度。该算法比较经典的是不需要提前训练，直接在测试阶段进行识别。
    traindata_tensor=tf.placeholder('float',[None,3])
    testdata_tensor=tf.placeholder('float',[3])

    distance = tf.sqrt(tf.reduce_sum(tf.pow(tf.add(traindata_tensor, tf.negative(testdata_tensor)), 2), reduction_indices=1))
    pred = tf.arg_min(distance,0)
    test_num=1
    accuracy=0
    init=tf.global_variables_initializer()
    with tf.Session() as sess:
        sess.run(init)
        for i in range(test_num):
            print(sess.run(distance,feed_dict={traindata_tensor:autoNormTrianingMat,testdata_tensor:autoNormTestMat[i]}))
            idx=sess.run(pred,feed_dict={traindata_tensor:autoNormTrianingMat,testdata_tensor:autoNormTestMat[i]})
            print(idx)

            print('test No.%d,the real label %d, the predict label %d'%(i,np.argmax(testLabel[i]),np.argmax(classLabel[idx])))
            if np.argmax(testLabel[i])==np.argmax(classLabel[idx]):
                accuracy+=1
        print("result:%f"%(1.0*accuracy/test_num))