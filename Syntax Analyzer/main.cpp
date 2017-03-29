#include "GrammarCollector.h"
#include "Analyzer.h"
#include <iostream>


using namespace std;

int main()
{
	Analyzer *analyzer = new Analyzer("ContextFreeGrammar.txt","..\\Lexical Analyzer\\LexicalFile.txt","SyntaxFile.txt","SyntaxErrorFile.txt");
	delete(analyzer);
	cout<<"·ÖÎöÍê±Ï"<<endl;
	cin.get();
}
