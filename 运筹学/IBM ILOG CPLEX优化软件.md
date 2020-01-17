IBM ILOG CPLEX优化软件

陈政培 17363011 智能科学与技术 智科1班

------

[TOC]

------

## 使用IBM ILOG OPL求解规划问题

### 入门

#### 准备工作

![1](C:\Users\93744\Desktop\1.png)

新建项目后，先复位OPL透视图将所有界面恢复到最初的配置

#### 报错

![TIM图片20191122211323](C:\Users\93744\Desktop\TIM图片20191122211323.png)

这个主要是汉化导致的错误，退出程序，对软件启动文件右键属性修改目标属性，加上 `-nl en` 以后软件将以英文启动，并且不会出现中文相关的报错了

![TIM截图20191122211430](C:\Users\93744\Desktop\TIM截图20191122211430.png)

#### 无法正常显示报错信息

![TIM截图20191123000304](C:\Users\93744\Desktop\TIM截图20191123000304.png)

在cmd中通过 `oplrun -p + 地址` 得到正常显示的报错信息

### 第一题——例2-13连续投资问题

#### 抽象出结构

![TIM截图20191123000045](C:\Users\93744\Desktop\TIM截图20191123000045.png)

分离数据，完成代码，debug后运行

#### 运行结果（最优解和最优目标）

![TIM截图20191123000522](C:\Users\93744\Desktop\TIM截图20191123000522.png)

#### Engine Log记录

![TIM截图20191123000533](C:\Users\93744\Desktop\TIM截图20191123000533.png)

### 第二题——习题2-11 工厂生产问题

#### 数学模型

![TIM截图20191123000828](C:\Users\93744\Desktop\TIM截图20191123000828.png)

分离数据，完成代码，debug后运行

#### 运行结果（最优解和最优目标）

![TIM截图20191123000943](C:\Users\93744\Desktop\TIM截图20191123000943.png)

#### Engine Log记录

![TIM截图20191123001009](C:\Users\93744\Desktop\TIM截图20191123001009.png)

### 第三题——统一花费的集合覆盖问题  

#### 构建思路

这个问题就类似于`dancing line`问题，难点在于如何生成正确的矩阵

#### 构建输入矩阵

通过递归求出`k`元组和`k+1`元组的所有元素，并保存在两个数组中，并逐一比对是否符合配对关系

```c++
#include<iostream>
using namespace std;
int main()
{
    int k=5;	//题目要求的k值
    int a=1,b=1;
    for(int i=1;i<=k;i++){
        a=a*(k*2+2-i)/i;
    }//k元组个数
    for(int i=1;i<=k+1;i++){
        b=a*(k*2+2-i)/i;
    }//k+1元组个数
    cout<<a<<" "<<b<<endl;//输出矩阵行和列
    int bi1[a][k],bi2[b][k+1],count=0,t[k+2],flag=0;
    for(int i=0;i<k+2;i++){
        t[i]=i;
    }//t数组辅助递归的实现，先初始化
    while(t[1]<=k+2){
    	for(int i=0;i<k;i++){
    		bi1[count][i]=t[i+1];
		}//记录当前递归的情况
		++t[k];
    	if(t[k]>k*2+1){
    		flag=1;
    		while(flag!=0 && flag<k){
    			t[k-flag+1]--;
    			t[k-flag]++;
    			if(t[k-flag]<t[k-flag+1]){
    				while(flag!=0){
    					t[k-flag+1]=t[k-flag]+1;
    					flag--;
					}
				}
    			else
    				flag++;
			}//递归实现
		}
		count++;
	}
	/*for(int i=0;i<a;i++)
	{
		for(int j=0;j<k;j++)
		{
			cout<<bi1[i][j]<<" ";
		}
		cout<<endl;
	}*/			//输出k元组，用于debug
	cout<<endl;
	count=0;flag=-1;
	for(int i=0;i<k+2;i++){
        t[i]=i;
    }
	while(t[1]<=k+1){	//递归原理相同
    	for(int i=0;i<k+1;i++){
    		bi2[count][i]=t[i+1];
		}
		++t[k+1];
    	if(t[k+1]>k*2+1){
    		flag=0;
    		while(flag!=-1 && flag<k){
    			t[k-flag+1]--;
    			t[k-flag]++;
    			if(t[k-flag]<t[k-flag+1]){
    				while(flag!=-1){
    					t[k-flag+1]=t[k-flag]+1;
    					flag--;
					}
				}
    			else
    				flag++;
			}
		}
		count++;
	}
	/*for(int i=0;i<b;i++){
		for(int j=0;j<k+1;j++)
		{
			cout<<bi2[i][j]<<" ";
		}
		cout<<endl;
	}*/			//输出k+1元组，用于debug
	cout<<endl;
	int axis[a][b],temp;
	for(int i=0;i<a;i++){
		for(int j=0;j<b;j++){
			axis[i][j]=0;
		}
	}
	for(int i=0;i<a;i++){			//枚举k元组比对是否符合约定条件
		int z;
		temp=0;
		flag=0;
		for(int j=0;j<b;j++){		//枚举k+1元组
			for(z=0;z<k+1;z++){		//逐个元素比对
				if(bi2[j][z] == bi1[i][temp]){
					temp++;
					if(temp==k)
						break;
				}	
			}
			if(temp==k){			//满足约定条件，将矩阵对应元素设置为1
				flag=1;
				axis[i][j]=1;
			}
			temp=0;
		}
	}
	for(int i=0;i<a;i++){			//按照格式输出矩阵
		cout<<"[";
		for(int j=0;j<b;j++){
			cout<<axis[i][j]<<" ";
		}
		cout<<"],"<<endl;
	}
}
```

##### 输出样例

- k=1

![捕获1](C:\Users\93744\Desktop\捕获1.PNG)

- k=2

  ![捕获2](C:\Users\93744\Desktop\捕获2.PNG)

- k=3

  ![捕获3](C:\Users\93744\Desktop\捕获3.PNG)

- k=4 k=5矩阵过大，就不截图了

#### ILOG程序

在解决统一花费的集合覆盖问题时，只需要把抽象的集合问题数学模型中，花费函数 `Cj` 统一定义为1，即每一个`k+1`元组被选取的权重一致，并且此时的 `Xj` 为 `bool` 类型变量

#### OPL ops配置文件设置

在k=1..4时，ILOG都可以很快的计算出问题的最优解，但是当k=5时，数据规模过大，ILOG很长时间无法拟合出最优解，所以需要子在ops中调整一些参数

![捕获4](C:\Users\93744\Desktop\捕获4.PNG)

首先最直观的加速拟合过程的方法就是提高并行计算，将全局线程设置更大，此处考虑到CPU承载能力设置为8线程

随着拟合过程的进行，可行基选取的树结构会越来越复杂，从一开始的几百MB无限增长，所以需要提前设置可用内存，考虑电脑运行内存为8GB，此处设置了4GB

![捕获6](C:\Users\93744\Desktop\捕获6.PNG)

配置文件默认为平衡效率和稳定性，但是为了更快拟合，需要勾选强调计算精度的选项，可以加快solution拟合

![捕获7](C:\Users\93744\Desktop\捕获7.PNG)

配置文件中默认为-1自动选择前处理聚合应用程序，此处设置为1对MIP模型变量替换无限制，以改善拟合效率

#### 运行结果（最优解、最优目标和Engine Log）（ k= 1..4 ）

- k=1

  最优解和最优目标

  ![TIM截图20191124175834](C:\Users\93744\Desktop\TIM截图20191124175834.png)

  Engine Log

  ![TIM截图20191124175908](C:\Users\93744\Desktop\TIM截图20191124175908.png)

- k=2

  最优解和最优目标

  ![TIM截图20191124175948](C:\Users\93744\Desktop\TIM截图20191124175948.png)

  Engine Log

  ![TIM截图20191124180000](C:\Users\93744\Desktop\TIM截图20191124180000.png)

- k=3

  最优解和最优目标

  ![TIM截图20191124180043](C:\Users\93744\Desktop\TIM截图20191124180043.png)

  Engine Log

  ![TIM截图20191124180050](C:\Users\93744\Desktop\TIM截图20191124180050.png)

- k=4

  最优解和最优目标

  ![TIM截图20191124180146](C:\Users\93744\Desktop\TIM截图20191124180146.png)

  Engine Log

  ![TIM截图20191124180154](C:\Users\93744\Desktop\TIM截图20191124180154.png)

#### k=5次优解分析

Engine Log  配置好ops后运行

![TIM截图20191124192140](C:\Users\93744\Desktop\TIM截图20191124192140.png)

Gap收敛速度相对配置前，更快

- 次优解

  ![TIM截图20191124191915](C:\Users\93744\Desktop\TIM截图20191124191915.png)

  得到的结果101，后续收敛非常慢，后由于内存溢出停止
  
  以下为log分析，前期收敛较快，从13%到10.64%收敛树结构生成了 `4091.81 MB` ，后开始压缩结构存储，内存报错前 `Gap` 值最优为 `10.63%` 
  
  ```c
          Nodes                                         Cuts/
     Node  Left     Objective  IInf  Best Integer    Best Bound    ItCnt     Gap
  
  *     0+    0                          462.0000        0.0000           100.00%
  *     0+    0                          110.0000        0.0000           100.00%
  *     0+    0                          108.0000        0.0000           100.00%
  *     0+    0                          106.0000        0.0000           100.00%
  *     0+    0                          104.0000        0.0000           100.00%
  *     0+    0                          102.0000        0.0000           100.00%
        0     0       77.0000   462      102.0000       77.0000     1056   24.51%
        0     0       80.5852   455      102.0000     Cuts: 106     1247   20.99%
        0     0       82.3184   453      102.0000  ZeroHalf: 70     1466   19.30%
        0     0       83.4929   446      102.0000  ZeroHalf: 75     1748   18.14%
        0     0       84.6125   441      102.0000  ZeroHalf: 66     2095   17.05%
        0     0       85.5231   439      102.0000  ZeroHalf: 77     2492   16.15%
        0     0       86.8162   415      102.0000  ZeroHalf: 32     3331   14.89%
        0     0       87.1999   419      102.0000  ZeroHalf: 24     3610   14.51%
        0     0       87.4310   416      102.0000  ZeroHalf: 18     3874   14.28%
        0     0       87.5333   417      102.0000   ZeroHalf: 9     4059   14.18%
        0     0       87.5954   421      102.0000   ZeroHalf: 7     4197   14.12%
        0     0       87.6657   417      102.0000   ZeroHalf: 8     4354   14.05%
        0     0       87.7408   421      102.0000   ZeroHalf: 8     4509   13.98%
        0     0       87.7877   420      102.0000   ZeroHalf: 6     4624   13.93%
        0     0       87.8646   417      102.0000   ZeroHalf: 6     4750   13.86%
        0     0       87.9614   412      102.0000   ZeroHalf: 9     4925   13.76%
        0     0       88.0049   418      102.0000   ZeroHalf: 4     5076   13.72%
        0     0       88.1268   419      102.0000   ZeroHalf: 8     5309   13.60%
        0     0       88.2101   424      102.0000   ZeroHalf: 8     5561   13.52%
        0     0       88.2963   415      102.0000  ZeroHalf: 11     6041   13.44%
        0     0       88.3779   412      102.0000   ZeroHalf: 9     6697   13.36%
        0     0       88.5119   410      102.0000   ZeroHalf: 9     7834   13.21%
        0     0       88.7222   410      102.0000  ZeroHalf: 14     7941   13.02%
        0     0       88.7222   415      102.0000   ZeroHalf: 3     7967   13.02%
        0     2       88.7222   400      102.0000       88.7222     7967   13.02%
  ...
  ...
  Elapsed time = 392.77 sec. (272300.48 ticks, tree = 384.39 MB, solutions = 7)
    72661  5807       96.1058   294      101.0000       90.0066  9310022   10.88%
  Elapsed time = 1151.27 sec. (806900.98 ticks, tree = 1035.82 MB, solutions = 7)
   260167 185744       99.8571   251      101.0000       90.0224 29967702   10.87%
  ...
  ...
  Elapsed time = 3178.33 sec. (2142853.00 ticks, tree = 3993.34 MB, solutions = 7)
   773685 678698       93.4005   307      101.0000       90.2499 86090688   10.64%
  Elapsed time = 3239.56 sec. (2181024.89 ticks, tree = 4091.81 MB, solutions = 7)
   788396 692338       91.0870   341      101.0000       90.2540 87645258   10.64%
  Elapsed time = 3306.80 sec. (2219194.14 ticks, tree = 4176.81 MB, solutions = 7)
  Nodefile size = 78.83 MB (56.45 MB after compression)
   802831 705947       93.6489   311      101.0000       90.2569 89199774   10.64%
  Elapsed time = 3372.73 sec. (2257370.87 ticks, tree = 4249.38 MB, solutions = 7)
  Nodefile size = 151.58 MB (108.44 MB after compression)
   817920 719181       96.1040   291      101.0000       90.2597 90697048   10.63%
  Elapsed time = 3439.75 sec. (2295546.96 ticks, tree = 4327.13 MB, solutions = 7)
  Nodefile size = 228.63 MB (163.49 MB after compression)
   832437 735391       97.1211   281      101.0000       90.2632 92492473   10.63%
  Elapsed time = 3506.42 sec. (2333713.97 ticks, tree = 4421.28 MB, solutions = 7)
  Nodefile size = 323.34 MB (231.22 MB after compression)
   846841 749868       91.0181   363      101.0000       90.2670 94131760   10.63%
  ```

# 参考文献

1. https://wenku.baidu.com/view/7f9fe7bb48d7c1c709a145dd.html **ILOG_OPL进阶功能**