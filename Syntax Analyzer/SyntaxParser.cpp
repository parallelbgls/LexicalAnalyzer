#include "SyntaxParser.h"

SyntaxParser :: SyntaxParser(GrammarCollector* grammarCollector, string syntaxOutputErrorFile)
{
	acc = false;
	errorout.open(syntaxOutputErrorFile.c_str());
	grammarCollection = grammarCollector->GetGrammar();
	unTerminateSet = grammarCollector->GetUnTerminate();
	terminateSet = grammarCollector->GetTerminate();
	nullableSet = grammarCollector->GetNullableSet();
	syntaxParseTable = new map<pair<int,string>,ParseStep>();
	solvedGrammarStatus = new map<set<Grammar>,int>();
	unSolvedGrammarSet = new vector<set<Grammar> >();
	solvedFirstSet = new map<string,set<string> >();
	symbolStack = new stack<string>();
	markStack = new stack<int>();
	productions = new list<Grammar>();
	if (grammarCollection->size() > 0)
	{
		int i = 0;
		Grammar grammar((*grammarCollection)[0]);
		grammar.lookAhead = "$end";
		finalGrammar = new Grammar(grammar.grammarContent,-1);
		finalGrammar->lookAhead = "$end";
		finalGrammar->analyzedPos = finalGrammar->productionRule.size();
		set<Grammar> initialGrammarSet;
		initialGrammarSet.insert(grammar);
		set<Grammar> eClosure = SolveEClosure(initialGrammarSet);
		unSolvedGrammarSet->push_back(eClosure);
		solvedGrammarStatus->insert(pair<set<Grammar>,int>(eClosure,i));
		SolveEndGrammar(i);
		do		
		{
			set<string>::iterator symbol_pIter;
			for (symbol_pIter = unTerminateSet->begin(); symbol_pIter != unTerminateSet->end(); symbol_pIter++)
			{
				set<Grammar> gotoState = SolveGotoParse(i, *symbol_pIter);
				if (gotoState.size() > 0)
				{						
					if (solvedGrammarStatus->find(gotoState) == solvedGrammarStatus->end())
					{
						int push_pos = solvedGrammarStatus->size();
						solvedGrammarStatus->insert(pair<set<Grammar>,int>(gotoState,push_pos));
						unSolvedGrammarSet->push_back(gotoState);
						SolveEndGrammar(push_pos);
						if (syntaxParseTable->find(pair<int,string>(i,*symbol_pIter)) == syntaxParseTable->end())
						{
							ParseStep parseStep(PARSE_GOTO,push_pos);
							syntaxParseTable->insert(pair<pair<int,string>,ParseStep>(pair<int,string>(i,*symbol_pIter),parseStep));
						}
					}
					else
					{
						ParseStep parseStep(PARSE_GOTO,solvedGrammarStatus->find(gotoState)->second);
						syntaxParseTable->insert(pair<pair<int,string>,ParseStep>(pair<int,string>(i,*symbol_pIter),parseStep));
					}				
				}
			}
			for (symbol_pIter = terminateSet->begin(); symbol_pIter != terminateSet->end(); symbol_pIter++)
			{
				set<Grammar> gotoState = SolveGotoParse(i, *symbol_pIter);
				if (gotoState.size() > 0)
				{
					if (solvedGrammarStatus->find(gotoState) == solvedGrammarStatus->end())
					{
						int push_pos = solvedGrammarStatus->size();
						solvedGrammarStatus->insert(pair<set<Grammar>,int>(gotoState,push_pos));
						unSolvedGrammarSet->push_back(gotoState);
						SolveEndGrammar(push_pos);
						if (syntaxParseTable->find(pair<int,string>(i,*symbol_pIter)) == syntaxParseTable->end())
						{
							ParseStep parseStep(PARSE_SHIFT,push_pos);
							syntaxParseTable->insert(pair<pair<int,string>,ParseStep>(pair<int,string>(i,*symbol_pIter),parseStep));
						}
						else
						{
							cout<<"生成语法表时发生冲突，请检查错误输出文件"<<endl;
							errorout<<"shift reduce conflict, change shift to reduce"<<endl;
							ParseStep parseStep(PARSE_SHIFT,push_pos);
							syntaxParseTable->find(pair<int,string>(i,*symbol_pIter))->second = parseStep;
						}
					}
					else
					{
						ParseStep parseStep(PARSE_SHIFT,solvedGrammarStatus->find(gotoState)->second);
						syntaxParseTable->insert(pair<pair<int,string>,ParseStep>(pair<int,string>(i,*symbol_pIter),parseStep));
					}
				}
			}
			i++;
		}
		while (i != unSolvedGrammarSet->size());
	}
}

SyntaxParser::~SyntaxParser()
{
	errorout.close();
	delete(finalGrammar);
	delete(productions);
	delete(solvedGrammarStatus);
	delete(unSolvedGrammarSet);
	delete(syntaxParseTable);
	delete(solvedFirstSet);
	delete(symbolStack);
	delete(markStack);
}

set<Grammar> SyntaxParser::SolveEClosure(set<Grammar> solvingGrammar)
{
	queue<Grammar> unSolvedGrammar;
	set<Grammar> solvedGrammar;
	set<Grammar>::iterator solvingGrammar_pIter;
	for (solvingGrammar_pIter = solvingGrammar.begin();solvingGrammar_pIter != solvingGrammar.end();solvingGrammar_pIter++)
	{
		unSolvedGrammar.push(*solvingGrammar_pIter);
		solvedGrammar.insert(*solvingGrammar_pIter);
	}
	while (!unSolvedGrammar.empty())
	{
		Grammar solvingGrammar = unSolvedGrammar.front();
		unSolvedGrammar.pop();
		set<string> firstSet = SolveFirstSet(solvingGrammar);
		if (solvingGrammar.analyzedPos < (int)(solvingGrammar.productionRule.size()))
		{
			string pointPos = solvingGrammar.productionRule[solvingGrammar.analyzedPos];
			vector<Grammar>::iterator grammarCollection_pIter;
			if (unTerminateSet->find(pointPos) !=  unTerminateSet->end())
			{
				for (grammarCollection_pIter = grammarCollection->begin();grammarCollection_pIter != grammarCollection->end(); grammarCollection_pIter++)
				{
					if (grammarCollection_pIter->unTerminate == pointPos)
					{
						if (solvedGrammar.find(*grammarCollection_pIter) == solvedGrammar.end())
						{
							Grammar grammar(*grammarCollection_pIter);
							grammar.analyzedPos = 0;
							set<string>::iterator firstSet_pIter;
							for (firstSet_pIter = firstSet.begin();firstSet_pIter != firstSet.end();firstSet_pIter++)
							{
								grammar.lookAhead = *firstSet_pIter;
								if (solvedGrammar.find(grammar) == solvedGrammar.end())
								{
									unSolvedGrammar.push(grammar);
									solvedGrammar.insert(grammar);
								}
							}
						}
					}
				}
			}
		}
	}
	return solvedGrammar;
}

set<Grammar> SyntaxParser::SolveEClosure(int solvingGrammarPos)
{
	
	set<Grammar> tmpGrammar;
	tmpGrammar = (*unSolvedGrammarSet)[solvingGrammarPos];
	set<Grammar> solvedGrammar = SolveEClosure(tmpGrammar);
	return solvedGrammar;
}

void SyntaxParser::SolveEndGrammar(int solvingGrammarPos)
{
	set<Grammar> solvingGrammarSet = (*unSolvedGrammarSet)[solvingGrammarPos];
	set<Grammar>::iterator solvingGrammarSet_pIter;
	for (solvingGrammarSet_pIter = solvingGrammarSet.begin(); solvingGrammarSet_pIter != solvingGrammarSet.end(); solvingGrammarSet_pIter++)
	{
		if ((Grammar)(*solvingGrammarSet_pIter) == (*finalGrammar))
		{
			ParseStep parseStep(PARSE_ACC,solvingGrammarSet_pIter->id);
			syntaxParseTable->insert(pair<pair<int,string>,ParseStep>(pair<int,string>(solvingGrammarPos,solvingGrammarSet_pIter->lookAhead),parseStep));
		}
		else if (solvingGrammarSet_pIter->analyzedPos == solvingGrammarSet_pIter->productionRule.size())
		{
			ParseStep parseStep(PARSE_REDUCE,solvingGrammarSet_pIter->id);
			if (syntaxParseTable->find(pair<int,string>(solvingGrammarPos,solvingGrammarSet_pIter->lookAhead)) == syntaxParseTable->end())
			{
				syntaxParseTable->insert(pair<pair<int,string>,ParseStep>(pair<int,string>(solvingGrammarPos,solvingGrammarSet_pIter->lookAhead),parseStep));
			}
			else
			{
				if (syntaxParseTable->find(pair<int,string>(solvingGrammarPos,solvingGrammarSet_pIter->lookAhead))->second.method == PARSE_REDUCE)
				{
					cout<<"生成语法表时发生冲突，请检查错误输出文件"<<endl;
					errorout<<"reduce reduce conflict, ignore reduce"<<endl;
				}
				else
				{
					cout<<"生成语法表时发生冲突，请检查错误输出文件"<<endl;
					errorout<<"reduce shift conflict, ignore reduce"<<endl;
				}
			}
		}
	}
}

set<Grammar> SyntaxParser::SolveGotoParse(int solvingGrammarPos, string transitionSymbol)
{
	set<Grammar> p_grammarSet = (*unSolvedGrammarSet)[solvingGrammarPos];
	set<Grammar> gotoParse;
	set<Grammar>::iterator grammarSet_pIter;
	for (grammarSet_pIter = p_grammarSet.begin(); grammarSet_pIter != p_grammarSet.end(); grammarSet_pIter++)
	{
		if (grammarSet_pIter->analyzedPos < (int)(grammarSet_pIter->productionRule.size()))
		{
			if (grammarSet_pIter->productionRule[grammarSet_pIter->analyzedPos] == transitionSymbol)
			{
				Grammar grammar(*grammarSet_pIter);
				grammar.analyzedPos++;
				gotoParse.insert(grammar);
			}
		}
	}
	return SolveEClosure(gotoParse);
}

//警告，当前算法假定没有表达式会产生空
set<string> SyntaxParser::SolveFirstSet(Grammar grammar)
{
	set<string> firstSet;
	vector<string> analyzeStr;
	for (int i = grammar.analyzedPos+1; i < (int)(grammar.productionRule.size()); i++)
	{
		analyzeStr.push_back(grammar.productionRule[i]);
	}
	analyzeStr.push_back(grammar.lookAhead);
	for (int i = 0; i < (int)(analyzeStr.size()); i++)
	{
		if (unTerminateSet->find(analyzeStr[i]) != unTerminateSet->end())
		{
			vector<Grammar>::iterator grammarCollection_pIter;
			for (grammarCollection_pIter = grammarCollection->begin();grammarCollection_pIter != grammarCollection->end(); grammarCollection_pIter++)
			{
				if (grammarCollection_pIter->unTerminate == analyzeStr[i])
				{
					set<string> hasSolvedUnterminate;
					set<string> subFirstSet = SolveFirstSet(analyzeStr[i],hasSolvedUnterminate);
					firstSet.insert(subFirstSet.begin(),subFirstSet.end());
				}
			}
		}
		else
		{
			firstSet.insert(analyzeStr[i]);
		}
		if (nullableSet->empty() || ((!nullableSet->empty()) && nullableSet->find(analyzeStr[i]) == nullableSet->end()))
		{
			break;
		}
	}
	return firstSet;
}

set<string> SyntaxParser::SolveFirstSet(string solveAlpha, set<string>& hasSolvedUnterminate)
{
	set<string> firstSet;
	vector<Grammar>::iterator grammarCollection_pIter;
	if (terminateSet->find(solveAlpha) !=terminateSet->end())
	{
		firstSet.insert(solveAlpha);
	}
	else 
	{
		hasSolvedUnterminate.insert(solveAlpha);
		for (grammarCollection_pIter = grammarCollection->begin();grammarCollection_pIter != grammarCollection->end(); grammarCollection_pIter++)
		{
			if (grammarCollection_pIter->unTerminate == solveAlpha)
			{
				if (grammarCollection_pIter->productionRule.size() != 0)
				{
					if (hasSolvedUnterminate.find(grammarCollection_pIter->productionRule[0]) == hasSolvedUnterminate.end())
					{
						set<string> subFirstSet;
						if (solvedFirstSet->find(solveAlpha) == solvedFirstSet->end())
						{
							subFirstSet = SolveFirstSet(grammarCollection_pIter->productionRule[0],hasSolvedUnterminate);
						}
						else
						{
							subFirstSet = solvedFirstSet->find(solveAlpha)->second;
						}
						firstSet.insert(subFirstSet.begin(),subFirstSet.end());
					}
				}
			}
		}
		if (solvedFirstSet->find(solveAlpha) == solvedFirstSet->end())
		{
			solvedFirstSet->insert(pair<string,set<string> >(solveAlpha,firstSet));
		}
	}
	return firstSet;
}

void SyntaxParser::RunFile(string filePath, string recognizeTokenFilePath)
{
	ifstream fin2;
	fin2.open(recognizeTokenFilePath.c_str());
	set<string> tokenSet;
	while (!fin2.eof())
	{
		string tokenf;
		fin2>>tokenf;
		if (tokenf != "")
			tokenSet.insert(tokenf);
	}
	markStack->push(0);
	int posrow,poscol;
	string source,token;
	ifstream fin;
	fin.open(filePath.c_str());
	while (!fin.eof())
	{
		fin>>posrow>>poscol>>source>>token;
		if (tokenSet.find(token) != tokenSet.end())  
		{
			if (!Run(posrow,poscol,source))
			{
				return;
			}
		}
		else
		{
			if (!Run(posrow,poscol,token))
			{
				return;
			}
		}
		token = "$end";
		if (!fin.eof() && acc)
		{
			errorout<<"File execute hasn't completed but syntax parser has been completed, please check sourceFile"<<endl;
			break;
		}
	}
	fin.close();
}

bool SyntaxParser::Run(int row, int col, string token)
{
	
	if (!acc)
	{
		if (syntaxParseTable->find(pair<int,string>(markStack->top(),token)) == syntaxParseTable->end())
		{
			errorout<<"Shift error in row:" << row <<" and col:" << col <<" and token:" << token <<", Please check the source file"<<endl;
			while (syntaxParseTable->find(pair<int,string>(markStack->top(),token)) == syntaxParseTable->end() && (!symbolStack->empty()))
			{
				markStack->pop();
				symbolStack->pop();
			}
			if (symbolStack->empty())
			{
				errorout<<"There is an unfixable error in row: "<<row<<" and col: "<<col<<", Please check the source file"<<endl;
				return false;
			}
			SolveParse(row, col, token);
			
		}
		else
		{
			SolveParse(row, col, token);
		}
	}
	return true;
}

void SyntaxParser::SolveParse(int row, int col, string token)
{
	ParseMethod parseToWhat;
	do 
	{
				
		ParseStep parseStep = syntaxParseTable->find(pair<int,string>(markStack->top(),token))->second;
		parseToWhat = parseStep.method;
		switch (parseStep.method)
		{
		case PARSE_SHIFT:
			{
				markStack->push(parseStep.parseToWhat);
				symbolStack->push(token);
				break;
			}
		case PARSE_REDUCE:
			{
				Grammar grammar = (*grammarCollection)[parseStep.parseToWhat];
				for (int i = grammar.productionRule.size()-1; i>=0; i--)
				{
					assert(grammar.productionRule[i] == symbolStack->top());
					symbolStack->pop();
					markStack->pop();
				}
				if (syntaxParseTable->find(pair<int,string>(markStack->top(),grammar.unTerminate)) == syntaxParseTable->end())
				{
					errorout<<"Reduce error in row:" << row <<" and col:" << col <<" and token:" << token <<", Please check the source file";
				}
				else
				{
					assert(syntaxParseTable->find(pair<int,string>(markStack->top(),grammar.unTerminate))->second.method == PARSE_GOTO);
					markStack->push(syntaxParseTable->find(pair<int,string>(markStack->top(),grammar.unTerminate))->second.parseToWhat);
					symbolStack->push(grammar.unTerminate);
					productions->push_back(grammar);
					break;
				}
			}
		case PARSE_ACC:
			{
				acc = true;
				break;
			}
		default:
			{
				assert(false);
			}
		}
	}
	while (parseToWhat == PARSE_REDUCE);
}

void SyntaxParser::PrintProductions(string printFilePath)
{
	ofstream fout;
	fout.open(printFilePath.c_str());
	list<Grammar> grammarList = *productions;
	grammarList.reverse();
	list<Grammar>::iterator grammarList_pIter;
	for (grammarList_pIter = grammarList.begin(); grammarList_pIter != grammarList.end(); grammarList_pIter++)
	{
		fout<<grammarList_pIter->id<<" "<<grammarList_pIter->grammarContent<<endl;
	}
	fout.close();
}
