#include "Analyzer.h"

Analyzer::Analyzer(string contextFreeGrammarFile, string lexicalSourceFile, string syntaxOutputFile, string syntaxOutputErrorFile)
{
	grammarReader = new GrammarReader(contextFreeGrammarFile);
	syntaxParser = new SyntaxParser(grammarReader->GetGrammarCollector(),syntaxOutputErrorFile);
	syntaxParser->RunFile(lexicalSourceFile,"IsThirdRecognizeTokenFile.txt");
	syntaxParser->PrintProductions(syntaxOutputFile);
}

Analyzer::~Analyzer()
{
	delete(grammarReader);
	delete(syntaxParser);
}

