#include "GrammarCollector.h"
#include "ParseStep.h"
#include <vector>
#include <list>
#include <map>
#include <stack>
#include <queue>
#include <set>
#include <fstream>      
#include <iostream>
#include <cassert>
using namespace std;

class SyntaxParser
{
protected:
	Grammar* finalGrammar;
	ofstream errorout;
	bool acc;   //读取后是否已经accept
	list<Grammar>* productions;   //读取产生的文法逆序
	map<set<Grammar>,int>* solvedGrammarStatus;
	vector<set<Grammar> >* unSolvedGrammarSet;
	map<pair<int,string>,ParseStep>* syntaxParseTable;   //文法转换表
	stack<string>* symbolStack;   //符号栈
	stack<int>* markStack;        //标号栈
	map<string,set<string> >* solvedFirstSet;
	vector<Grammar>* grammarCollection;
	set<string>* unTerminateSet;
	set<string>* terminateSet;
	set<string>* nullableSet;
	bool Run(int row, int col, string token);           //运行一个词法分析器分析出的一行,注意action之间检测acc
	set<Grammar> SolveGotoParse(int solvingGrammarPos, string transitionSymbol);
	set<Grammar> SolveEClosure(int solvingGrammarPos);
	set<Grammar> SolveEClosure(set<Grammar> solvingGrammar);
	void SolveEndGrammar(int solvingGrammarPos);
	set<string> SolveFirstSet(Grammar grammar);
	set<string> SolveFirstSet(string solveAlpha, set<string>& hasSolvedUnterminate);
	void SolveParse(int row, int col, string token);
	

public:
	SyntaxParser(GrammarCollector* grammarCollector, string syntaxOutputErrorFile);  //传入一个grammarCollection，构造该grammarCollection的文法转换表
	~SyntaxParser();
	void RunFile(string filePath, string recognizeTokenFilePath);        //执行一个词法文件
	bool IsAcc();                     //判断是否已经acc
	void PrintProductions(string printFilePath);    //返回产生文法的顺序
	void Reset()                     //重置分析器
	{
		acc = false;
		delete(symbolStack);
		symbolStack = new stack<string>();
		delete(markStack);
		markStack = new stack<int>();
	}
};
