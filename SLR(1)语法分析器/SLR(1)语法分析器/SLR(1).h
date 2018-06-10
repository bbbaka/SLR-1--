//SLR(1).h
#ifndef SLR1_H
#define SLR1_H
#include"printer.h"

////通过old所存储的文法来求解first和follow,先不考虑带空字的产生式（LR(0)和SLR(1)文法会带空字吗？？
////又回到最初的起点，呆呆地站在空字前   '@'为空字代表选手
//void first()//(i,j)为1时表示，vt[j]表示的终结符包含于vn[i]的FIRST集中
//{
//	int flag,i,j,k,x,y,z;
//	do
//	{
//		flag=0;	//若FIRST有变化，则FLAG=1
//		for(i=1;i<=pronum;i++)//把产生式中第一个是vt的加入first集
//		{
//			j=test(old[i][1]);k=test(old[i][0]);
//			if(j<2*N)//是终结符，若为空字，则产生式右部必定只有空字，否则可以转换成不带空字的形式
//			{
//				if(FIRST[k-2*N][j]==0)	
//				{
//					FIRST[k-2*N][j]=1;
//					flag=1;
//				}
//			}
//		}
//
//		for(i=1;i<=pronum;i++)//将产生式中第一个是 VN的first集-空字 加入到产生式左部的FIRST
//		{
//			j=test(old[i][1]);k=test(old[i][0]);
//			if(j>2*N && j!=4*N)
//			{
//				for(x=0;x<=vtnum;x++)
//				{
//					//if(x!=vtnum-1)	//空字除外
//					//{
//						if(FIRST[j-2*N][x]==1&&FIRST[k-2*N][x]==0)
//						{
//							FIRST[k-2*N][x]=1;
//							flag=1;
//						}
//					//}
//				}
//			}
//		}
//		//由于不考虑带空字的产生式，规则4不实现
//		//出来混，迟早都是要还的
//		//for(i=1;i<=pronum;i++)
//		//{
//		//	int eflag=0;
//		//	y=test(old[i][0]);
//		//	for(j=2;j<N;j++)
//		//	{
//		//		if(old[i][j]=="")	//右部遍历结束
//		//		{
//		//			break;
//		//		}
//		//		k=test(old[i][j]);
//		//		if(k<2*N)	//遇到了终结符
//		//			break;
//		//		//为非终结符
//		//		if(FIRST[k-2*N][vtnum-1]==1)	//该非终结符的first集包含空字
//		//		{//不拿老百姓的一个空字
//		//			for(x=0;x<=vtnum;x++)
//		//			{
//		//				if(x!=vtnum-1)	//空字除外
//		//				{
//		//					if(FIRST[j-2*N][x]==1&&FIRST[k-2*N][x]==0)
//		//					{
//		//						FIRST[k-2*N][x]=1;
//		//						flag=1;
//		//					}
//		//				}
//		//			}
//		//			eflag++;
//		//		}
//		//	}
//			//if(eflag+2==__getlen(i))//产生式右部的非终结符都带空字
//			//{
//			//	FIRST[y-2*N][vtnum-1]=1;
//			//}
//		//}
//
//	}while(flag==1);
//}
//
//void follow()//(i,j)为1时表示，vt[j]表示的终结符包含于vn[i]的FOLLOW集中
//{
//	FOLLOW[0][vtnum]=1;//把#加入文法开始符的FOLLOW集
//	int flag,i,j,k,x,y;
//	do
//	{
//		flag=0;
//		for(i=1;i<=pronum;i++)//将产生式右部 VN后的字符串的FIRST集加入FOLLOW集，仍然不考虑空字
//		{
//			for(j=1;old[i][j]!="";j++)//扫描产生式
//			{
//				k=test(old[i][j]);
//				if(k>=2*N && k!=4*N)//非终结符
//				{
//					if(old[i][j+1]!="")//不是最后一个
//					{
//						x=test(old[i][j+1]);
//						if(x<2*N &&FOLLOW[k-2*N][x]==0)//如果vn[k-100]后为终结符，将其加入follow
//						{
//							FOLLOW[k-2*N][x]=1;
//							flag=1;
//						}
//						else if(x>2*N && x!=4*N)//如果vn[k-100]后为非终结符x，将x的 first集-空字 加到follow
//						{
//							for(y=0;y<=vtnum;y++)
//							{
//								//if(FIRST[x-2*N][y]==1 && FOLLOW[k-2*N][y]==0 && y!=vtnum-1)
//								if(FIRST[x-2*N][y]==1 && FOLLOW[k-2*N][y]==0)
//								{
//									FOLLOW[k-2*N][y]=1;
//									flag=1;
//								}
//							}
//						}
//					}
//					else//是最后一个，则将产生式左部的非终结符的follow加到follow中
//					{
//						for(y=0;y<=vtnum;y++)
//						{
//							if(FOLLOW[test(old[i][0])-2*N][y]==1 && FOLLOW[k-2*N][y]==0)
//							{
//								FOLLOW[k-2*N][y]=1;
//								flag=1;
//							}
//						}
//					}
//				}
//			}
//		}
//
//		////回来考虑空字了 这里处理A->aBb 空字属于FIRST(b)
//		//for(i=1;i<pronum;i++)
//		//{
//
//		//}
//
//	}while(flag==1);
//}

void first()//(i,j)为1时表示，vt[j]表示的终结符包含于vn[i]的FIRST集中
{
	int flag,i,j,k,x;
	do
	{
		flag=0;	//若FIRST有变化，则FLAG=1
		for(i=1;i<=pronum;i++)//把产生式中第一个是vt的加入first集
		{
			j=test(old[i][1]);k=test(old[i][0]);
			if(j<2*N)//是终结符
			{
				if(FIRST[k-2*N][j]==0)
				{
					FIRST[k-2*N][j]=1;
					flag=1;
				}
			}
		}
		for(i=1;i<=pronum;i++)//将产生式中第一个是VN的first集加入到产生式左部的FIRST
		{
			j=test(old[i][1]);k=test(old[i][0]);
			if(j>2*N && j!=4*N)
			{
				for(x=0;x<=vtnum;x++)
				{
					if(FIRST[j-2*N][x]==1&&FIRST[k-2*N][x]==0)
					{
						FIRST[k-2*N][x]=1;
						flag=1;
					}
				}
			}
		}
		//由于不考虑带空字的产生式，规则4不实现
	}while(flag==1);
}

void follow()//(i,j)为1时表示，vt[j]表示的终结符包含于vn[i]的FOLLOW集中
{
	FOLLOW[0][vtnum]=1;//把#加入文法开始符的FOLLOW集
	int flag,i,j,k,x,y;
	do
	{
		flag=0;
		for(i=1;i<=pronum;i++)//将产生式右部 VN后的字符串的FIRST集加入FOLLOW集，仍然不考虑空字
		{
			for(j=1;old[i][j]!="";j++)//扫描产生式
			{
				k=test(old[i][j]);
				if(k>=2*N && k!=4*N)//非终结符
				{
					if(old[i][j+1]!="")//不是最后一个
					{
						x=test(old[i][j+1]);
						if(x<2*N &&FOLLOW[k-2*N][x]==0)//如果vn[k-100]后为终结符，将其加入follow
						{
							FOLLOW[k-2*N][x]=1;
							flag=1;
						}
						else if(x>2*N && x!=4*N)//如果vn[k-100]后为非终结符x，将x的first集加到follow
						{
							for(y=0;y<=vtnum;y++)
							{
								if(FIRST[x-2*N][y]==1 && FOLLOW[k-2*N][y]==0)
								{
									FOLLOW[k-2*N][y]=1;
									flag=1;
								}
							}
						}
					}
					else//是最后一个，则将产生式左部的非终结符的follow加到follow中
					{
						for(y=0;y<=vtnum;y++)
						{
							if(FOLLOW[test(old[i][0])-2*N][y]==1 && FOLLOW[k-2*N][y]==0)
							{
								FOLLOW[k-2*N][y]=1;
								flag=1;
							}
						}
					}
				}
			}
		}
	}while(flag==1);
}


void print_first()
{
	int i,j;
	cout<<"FIRST:"<<endl;
	_log<<"FIRST:"<<endl;
	for(i=0;i<vnnum;i++)
	{
		cout<<vn[i]<<"={";
		_log<<vn[i]<<"={";
		for(j=0;j<=vtnum;j++)
		{
			if(FIRST[i][j]==1)
			{
				cout<<vt[j]<<",";
				_log<<vt[j]<<",";
			}
		}
		cout<<endl;
		_log<<endl;
	}
	cout<<endl;
	_log<<endl;
}

void print_follow()
{
	int i,j;
	cout<<"FOLLOW:"<<endl;
	_log<<"FOLLOW:"<<endl;
	for(i=0;i<vnnum;i++)
	{
		cout<<vn[i]<<"={";
		_log<<vn[i]<<"={";
		for(j=0;j<=vtnum;j++)
		{
			if(FOLLOW[i][j]==1)
			{
				cout<<vt[j]<<",";
				_log<<vt[j]<<",";
			}
		}
		cout<<endl;
		_log<<endl;
	}
	cout<<endl;
	_log<<endl;
}


bool contain(int k,string c)//判断c是否属于vn[K]的FOLLOW集
{
	for(int i=0;i<=vtnum;i++)
	{
		if(FOLLOW[k][i]==1)
			if(c==vt[i])
				return true;
	}
	return false;
}


//构造ACTION表：0为出错，0-100为移进操作，100-300为归约操作，300表示acc
void action()
{ 
	int i,j,k;
	first();follow();
	first();follow();first();follow();
	print_first();print_follow();
	for(i=0;i<D.cd_num;i++)//对每个状态循环
	{
		for(j=0;j<D.s[i].link_num;j++)//S，对状态i的每个边进行循环，若状态i发出的边接受的字符为终结符，则ACTION[当前状态号][对应终结符编号]=边另一端状态号 
		{
			if(D.s[i].u[j].f<2*N)	//终结符？
			{
				ACTION[i][D.s[i].u[j].f]=D.s[i].u[j].l;	//偏移量0表示为S
			}
		}
		for(j=0;j<D.s[i].item_num;j++)//r，对状态i的每个项目循环，如果找到一个终结项目，例如A->c·,  则ACTION[当前状态][所有vt和$]=产生式A->c的编号
		{
			if(oldz[D.s[i].w[j].f][D.s[i].w[j].l]=="")
			{
				for(k=0;k<=vtnum;k++)
				{
					if(contain(test(oldz[D.s[i].w[j].f][0])-2*N, vt[k]))//属于其FOLLOW集
						ACTION[i][k]=D.s[i].w[j].f+2*N;	//加100偏移量表示为R
				}
			}
		}
		for(j=0;j<D.s[i].item_num;j++)//acc，对每个状态的每个项目循环，如果找到P->S· 所在状态，则ACTION[当前状态][$]=300，即acc
		{
			if(D.s[i].w[j].f==0&&D.s[i].w[j].l==2)
			{
				ACTION[i][vtnum]=6*N;  break;
			}
		}
	}
}

//构造GOTO表
void go_answer()
{ 
	int i,j;
	for(i=0;i<D.cd_num;i++)
	{
		for(j=0;j<D.s[i].link_num;j++)//对状态i的每个边进行循环，若状态i发出的边接受的字符为非终结符，则GOTO[当前状态号][对应非终结符编号]=连接另一端状态号
		{
			if(D.s[i].u[j].f>=2*N)//非终结符？
			{
				GOTO[i][D.s[i].u[j].f-2*N]=D.s[i].u[j].l;
			}
		}
	}
}

#endif // !SLR1_H
