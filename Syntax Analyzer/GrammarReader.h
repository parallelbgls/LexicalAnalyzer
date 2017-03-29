#include "GrammarCollector.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class GrammarReader
//读取ContextFreeGrammar.txt文件,转化为GrammarCollector
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
