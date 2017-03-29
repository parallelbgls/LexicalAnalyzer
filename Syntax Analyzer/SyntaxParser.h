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
	bool acc;   //��ȡ���Ƿ��Ѿ�accept
	list<Grammar>* productions;   //��ȡ�������ķ�����
	map<set<Grammar>,int>* solvedGrammarStatus;
	vector<set<Grammar> >* unSolvedGrammarSet;
	map<pair<int,string>,ParseStep>* syntaxParseTable;   //�ķ�ת����
	stack<string>* symbolStack;   //����ջ
	stack<int>* markStack;        //���ջ
	map<string,set<string> >* solvedFirstSet;
	vector<Grammar>* grammarCollection;
	set<string>* unTerminateSet;
	set<string>* terminateSet;
	set<string>* nullableSet;
	bool Run(int row, int col, string token);           //����һ���ʷ���������������һ��,ע��action֮����acc
	set<Grammar> SolveGotoParse(int solvingGrammarPos, string transitionSymbol);
	set<Grammar> SolveEClosure(int solvingGrammarPos);
	set<Grammar> SolveEClosure(set<Grammar> solvingGrammar);
	void SolveEndGrammar(int solvingGrammarPos);
	set<string> SolveFirstSet(Grammar grammar);
	set<string> SolveFirstSet(string solveAlpha, set<string>& hasSolvedUnterminate);
	void SolveParse(int row, int col, string token);
	

public:
	SyntaxParser(GrammarCollector* grammarCollector, string syntaxOutputErrorFile);  //����һ��grammarCollection�������grammarCollection���ķ�ת����
	~SyntaxParser();
	void RunFile(string filePath, string recognizeTokenFilePath);        //ִ��һ���ʷ��ļ�
	bool IsAcc();                     //�ж��Ƿ��Ѿ�acc
	void PrintProductions(string printFilePath);    //���ز����ķ���˳��
	void Reset()                     //���÷�����
	{
		acc = false;
		delete(symbolStack);
		symbolStack = new stack<string>();
		delete(markStack);
		markStack = new stack<int>();
	}
};
