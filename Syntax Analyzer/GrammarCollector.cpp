#include "GrammarCollector.h"

GrammarCollector::GrammarCollector()
{
	grammarCollection = new vector<Grammar>();
	unTerminateSet = new set<string>();
	terminateSet = new set<string>();
	nullable = new set<string>();
}

GrammarCollector::~GrammarCollector()
{
	delete(grammarCollection);
	delete(unTerminateSet);
	delete(terminateSet);
	delete(nullable);
}

bool GrammarCollector::Insert(Grammar grammar)
{
	for (int i=0 ;i<(int)(grammarCollection->size());i++)
	{
		if ((*grammarCollection)[i] == grammar)
		{
			return false;
		}
	}
	for (int i = 0; i<(int)(grammar.productionRule.size()); i++)
	{
		if (grammar.productionRule[i] == "$end")
		{
			return false;
		}
	}
	grammarCollection->push_back(grammar);
	unTerminateSet->insert(grammar.unTerminate);
	if (terminateSet->find(grammar.unTerminate) != terminateSet->end())
	{
		terminateSet->erase(grammar.unTerminate);
	}
	if (grammar.productionRule.size() == 0)
	{
		nullable->insert(grammar.unTerminate);
	}
	else for (int i = 0; i<(int)(grammar.productionRule.size()); i++)
	{
		if (unTerminateSet->find(grammar.productionRule[i]) == unTerminateSet->end())
		{
			terminateSet->insert(grammar.productionRule[i]);
		}
	}
	return true;
}

bool GrammarCollector::Insert(string grammarString)
{
	for (int i=0;i<(int)(grammarCollection->size());i++)
	{
		if ((*grammarCollection)[i].grammarContent == grammarString)
		{
			return false;
		}
	}
	Grammar grammar(grammarString,grammarCollection->size());
	Insert(grammar);
	return true;
}
