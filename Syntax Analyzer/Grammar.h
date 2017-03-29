#ifndef __GRAMMAR
#define __GRAMMAR
#include <string>
#include <vector>
using namespace std;

struct Grammar
{
public:
	string grammarContent;   //�����Ĳ���ʽ�ַ���
	int id;   //�ķ��ı��
	string unTerminate;    //�ķ���ߵķ��ս��
	vector<string> productionRule;      //�ķ��ұߵĲ���ʽ
	int analyzedPos;    //�Բ���ʽĿǰ��������λ��
	string lookAhead;    //����ʽ����һ����������

	Grammar(string grammarRule, int id)
	{
		grammarContent = grammarRule;
		int ruleSymbolPos = grammarRule.find_first_of("->");
		unTerminate = grammarRule.substr(0,ruleSymbolPos-1);
		grammarRule = grammarRule.substr(ruleSymbolPos+3);
		int spacePos = grammarRule.find_first_of(' ');
		while (spacePos != grammarRule.npos)
		{
			if (spacePos != 0)
			{
				productionRule.push_back(grammarRule.substr(0,spacePos));
			}
			grammarRule = grammarRule.substr(spacePos+1);
			spacePos = grammarRule.find_first_of(' ');
		}
		if (grammarRule != "")
		{
			productionRule.push_back(grammarRule);
		}
		this->id = id;
		analyzedPos = 0;
		lookAhead = "";
		if (productionRule.size() == 1 && productionRule[0] == "$end")
		{
			productionRule.clear();
		}
	}

	bool operator== (Grammar &comparedGrammar) const
	{
		if (grammarContent == comparedGrammar.grammarContent && analyzedPos == comparedGrammar.analyzedPos && lookAhead == comparedGrammar.lookAhead)
			return true;
		else
			return false;
	}

	bool operator!= (Grammar &comparedGrammar) const
	{
		if (grammarContent != comparedGrammar.grammarContent || analyzedPos != comparedGrammar.analyzedPos || lookAhead != comparedGrammar.lookAhead)
			return true;
		else
			return false;
	}

	bool operator< (Grammar &comparedGrammar) const
	{
		if (grammarContent.compare(comparedGrammar.grammarContent) < 0)
			return true;
		else if (grammarContent == comparedGrammar.grammarContent && analyzedPos < comparedGrammar.analyzedPos)
			return true;
		else if (grammarContent == comparedGrammar.grammarContent && analyzedPos == comparedGrammar.analyzedPos && lookAhead.compare(comparedGrammar.lookAhead) < 0)
			return true;
		else 
			return false;
	}

	bool operator> (Grammar &comparedGrammar) const
	{
		if (grammarContent.compare(comparedGrammar.grammarContent) > 0)
			return true;
		else if (grammarContent == comparedGrammar.grammarContent && analyzedPos > comparedGrammar.analyzedPos)
			return true;
		else if (grammarContent == comparedGrammar.grammarContent && analyzedPos == comparedGrammar.analyzedPos && lookAhead.compare(comparedGrammar.lookAhead) > 0)
			return true;
		else 
			return false;
	}

	bool operator< (const Grammar &comparedGrammar) const 
	{
		if (grammarContent.compare(comparedGrammar.grammarContent) < 0)
			return true;
		else if (grammarContent == comparedGrammar.grammarContent && analyzedPos < comparedGrammar.analyzedPos)
			return true;
		else if (grammarContent == comparedGrammar.grammarContent && analyzedPos == comparedGrammar.analyzedPos && lookAhead.compare(comparedGrammar.lookAhead) < 0)
			return true;
		else 
			return false;
	}

	bool operator> (const Grammar &comparedGrammar) const
	{
		if (grammarContent.compare(comparedGrammar.grammarContent) > 0)
			return true;
		else if (grammarContent == comparedGrammar.grammarContent && analyzedPos > comparedGrammar.analyzedPos)
			return true;
		else if (grammarContent == comparedGrammar.grammarContent && analyzedPos == comparedGrammar.analyzedPos && lookAhead.compare(comparedGrammar.lookAhead) > 0)
			return true;
		else 
			return false;
	}
};

#endif
