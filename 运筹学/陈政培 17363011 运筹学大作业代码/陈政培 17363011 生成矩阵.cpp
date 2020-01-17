#include<iostream>
using namespace std;
int main()
{
    int k=5;
    int a=1,b=1;
    for(int i=1;i<=k;i++)
    {
        a=a*(k*2+2-i)/i;
    }
    for(int i=1;i<=k+1;i++)
    {
        b=a*(k*2+2-i)/i;
    }
    cout<<a<<" "<<b<<endl;
    int bi1[a][k],bi2[b][k+1],count=0,t[k+2],flag=0;
    for(int i=0;i<k+2;i++)
    {
        t[i]=i;
    }
    while(t[1]<=k+2)
    {
    	for(int i=0;i<k;i++)
    	{
    		bi1[count][i]=t[i+1];
		}
		++t[k];
    	if(t[k]>k*2+1)
    	{
    		flag=1;
    		while(flag!=0 && flag<k)
    		{
    			t[k-flag+1]--;
    			t[k-flag]++;
    			if(t[k-flag]<t[k-flag+1])
    			{
    				while(flag!=0)
    				{
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
//	for(int i=0;i<a;i++)
//	{
//		for(int j=0;j<k;j++)
//		{
//			cout<<bi1[i][j]<<" ";
//		}
//		cout<<endl;
//	}
//	cout<<endl;
	count=0;flag=-1;
	for(int i=0;i<k+2;i++)
    {
        t[i]=i;
    }
	while(t[1]<=k+1)
    {
    	for(int i=0;i<k+1;i++)
    	{
    		bi2[count][i]=t[i+1];
		}
		++t[k+1];
    	if(t[k+1]>k*2+1)
    	{
    		flag=0;
    		while(flag!=-1 && flag<k)
    		{
    			t[k-flag+1]--;
    			t[k-flag]++;
    			if(t[k-flag]<t[k-flag+1])
    			{
    				while(flag!=-1)
    				{
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
//	for(int i=0;i<b;i++)
//	{
//		for(int j=0;j<k+1;j++)
//		{
//			cout<<bi2[i][j]<<" ";
//		}
//		cout<<endl;
//	}
//	cout<<endl;
	int axis[a][b],temp;
	for(int i=0;i<a;i++)
	{
		for(int j=0;j<b;j++)
		{
			axis[i][j]=0;
		}
	}
	for(int i=0;i<a;i++)
	{
		int z;
		temp=0;
		flag=0;
		for(int j=0;j<b;j++)
		{
			for(z=0;z<k+1;z++)
			{
				if(bi2[j][z] == bi1[i][temp])
				{
					temp++;
					if(temp==k)
						break;
				}
					
			}
			if(temp==k)
			{
				flag=1;
				axis[i][j]=1;
			}
			temp=0;
		}
	}
	for(int i=0;i<a;i++)
	{
		cout<<"[";
		for(int j=0;j<b;j++)
		{
			cout<<axis[i][j]<<" ";
		}
		cout<<"],"<<endl;
	}
}
