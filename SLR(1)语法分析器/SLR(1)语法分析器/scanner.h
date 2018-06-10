#ifndef SCANNER_H
#define SCANNER_H

#include<string>
#include<sstream>
#include<iostream>
#include<fstream>
#include<iomanip>
using namespace std;

FILE *fp=fopen("d:\\source.txt","r");
ofstream op("d:\\output.txt");

#define GetChar() Get_Char(fp)
#define UnGetC() ungetc(ch,fp);
#define Token _Token
#define NUM 18
string keywords[NUM]=
{
	"int",			//1
	"char",			//2
	"void",			//3
	"extern",		//..
	"if",
	"else",
	"switch",
	"case",
	"default",
	"while",
	"do",
	"for",
	"break",
	"continue",
	"return"
	//"main",
	//"string",
	//"then",
	//"begin",
	//"end",
	//"bool"
	//"struct"
};

static int rol=0;
static int col=0;

char Get_Char(FILE *fp)	//扫描器
{
	char ch=fgetc(fp);
	if(ch=='\n')
	{
		rol++;
		col=0;
	}
	col++;
	return ch;
}

class _Token
{
public:
	string value;
	string type;
	static int ss;
	Token()
	{
		value="",type="";
	};
	Token(string str1,string str2){
		type=str1;
		value=str2;
	}
	void check()
	{
		if(type=="id")		//区分标识符中的保留字，
		{
			for(int i=0;i<NUM;i++)
			{
				if(value==keywords[i])
				{
					type=keywords[i];
					value=to_string(i+1);
					break;
				}
			}
		}
	}
	void print()
	{
		check();
		if(type!=""&&value!="")
			if(type!="id")	//如果是变量，则为他给他一个ID编号（入口地址 ss可以改为0x20000000作为数据段首地址之类的）
			{
				if(type=="decimal"||type=="Hexadecimal"||type=="binary"||type=="Octal")
				{
					op<<left<<setw(15)<<"num"<<left<<setw(10)<<ss++<<left<<setw(10)<<value<<endl;
				}
				else if(type=="_char")
				{
					op<<left<<setw(15)<<"ch"<<left<<setw(10)<<ss++<<left<<setw(10)<<value<<endl;
				}
				else if(type=="_string")
				{
					op<<left<<setw(15)<<"str"<<left<<setw(10)<<ss++<<left<<setw(10)<<value<<endl;
				}
				else
				{
					op<<left<<setw(15)<<type<<left<<setw(10)<<value<<left<<setw(10)<<"null"<<endl;
				}

			}
			else
			{
				op<<left<<setw(15)<<"id"<<left<<setw(10)<<ss++<<left<<setw(10)<<value<<endl;
			}
		
	}

};

void error(){			//错误报告位置
	cout<<"There is an error in line:"<<rol<<endl;//"col:"<<col<<endl;
	exit(1);
}

void m_com(){			//块注释自动机 的一段递归小函数
	char ch;
	ch=GetChar();
	if(ch==-1)
		error();
	while(ch!='*')
		ch=GetChar();
	while(ch=='*')
		ch=GetChar();
	if(ch!='/')
		m_com();
	else
		return;
}

Token * IsIdentify(char ch)
{
	string token="";
	if(ch>='a'&&ch<='z'||ch>='A'&&ch<='Z'||ch=='_')		
	{
		token=token+ch;
		ch=GetChar();
		while(ch>='a'&&ch<='z'||ch>='A'&&ch<='Z'||ch=='_'||ch>='0'&&ch<='9')
		{
			token+=ch;
			ch=GetChar();
		}
		UnGetC();
		return new Token("id",token);
	}
	//到达文件尾，返回空
	if(ch==-1)
	{
		UnGetC();
		return new Token();
	}
	return NULL;
}

Token * IsNum(char ch)
{
	string token="";
	//十进制整数
	if(ch>='1'&&ch<='9')			
	{
		token+=ch;
		ch=GetChar();
		while(ch>='0'&&ch<='9')
		{
			token+=ch;
			ch=GetChar();
		}
		UnGetC();
		return new Token("decimal",token);
	}
	
	//当数字首位为0时，可能为十进制0，八进制，十六进制，二进制
	if(ch=='0')				
	{
		token+=ch;
		ch=GetChar();
		if(ch=='x')
		{
			token+=ch;
			ch=GetChar();
			if(ch>='0'&&ch<='9'||ch>='A'&&ch<='F'||ch>='a'&&ch<='f')
			{
				while(ch>='0'&&ch<='9'||ch>='A'&&ch<='F'||ch>='a'&&ch<='f')
				{
					token+=ch;
					ch=GetChar();
				}
				UnGetC();
				return new Token("Hexadecimal",token);
			}
			UnGetC();
			error();
		}
		else if(ch=='b')
			{
				token+=ch;
				ch=GetChar();
				while(ch=='0'||ch=='1')
				{
					token+=ch;
					ch=GetChar();
				}
				UnGetC();
				return new Token("binary",token);
			}
			else
			{
				if(ch>='0'&&ch<='7')
				{
					while(ch>='0'&&ch<='7')
					{
						token+=ch;
						ch=GetChar();
					}
					UnGetC();
					return new Token("Octal",token);
				}
			}
		UnGetC();
		return new Token("decimal","0");		//值为0的十进制整数
	}
	//到达文件尾，返回空
	if(ch==-1)
	{
		UnGetC();
		return new Token();
	}
	return NULL;
}

Token * IsChar(char ch)
{
	string token="";
	//char常量
	if(ch=='\'')		
	{
		ch=GetChar();
		if(ch=='\''||ch=='\n'||ch==-1)
		{
			error();							//可以通过记录当前读取的行号以返回错误位置
			return NULL;
		}
		if(ch=='\\')				
		{	
			ch=GetChar();
			if(ch=='\n'||ch=='\\')
			{
				error();
				return NULL;
			}
		}
		token+=ch;
		ch=GetChar();
		if(ch=='\'')
			return new Token("_char",token);
		else
		{
			error();
			return NULL;
		}
	}
	//到达文件尾，返回空
	if(ch==-1)
	{
		UnGetC();
		return new Token();
	}
	return NULL;
}

Token * IsString(char ch)
{
	string token="";
	//string 常量
	if(ch=='"')
	{
		ch=GetChar();
		if(ch=='\\')
		{
			ch=GetChar();
			if(ch==-1||ch=='\n')
			{	
				error();
				return NULL;
			}
			token+=ch;
		}
		while(ch!='"')	
		{
			if(ch=='\\')
			{
				ch=GetChar();
			}
			token+=ch;
			ch=GetChar();
		}
		if(ch=='\n')
		{
			error();
			return NULL;
		}
		if(ch=='"')
			return new Token("_string",token);
	}
	//到达文件尾，返回空
	if(ch==-1)
	{
		UnGetC();
		return new Token();
	}
	return NULL;
}

Token * IsSign(char ch)
{
	string token="";
	//以下为界符,若有增加，请写于文本末，并使返回的value值+1；
	if(ch=='%')
		return new Token("mod",to_string(NUM+1));
	if(ch=='>')
	{
		ch=GetChar();
		if(ch=='=')
			return new Token("ge",to_string(NUM+2));
		UnGetC();
		return new Token("gt",to_string(NUM+3));
	}

	//除号、行注释、块注释
	if(ch=='/')
	{
		ch=GetChar();
		if(ch=='/')
		{
			while(ch!='\n'&&ch!=-1)
				ch=GetChar();
			if(ch=='\n'||ch==-1)
				return new Token("s_com",to_string(NUM+4));
		}
		if(ch='*')	
		{
			m_com();
			return new Token("m_com",to_string(NUM+5));
		}
		 
		{
			UnGetC();
			return new Token("div",to_string(NUM+6));
		}
	}


	if(ch=='+')
	{
		ch=GetChar();
		if(ch=='+')
			return new Token("increase",to_string(NUM+7));
		UnGetC();
		return new Token("add",to_string(NUM+8));
	}
	if(ch=='=')
	{
		ch=GetChar();
		if(ch=='=')
			return new Token("equal",to_string(NUM+9));
		UnGetC();
		return new Token("assign",to_string(NUM+10));
	}
	if(ch=='-')
	{
		ch=GetChar();
		if(ch=='-')
			return new Token("decrease",to_string(NUM+11));
		UnGetC();
		return new Token("sub",to_string(NUM+12));
	}
	if(ch=='<')
	{
		ch=GetChar();
		if(ch=='=')
			return new Token("le",to_string(NUM+13));
		UnGetC();
		return new Token("lt",to_string(NUM+14));
	}
	if(ch=='!')
	{
		ch=GetChar();
		if(ch=='=')
			return new Token("nequ",to_string(NUM+15));
		UnGetC();
		return new Token("not",to_string(NUM+16));
	}
	if(ch=='&')
	{
		ch=GetChar();
		if(ch=='&')
			return new Token("and",to_string(NUM+17));
		UnGetC();
		return new Token("lea",to_string(NUM+18));
	}
	if(ch=='|')
	{
		ch=GetChar();
		if(ch=='|')
			return new Token("or",to_string(NUM+19));
		UnGetC();
		//return new Token("BitwiseOr",to_string(NUM+20)); 爸爸不要你了
		error();
	}
	if(ch==',')
		return new Token("comma",to_string(NUM+21));
	if(ch==';')
		return new Token("semicon",to_string(NUM+22));
	if(ch==':')
		return new Token("colon",to_string(NUM+23));
	if(ch=='(')
		return new Token("lparen",to_string(NUM+24));
	if(ch==')')
		return new Token("rparen",to_string(NUM+25));
	if(ch=='{')
		return new Token("lbrac",to_string(NUM+26));
	if(ch=='}')
		return new Token("rbrac",to_string(NUM+27));
	if(ch=='*')
		return new Token("mul",to_string(NUM+28));
	if(ch=='[')
		return new Token("lbrack",to_string(NUM+29));
	if(ch==']')
		return new Token("lbrack",to_string(NUM+30));
	// 待添加的其他界符、运算符

	//到达文件尾，返回空
	if(ch==-1)
	{
		UnGetC();
		return new Token();
	}
	return NULL;
}

Token * scanner(){				//略去注释和空格 输出单词二元式
	string token="";
	char ch=GetChar();
	Token * t=new Token();

	//忽略空格 换行符 制表符
	while(ch==' '||ch=='\n'||ch=='\t')
		ch=GetChar();

	t=IsIdentify(ch);
	if(t)
		return t;

	t=IsNum(ch);
	if(t)
		return t;

	t=IsChar(ch);
	if(t)
		return t;

	t=IsString(ch);
	if(t)
		return t;

	t=IsSign(ch);
	if(t)
		return t;
	
	return t;
}

#endif // !SCANNER_H
