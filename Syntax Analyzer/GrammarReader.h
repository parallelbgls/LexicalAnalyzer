#include "GrammarCollector.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class GrammarReader
//��ȡContextFreeGrammar.txt�ļ�,ת��ΪGrammarCollector
{
private:
	GrammarCollector* grammarCollector;

public:	
	GrammarReader(string contextFreeGrammarSourceFile);
	~GrammarReader();

	GrammarCollector* GetGrammarCollector()
	{
		return grammarCollector;
	}
};
