#include"scanner.h"
//LR_genertor.cpp
#include"dfa_generator.h"
//#include"LR(0).h"
#include"SLR(1).h"
#include"printer.h"
/*
	声明变量时无法同时初始化；
	
	表达式文法有问题 明天再研究研究










*/
void input_strarray(ifstream &input,string v[],int vnum)
{
	int i;
	for(i=0;i<vnum;i++)
		input>>v[i];
}

int _Token::ss=10000;

void input_strarray(ifstream &input,string v[])//将Ex Ex ++ Tx填入v[]中。遇到换行符时停止；
{
	int i;
	//input>>v[0];
	for(i=0;;i++)	
	{	
		input>>v[i];
		if(input.get()=='\n'||input.get()==EOF)
			break;
		input.seekg(-1,ios::cur);
	}
}

int main()
{
	{
		Token *t;
		do
		{
			t=scanner();
			t->print();
		}while(t->type!=""&&t->value!="");
		op<<left<<setw(15)<<"#"<<left<<setw(10)<<0<<left<<setw(10)<<"null"<<endl;;
	}

	int i,j;
	ifstream in("d:\\文法.txt",ios_base::in);//读文件,从文件中读入pronum,vtnum,vnnum以及产生式
	in>>pronum>>vnnum>>vtnum;
	//in>>vn;	//要求以文法开始符为开头
	//in>>vt;
	input_strarray(in,vn,vnnum);
	input_strarray(in,vt,vtnum);

	in.get();
	for(i=1;i<=pronum;i++)
		//in>>old[i];//将产生式存入old[][],old[1]为第一个产生式,且第一个产生式左部必须为文法开始符
			input_strarray(in,old[i]);
	for(i=1;i<=pronum;i++)
		for(j=0;old[i][j]!="";j++)
			oldz[i][j]=old[i][j];//将产生式从old[][]录入oldz[][]
	oldz[0][0]="$";
	oldz[0][1]=old[1][0];//加入P->S，将原文法扩充，使其变为增广文法

	//	vn[vnnum]='P';
	vt[vtnum]="#";//把结束符'#'加入终结符集

	D.cd_num=0;
	for(i=0;i<=N;i++)
	{
		D.s[i].item_num=0;
		D.s[i].link_num=0;
	}//初始化状态个数、连接个数、项目个数

	dfa();
	action();
	go_answer();
	printf_ag();

	Controler();
}

