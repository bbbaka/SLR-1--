//printer.h
#ifndef PRINTER_H
#define PRINTER_H

#include<fstream>
#include<stack>
#include<iomanip>
#include"dfa_generator.h"

ofstream _log("d:\\log.txt");
//输出两张表
void printf_ag()
{
	int i,j;
	printf("ACTION表为:\n");
	_log<<"ACTION表为:\n";
	printf("\t");
	_log<<'\t';
	for(i=0;i<=vtnum;i++)
	{
		printf("%s\t",vt[i].c_str());
		_log<<vt[i].c_str()<<'\t';
	}
	printf("\n");
	_log<<'\n';
	for(i=0;i<D.cd_num;i++)
	{
		if(i<10)
		{
			printf("%d\t",i);
			_log<<i<<'\t';
		}
		else
		{
			printf("%d\t",i);
			_log<<i<<'\t';
		}
		for(j=0;j<=vtnum;j++)
		{
			if(ACTION[i][j]<2*N && ACTION[i][j]>0)
			{
				if(ACTION[i][j]<2*N && ACTION[i][j]>0)
				{
					printf("s%d\t",ACTION[i][j]);
					_log<<"S"<<ACTION[i][j]<<'\t';
				}
				else
				{
					printf("s%d\t",ACTION[i][j]);
					_log<<"S"<<ACTION[i][j]<<'\t';
				}
			}
			else if(ACTION[i][j]>2*N && ACTION[i][j]<6*N)
			{
				if((ACTION[i][j]-2*N)<2*N && (ACTION[i][j]-2*N)>0)
				{
					printf("r%d\t",ACTION[i][j]-2*N);
					_log<<"R"<<ACTION[i][j]-2*N<<'\t';
				}
				else
				{
					printf("r%d\t",ACTION[i][j]-2*N);
					_log<<"R"<<ACTION[i][j]-2*N<<'\t';
				}
			}
			else if(ACTION[i][j]==6*N)
			{
				printf("acc\t");
				_log<<"acc\t";
			}
			else
			{
				printf("\t");
				_log<<"\t";
			}
		}
		printf("\n");
		_log<<"\n";
	}
	printf("\nGOTO表为:\n");
	_log<<"\nGOTO表为:\n";
	printf("\t");
	_log<<"\t";
	for(i=0;i<vnnum;i++)
	{
		printf("%s\t",vn[i].c_str());
		_log<< vn[i].c_str()<<"\t";
	}
	printf("\n");
	_log<<'\n';
	for(i=0;i<D.cd_num;i++)
	{
		if(i<10)
		{
			printf("%d\t",i);
			_log<<i<<'\t';
		}
		else
		{
			printf("%d\t",i);
			_log<<i<<'\t';
		}
		for(j=0;j<vnnum;j++)
		{
			if(GOTO[i][j]<10)
			{
				printf("%d\t",GOTO[i][j]);
				_log<<GOTO[i][j]<<'\t';
			}
			else
			{
				printf("%d\t",GOTO[i][j]);
				_log<<GOTO[i][j]<<'\t';
			}
		}
		printf("\n");
		_log<<'\n';
	}
}

typedef struct token
{
	string type;
	int  key;
	string value;
}token;

void print(token t)
{
	cout<<t.type;
}

void ErroR()
{

}

void readin(string &type,int &key,string &value,fstream &str)
{
	str>>type>>key>>value;
	//cout<<type<<" "<<key<<" "<<value<<endl;
	_log<<type<<" "<<key<<" "<<value<<endl;
	//str>>type;
}


int __getlen(int i)//获取产生式长度
{
	if(i<=pronum)	//产生式合法
	{
		//return strlen(oldz[i])-1;
		int count=0;
		for(int j=0;oldz[i][j]!="";j++)
			count++;
		return count-1;
	}
	else
	{
		cout<<"产生式不存在"<<endl;
		_log<<"产生式不存在\n";
		ErroR();exit(0);
	}
}

void stack_overlook(stack<int> S)
{
	stack<int> SS;
	int temp;
	while(S.size())
	{
		temp=S.top();
		S.pop();
		SS.push(temp);
	}
	while(SS.size())
	{
		temp=SS.top();
		cout<<temp<<" ";
		_log<<temp<<" ";
		SS.pop();
	}
}

void stack_overlook(stack<string> S)
{
	stack<string> SS;
	string temp;
	while(S.size())
	{
		temp=S.top();
		S.pop();
		SS.push(temp);
	}
	while(SS.size())
	{
		temp=SS.top();
		cout<<temp<<" ";
		_log<<temp<<" ";
		SS.pop();
		//S.push(temp);
	}
	_log<<"**";
}

void print_gen(int i)
{
	cout<<oldz[i][0]<<"->";
	_log<<oldz[i][0]<<"->";
	int j=1;
	while(oldz[i][j]!="")
	{
		cout<<oldz[i][j];
		_log<<oldz[i][j];
		j++;
	}
}


//LR分析器
void Controler()
{
	//variable
	//token t;
	//cout<<"步骤\t"<<"状态栈\t"<<"符号栈\t"<<"当前符号\t"<<"动作说明\n";
	string type,value;
	int  key;
	int ss=1;
	stack<int> S;		//状态栈
	stack<string> mark;	//符号栈
	//init
	vtnum++;
	S.push(0);
	mark.push("#");
	fstream str("d:\\output.txt",ios::in);
	//for(int i=0;i<10;i++)
	cout<<endl;
	_log<<'\n';
	readin(type,key,value,str);
	do
	{
		if(S.size()==mark.size())//状态栈大小等于符号栈，查ACTION
		{
			int j=ACTION[S.top()][test(type)];	//test返回标号
			/*
			if(j==0)//调用出错处理
			{
				cout<<"wrong"<<endl;
				_log<<"ACTION=0;wrong"<<endl;
				ErroR();exit(0);
			}
			else */
			if(1<=j&&j<2*N)//移进 结束后移动输入串
			{
				cout<<ss<<'\t';
				_log<<ss<<'\t';
				stack_overlook(S);cout<<'\t';_log<<'\t';stack_overlook(mark);
				cout<<'\t'<<type<<'\t';
				_log<<'\t'<<type<<'\t';
				cout<<"ACTION["<<S.top()<<","<<type<<"]="<<"S"<<j<<",即状态"<<j<<"入栈"<<endl;
				_log<<"ACTION["<<S.top()<<","<<type<<"]="<<"S"<<j<<",即状态"<<j<<"入栈"<<endl;
				ss++;
				S.push(j);
				mark.push(type);
				readin(type,key,value,str);
			}else if(2*N<=j&&j<6*N)//归约后不允许立刻读进 因为符号栈大于状态栈，跳转至GOTO；
			{
				//int i=ACTION[S.top()][test(type)];
				cout<<ss<<'\t';
				_log<<ss<<'\t';
				stack_overlook(S);cout<<'\t';_log<<'\t';stack_overlook(mark);
				cout<<'\t'<<type<<'\t';
				_log<<'\t'<<type<<'\t';
				//cout<<"ACTION["<<S.top()<<","<<type<<"]="
				cout<<"R"<<j-2*N<<",用产生式";
				_log<<"R"<<j-2*N<<",用产生式";
				print_gen(j-2*N);
				cout<<" 归约且GOTO[";
				_log<<" 归约且GOTO[";
				int k=__getlen(j-2*N);	//返回产生式长度
				//cout<<"产生式长度为："<<k<<endl;
				while(k--)	//运行时判断边界是否正确
				{
					//cout<<"弹出状态"<<S.top()<<" 符号"<<mark.top()<<' \t ';
					S.pop();
					mark.pop();
				}
				if(j-2*N!=0)
				{
					//cout<<"压入符号"<<oldz[j-100][0]<<endl;
					mark.push(oldz[j-2*N][0]);
					//补回去
					int j=GOTO[S.top()][test(mark.top())-2*N];
					if(S.size()!=mark.size())
					{
						if(j!=0)
						{
							cout<<S.top()<<","<<type<<"]="<<j<<"入栈"<<endl;
							_log<<S.top()<<","<<type<<"]="<<j<<"入栈"<<endl;
							ss++;
							S.push(j);
						}
						else 
						{
							cout<<"wrong"<<endl;
							_log<<"GOTO=0 wrong"<<endl;
							ErroR();exit(0);
						}
					}
				}
			}else if(j==6*N)	//acc
			{
				cout<<ss<<'\t';
				_log<<ss<<'\t';
				stack_overlook(S);cout<<'\t';_log<<'\t';stack_overlook(mark);
				cout<<'\t'<<type<<'\t';
				_log<<'\t'<<type<<'\t';
				cout<<"acc!分析成功"<<endl;
				_log<<"acc!分析成功"<<endl;
				return ;
			}else 
			{
				cout<<ss<<'\t';
				_log<<ss<<'\t';
				stack_overlook(S);cout<<'\t';_log<<'\t';stack_overlook(mark);
				cout<<'\t'<<type<<'\t';
				_log<<'\t'<<type<<'\t';
				cout<<"wrong!分析出错"<<endl;
				_log<<"ACTION=0 wrong!分析出错"<<endl;
				ErroR();exit(0);
			}
		}

	}while(S.size()!=0);
	//}
}

#endif // !PRINTER_H
