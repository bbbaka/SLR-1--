#include"scanner.h"
//LR_genertor.cpp
#include"dfa_generator.h"
//#include"LR(0).h"
#include"SLR(1).h"
#include"printer.h"
#include<time.h>  
/*
	1、本文法无法识别 char str[10]="abcdefg"的变量声明
	   请使用char str[10];str="abcdefg";代替
	
	2、函数体中，1）形式参数名称不能省略。
				 2）形式参数不允许有默认值。
				 3）数组参数必须指定数组长度。

	3、bug:敷衍的解决方法，在source中改写位if(a%b==0 ) 
		if(a%b==0) 经过词法分析器后，会变成 if lp a mod b equ rp rp 
		原因：疑是字符控制不合理；
		已解决：在对十进制0的识别中，多回退了一个字节；

	4、a--,--a报错 已修复。原因：文法中对终结符定义时拼写错了。。。decrese




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
	 clock_t startTime,endTime;  
    startTime = clock();  
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

	endTime = clock();  
    cout << "Totle Time : " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;  
    system("pause");  
    return 0;  
}

