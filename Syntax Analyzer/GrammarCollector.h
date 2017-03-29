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
	vector<Grammar>* grammarCollection;  //�������޹��ķ��ļ���
	set<string>* unTerminateSet;  //���ս������
	set<string>* terminateSet;    //�ս������
	set<string>* nullable; 

public:
	GrammarCollector();
	~GrammarCollector();
	bool Insert(Grammar grammar);  //����һ������ʽ
	bool Insert(string grammarString);   //����һ������ʽ���ַ��������ò���ʽ��Insert
	vector<Grammar>* GetGrammar()   //��ȡ�������޹��ķ��ļ���
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
