#include<fstream>
#include<string>
#include"DFA.h"
#include<map>
#include<list>
#include<iostream>
using namespace std;

class Expression
{
protected:
	//不需要参与自动机检测的字母集
	map<string,DFA*>* letter;
	//正则表达式自动机
	map<string,DFA*>* regularExpression;
	//抢占符号集
	list<string>* grabLetter;
	//非抢占符号集
	list<string>* unGrabLetter;
	//正则表达式插入位置
	int insertInto;
	//自动机个数
	int dfaCount;
public:
	Expression();
	Expression(string title);
	~Expression();
	void Insert(string expression, int previousNum);
	map<string,DFA*>* GetLetter()
	{
		return letter;
	}
	map<string,DFA*>* GetRegularExpression()
	{
		return regularExpression;
	}
	list<string>* GetGrabLetter()
	{
		return grabLetter;
	}
	list<string>* GetUnGrabLetter()
	{
		return unGrabLetter;
	}
	string CheckWord(string word);
};