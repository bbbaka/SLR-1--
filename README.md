# SLR(1)语法分析器

实现对C语言子集的语法分析。

对输入的源程序的限制：
1、本文法无法识别 `char str[10]="abcdefg"`的变量声明，请使用`char str[10];str="abcdefg";`代替
	
2、函数体中，1）形式参数名称不能省略。 2）形式参数不允许有默认值。3）数组参数必须指定数组长度。
         
对输入的文法的要求：1）第一行为三个整型值，分别表示产生式数目pronum、VN数目vnnum、VT数目vtnum
                  2）第二行为vnnum个字符串，表示文法中出现过的全部非终结符，并要求文法开始符作为第一个字符串
                  3）第三行为vtnum个字符串，表示文法中（并且是词法分析器所能识别的）全部终结符
                  4）剩下pronum行表示产生式，由产生式的形式，左部通常只有一个符号，因此以位置关系代替符号"->"（定义为）
                  5）本语法分析器无法识别带空字的文法（因为我不想写了。。），因此输入的文法需稍做处理，用等价的产生式约去空字
                  
输出：output.txt log.txt
output.txt是词法分析程序的形成的单词表，其中以（type,key,value）形式保存单词，type表示种别的助记符，key表示键值，value表示变量或常量的值（实际上key和value我都没用哈哈哈）
log.txt是语法分析程序过程中的一些内容，包括FIRST、FOLLOW集，ACTION、GOTO表，以及对源程序的SLR分析过程。便于调试。

​    
