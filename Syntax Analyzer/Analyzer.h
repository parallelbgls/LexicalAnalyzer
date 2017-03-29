#include "GrammarReader.h"
#include "SyntaxParser.h"

class Analyzer
//分析器，与词法相同
{
protected:
	GrammarReader *grammarReader;
	SyntaxParser *syntaxParser;

public:
	Analyzer(string contextFreeGrammarFile, string lexicalSourceFile, string syntaxOutputFile, string syntaxOutputErrorFile);
	~Analyzer();
};
