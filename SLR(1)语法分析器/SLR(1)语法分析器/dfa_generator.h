//dfa_generator.h
#ifndef DFA_H
#define DFA_H

#include<iostream>
#include<stdio.h>
#include<fstream>
#include<malloc.h>
#include<string>
using namespace std;

#define N 1000
#define Y 100

int vtnum,vnnum,pronum;//依次是终结符个数，非终结符个数，产生式个数           
string vt[N];//终结符集         
string vn[N];//非终结符集
string old[N][N]={""};//用于存储文法
string oldz[N][N]={""};//用于存储拓广文法
int FIRST[N][N]={0};//用于存储FIRST集,(i,j)为1时表示，vt[j]表示的终结符包含于vn[i]的FIRST集中
int FOLLOW[N][N]={0}; //用于存储FOLLOW集，(i,j)为1时表示，vt[j]表示的终结符包含于vn[i]的FOLLOW集中
int ACTION[N][N]={0};//动作表
int GOTO[N][N]={0};//状态转换表

typedef struct item{
	int f;//产生式编号
	int l;//表示停顿点在产生式的位置
}item;//定义项目

typedef struct link{
	int f;//表示识别的符号的在符号集中的编号，为了区分终结符和非终结符，非终结符编号=在vn[]中的下标+100
	int l;//表示为状态编号
}link;//DFA中的有向边

typedef struct cd{
	int item_num;//状态中的项目数
	int link_num;//状态的连接数
	item w[N];//项目集I
	link u[N];//边集	
}cd;//定义状态

typedef struct DFA{
	int cd_num;//状态个数
	cd s[N+1];//状态集
}DFA;//定义规范LR(0)项目族,并将D.s[N]用作临时状态转换函数go_switch()的存储空间

DFA D;


//求状态i的闭包
void closure(int i)
{
	int j,k,m,x,flag;
	do
	{
		j=D.s[i].item_num;//j是本轮循环开始前的项目数
		for(k=0;k<D.s[i].item_num;k++)
		{
			for(m=0;m<=pronum;m++)//为了找到所有A->·..的产生式的项目
			{
				if(oldz[m][0]==oldz[D.s[i].w[k].f] [D.s[i].w[k].l])//对当前状态i中的每个项目，查询每个产生式，例如：A->a·Ab应找到A->·..
				{
					flag=0;//判断该项是否在当前状态i中,即检查(m,1)是否存在于状态i中,保证求闭包时加入的新项目和原项目集不重合
					for(x=0;x<D.s[i].item_num;x++) //检索当前状态的项目集中，是否已有(m,l)项目
					{
						if(D.s[i].w[x].f==m&&D.s[i].w[x].l==1)//已存在
						{
							flag=1;   break;
						}
					}
					if(flag==0)//该项不在当前状态i中，将其加入状态i
					{
						D.s[i].w[D.s[i].item_num].f=m;
						D.s[i].w[D.s[i].item_num].l=1;	//由于项目形式为A->·...所以停顿点位置为1
						D.s[i].item_num++;
					}
				}
			}
		}
	}while(j!=D.s[i].item_num);//当一轮没有新的项目加入i时，结束循环
}

//返回终结符和非终结符的标号,判断符号的类型,0<=终结符返回值<100,100<=非终结符返回值<200,错误符号返回值=200
int test(string c)
{  
	int i,j;
	for(i=0;i<vtnum;i++)
	{  
		if(vt[i]==c)
			break;
	}
	if(i<vtnum)
		return i;
	else
	{
		for(j=0;j<vnnum;j++)
		{
			if(vn[j]==c)
				break;
		}
		if(j<vnnum)
			return (N*2+j);
		else 
			return N*4;
	}
}

//状态转换函数，i是状态编号，num是符号编号，状态转换函数的结果存于s[N]
void go_switch(int i,int num)
{   
	int j;
	for(j=0;j<D.s[i].item_num;j++)
	{
		if(test(oldz[D.s[i].w[j].f] [D.s[i].w[j].l])==num) //判断项目j停顿点后的字符的编号是否与num相等；求集合J，并把J加入CLOSURE(J)中
		{
			D.s[N].w[D.s[N].item_num].f=D.s[i].w[j].f;
			D.s[N].w[D.s[N].item_num].l=D.s[i].w[j].l+1;
			D.s[N].item_num++;
			closure(N);	//求CLOSURE(J)
		}
	}
}

//检验状态i是否已存在用当前字符建立的连接
bool test_link(int i,int num)
{
	int j;
	for(j=0;j<D.s[i].link_num;j++)
	{
		if(D.s[i].u[j].f==num)
			return true;
	}
	return false;
}


//检验状态转换函数的结果
int test_go_switch()
{  
	int i,j,k,flag;
	if(D.s[N].item_num==0)//状态转换的结果为空，即当前状态不可接收该字符
		return 0;
	else
	{
		for(i=0;i<D.cd_num;i++)//选定一状态，对s[N]中的每个项目进行循环，如果存在一个项目在当前状态中未找到，即flag=0，立即跳至下一状态
		{
			flag=1;//判断状态转换函数的结果是否已经完全包含于某一现有状态中
			for(j=0;j<D.s[N].item_num;j++)//如果在当前状态i中找不到s[N]的当前项目j,就跳至下一状态
			{
				for(k=0;k<D.s[i].item_num;k++)
				{
					if(D.s[i].w[k].f==D.s[N].w[j].f&&D.s[i].w[k].l==D.s[N].w[j].l) //状态i的项目集==状态转换函数的结果？
						break;
				}
				if(k>=D.s[i].item_num)	//状态i的项目集!=状态转换函数的结果
				{ 
					flag=0; 
					break;
				}
			}
			if(flag==1)//状态i的项目集==状态转换函数的结果
				return 50*N+i;	//返回已存在的状态i的编号
		}
		return 1;//状态转换函数的结果未被任何现有状态完全包含，完全满足建立新状态的条件
	}
}

//把状态转换函数的结果加入DFA，即当建立新状态的条件符合时，建立新的状态s[D.cd_num]
void add_go_switch()
{
	int i;
	for(i=0;i<D.s[N].item_num;i++)
	{
		D.s[D.cd_num].w[D.s[D.cd_num].item_num].f=D.s[N].w[i].f;
		D.s[D.cd_num].w[D.s[D.cd_num].item_num].l=D.s[N].w[i].l;
		D.s[D.cd_num].item_num++;
	}
	D.cd_num++;
}

//清空状态转换函数的存储空间
void del_go_switch()
{
	D.s[N].item_num=0;
}

//构造规范LR(0)项目族
void dfa()
{
	int i,j,k;
	D.s[0].w[0].f=0;
	D.s[0].w[0].l=1;
	D.cd_num++;
	D.s[0].item_num++;
	closure(0);//把P->S加入初状态I，并求其闭包I0
	do 
	{ 
		i=D.cd_num;//本轮循环开始时状态数
		for(j=0;j<D.cd_num;j++)//对每个状态进行循环
		{
			for(k=0;k<vnnum;k++)//对当前状态，每个非终结符
			{
				if(!test_link(j,k+2*N))//检验当前状态用当前非终结符构造的连接是否已经存在，若存在，则跳至下一非终结符
				{
					go_switch(j,k+2*N);//求GO(J,K) K为非终结符，求的结果存放在S[N]
					if(test_go_switch()==1)//如果符合建立新状态的条件
					{
						add_go_switch();//将新状态添加进DFA中
						D.s[j].u[D.s[j].link_num].f=k+2*N;//建立当前状态j和新状态的连接
						D.s[j].u[D.s[j].link_num].l=D.cd_num-1;
						D.s[j].link_num++;
					}
					else
					{
						if(test_go_switch()>=50*N)//如果状态转换的结果包含于某一现有状态
						{
							D.s[j].u[D.s[j].link_num].f=k+2*N;//建立当前状态和该现有状态的连接
							D.s[j].u[D.s[j].link_num].l=test_go_switch()-50*N;//该现有状态的编号
							D.s[j].link_num++;
						}
					}
					del_go_switch();//清空状态转换函数结果，以存储下一个GO函数
				}
			}
			for(k=0;k<vtnum;k++)//对当前状态，每个终结符
			{
				if(!test_link(j,k))//原理与非终结符相同
				{
					go_switch(j,k);
					if(test_go_switch()==1)
					{
						add_go_switch();
						D.s[j].u[D.s[j].link_num].f=k;
						D.s[j].u[D.s[j].link_num].l=D.cd_num-1;
						D.s[j].link_num++;
					}
					else
					{
						if(test_go_switch()>=50*N)
						{
							D.s[j].u[D.s[j].link_num].f=k;
							D.s[j].u[D.s[j].link_num].l=test_go_switch()-50*N;
							D.s[j].link_num++;
						}
					}
					del_go_switch();
				}
			}
		}
	}while(i!=D.cd_num);//当一轮没有新的状态产生时，结束
}


#endif // !DFA_H
