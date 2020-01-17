/*********************************************
 * OPL 12.9.0.0 Model
 * Author: 93744
 * Creation Date: Nov 23, 2019 at 2:47:12 AM
 *********************************************/
range i = 1..462;//少的那个
range j = 1..462;//多的那个

int scale[i][j] = ...; 
//float c[j] = ...;

dvar boolean x[j];

minimize
   sum(j1 in j) 1 * x[j1]; ;

constraint ct1;
subject to {
 ct1 = forall (i1 in i)
    sum (j1 in j) scale[i1][j1] * x[j1] >= 1;
}