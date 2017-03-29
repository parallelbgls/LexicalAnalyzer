#ifndef __GRAMMARCOLLECTOR
#define __GRAMMARCOLLECTOR

#include "Grammar.h"
#include <vector>
#include <string>
#include <set>
#include <map>
using namespace std;

class GrammarCollector
{
protected:
	vector<Grammar>* grammarCollection;  //上下文无关文法的集合
	set<string>* unTerminateSet;  //非终结符集合
	set<string>* terminateSet;    //终结符集合
	set<string>* nullable; 

public:
	GrammarCollector();
	~GrammarCollector();
	bool Insert(Grammar grammar);  //插入一个产生式
	bool Insert(string grammarString);   //插入一个产生式的字符串，利用产生式的Insert
	vector<Grammar>* GetGrammar()   //获取上下文无关文法的集合
	{
		return grammarCollection;
	}
	set<string>* GetUnTerminate()
	{
		return unTerminateSet;
	}
	set<string>* GetTerminate()
	{
		return terminateSet;
	}
	set<string>* GetNullableSet()
	{
		return nullable;
	}
};

#endif
