/*********************************************
 * OPL 12.9.0.0 Model
 * Author: 93744
 * Creation Date: Nov 22, 2019 at 9:17:19 PM
 *********************************************/
{string} Project = ...;
{string} year = ...;

float scale[Project][year] = ...; 
float money[year] = ...;
tuple ProjectLimit {
   float profit;
   float maxmoney;
}
ProjectLimit limit[Project] = ...; 

dvar float+ Product[Project];

maximize
   sum(p in Project) limit[p].profit * Product[p]; ;

constraint ct1;
constraint ct2;
subject to {
 ct1 = forall (y in year)
    sum (p in Project) scale[p][y] * Product[p] == money[y];
 ct2 = forall (p in Project)
 	Product[p] <= limit[p].maxmoney;
}
