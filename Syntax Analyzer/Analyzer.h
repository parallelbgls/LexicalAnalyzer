#include "GrammarReader.h"
#include "SyntaxParser.h"

class Analyzer
//����������ʷ���ͬ
{
protected:
	GrammarReader *grammarReader;
	SyntaxParser *syntaxParser;

public:
	Analyzer(string contextFreeGrammarFile, string lexicalSourceFile, string syntaxOutputFile, string syntaxOutputErrorFile);
	~Analyzer();
};
