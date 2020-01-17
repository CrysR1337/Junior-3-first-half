/*********************************************
 * OPL 12.9.0.0 Model
 * Author: 93744
 * Creation Date: Nov 22, 2019 at 11:13:34 PM
 *********************************************/
{string} Product = ...;
{string} Machine = ...;
{string} Extra = ...;

float scale[Machine][Product] = ...; 
float equal[Extra][Product] = ...; 
float hour[Machine] = ...;
float profit[Product] = ...;

dvar float+ Production[Product];

maximize
   sum(p in Product) profit[p] * Production[p]; ;

constraint ct1;
constraint ct2;
subject to {
 ct1 = forall (m in Machine)
    sum (p in Product) scale[m][p] * Production[p] <= hour[m];
 ct2 = forall (e in Extra)
    sum (p in Product) equal[e][p] * Production[p] == 0;
}