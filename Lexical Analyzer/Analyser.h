#include"Expression.h"
#include<fstream>
#include<list>
#include<string>

using namespace std;
const int LINE_READ = 300;

class Analyser
{
protected:
	//正则表达式
	Expression* expression;
	ifstream fin;
	ofstream fout;
	//行拆分后的单词链表
	list<Triple<int,int,string> >* wordList;
	list<Triple<int,int,string> > unknownWord;
	//当前读的行数
	int rowCount;
	//读文件
	bool ReadFile();
	//分析文件
	void Analyze();
	//写文件
	void WriteFile(int row, int col, string word, string dfaName);
	//拆分单词，包括拆分符号表，和单词链表当前所指的指针
	void DivideWord(list<string>* letterTable,list<Triple<int,int,string> >::iterator &wordList_pIter);
	void WriteUnknownWord();
public:
	//源文件地址，正则表达式文件地址，目标文件地址
	Analyser(string sourceFilePath, string regularExpressionPath, string destinationFile);
	~Analyser();
};
