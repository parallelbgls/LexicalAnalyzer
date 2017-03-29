#include"DFA.h"

DFA::DFA(string name, string expression, int previousNum)	
{
	this->name = name;
	this->expression = expression;
	this->previousNum = previousNum;
	dfa = new map<pair<int,char>,int>();
	status = new map<int,set<int> >();
	statusReverse = new map<set<int>,int>();
	expressionTree = new Tree();
	doubleStack = new DoubleStack();
	endState = new set<int>();
	bool forceLetter = false;
	//必须能让所有元素的followpos都能走到#，不然会发生悬空。
	expression = '(' + expression + ')' + '#';
	try
	{
		for(string::const_iterator iter = expression.begin(); iter != expression.end(); iter++)
		{
			char ch = *iter;
			if (ch == '\\')
			{
				forceLetter = true;
			}
			else
			{
				Node *node = new Node(ch);
				//强制字母压入双栈
				if (forceLetter)
				{
					if (ch == '$')
					{
						node->nullable = false;
					}
					queue<Triple<Node*,Node*,Node*> >* returnQueue = doubleStack->InsertLetter(node);
					BuildTree(returnQueue);
					forceLetter = false;
				}
				else
				{
					//符号压入符号栈
					if (ch == '(' || ch == ')' || ch == '|' || ch == '&' || ch == '*')
					{
						queue<Triple<Node*,Node*,Node*> >* returnQueue = doubleStack->InsertSymbol(node);
						BuildTree(returnQueue);
					}
					//字母压入字母栈
					else
					{
						queue<Triple<Node*,Node*,Node*> >* returnQueue = doubleStack->InsertLetter(node);
						BuildTree(returnQueue);
					}
				}
			}
		}
		//弹出所有还在栈中的元素
		queue<Triple<Node*,Node*,Node*> >* returnQueue = doubleStack->PopAll();
		//设置树中的根节点
		expressionTree->SetRoot(returnQueue->back().third);
		//建树
		BuildTree(returnQueue);
	}
	catch(DoubleStack *doubleStack)
	{
		throw this;
	}
	//添加树中的firstPos，lastPos，nullable
	FixTree(expressionTree);
	//求followPos
	followPos = new set<int>[expressionTree->GetLetterCount()+1];
	SolveFollowPos(expressionTree);
	//构建DFA
	ConstructDFA();
	//最小化DFA
	MinimizeDFA();
}

DFA::~DFA()
{
	delete(dfa);
	delete(status);
	delete(statusReverse);
	delete(expressionTree);
	delete(doubleStack);
	delete[](followPos);
	delete(endState);
}

void DFA::BuildTree(queue<Triple<Node*,Node*,Node*> >* returnQueue)
{
	while (!returnQueue->empty())
	{
		//取返回队列的首元素
		Triple<Node*,Node*,Node*> triple = returnQueue->front();
		//建左子树
		expressionTree->AddChild(triple.first,triple.third,leftChild);
		if (triple.second != NULL)
		{
			//建右子树
			expressionTree->AddChild(triple.second,triple.third,rightChild);
		}
		//弹出首元素
		returnQueue->pop();
	}
	delete(returnQueue);
}

void DFA::FixTree(Tree* tree)
{
	tree->Fix();
}

void DFA::SolveFollowPos(Tree* tree)
{
	Node* root = tree->GetRoot();
	SolveFollowPos(root);
}

void DFA::SolveFollowPos(Node* node)
{
	if (node->leftchild != NULL)
	{
		SolveFollowPos(node->leftchild);
		if (node->rightchild != NULL)
		{
			SolveFollowPos(node->rightchild);
			//节点是&，左子树的lastPos的每个节点的followPos有右子树的firstPos
			if (node->value == '&')
			{
				set<int>::iterator first_pIter;
				set<int>::iterator last_pIter;
				for (last_pIter = node->leftchild->lastpos->begin(); last_pIter != node->leftchild->lastpos->end(); last_pIter++)
				{
					for (first_pIter= node->rightchild->firstpos->begin(); first_pIter != node->rightchild->firstpos->end(); first_pIter++)
					{
						followPos[*last_pIter].insert(*first_pIter);
					}
				}
			}
			//节点是|，什么都不做
			else if (node->value == '|')
			{
			}
			else throw node;
		}
		else
		{
			//节点是*,儿子节点的每个lastPos节点的followPos都有儿子节点的firstPos
			if (node->value == '*')
			{
				set<int>::iterator first_pIter;
				set<int>::iterator last_pIter;
				for (last_pIter = node->lastpos->begin(); last_pIter != node->lastpos->end(); last_pIter++)
				{
					for (first_pIter= node->firstpos->begin(); first_pIter != node->firstpos->end(); first_pIter++)
					{
						followPos[*last_pIter].insert(*first_pIter);
					}
				}
			}
			else throw node;
		}
	}
}

void DFA::ConstructDFA()
{
	//字母值对应的位置
	map<char,set<int> >* letterMap;
	int mark = 0;
	int hasmarked = 0;
	beginState = mark;
	//初始可达字母
	set<int> firstPosRoot = *(expressionTree->GetRoot()->firstpos);
	//添加
	status->insert(status_Pair(mark,firstPosRoot));
	statusReverse->insert(statusReverse_Pair(firstPosRoot,mark));
	mark++;
	while (hasmarked < mark)
	{
		letterMap = new map<char,set<int> >();
		//当前标记的状态
		set<int> hasmarkedSet = status->find(hasmarked)->second;
		set<int>::iterator hasmarkedSet_pIter;
		for(hasmarkedSet_pIter = hasmarkedSet.begin();hasmarkedSet_pIter != hasmarkedSet.end();hasmarkedSet_pIter++)
		{
			//找到字母标记对应的字母节点
			Node* node = expressionTree->GetLeaf()->find(*hasmarkedSet_pIter)->second;
			//如果该节点不是终结符
			if (node->id != expressionTree->GetLetterCount())
			{
				//如果字母在字母位置表中已存在，直接添加当前位置
				if (letterMap->find(node->value) != letterMap->end())
				{
					letterMap->find(node->value)->second.insert(node->id);
				}
				else
				{
					//插入该字母表
					letterMap->insert(pair<char,set<int> >(node->value,set<int>()));
					//添加位置
					letterMap->find(node->value)->second.insert(node->id);
				}
			}
		}
		map<char,set<int> >::iterator letterMap_pIter;
		for(letterMap_pIter = letterMap->begin();letterMap_pIter != letterMap->end(); letterMap_pIter++)
		{
			//按每个字母，遍历它们的所有位置的followPos，然后合并成一个集合
			char ch = letterMap_pIter->first;
			set<int> now = letterMap_pIter->second;
			set<int> allFollowTmp;
			set<int>::iterator now_pIter;
			for(now_pIter = now.begin();now_pIter!=now.end();now_pIter++)
			{
				set<int> followTmp = followPos[*now_pIter];
				allFollowTmp.insert(followTmp.begin(),followTmp.end());
			}
			//如果这个集合是一个新的集合，插入到为检测的状态集中
			if (statusReverse->find(allFollowTmp) == statusReverse->end())
			{
				status->insert(status_Pair(mark,allFollowTmp));
				statusReverse->insert(statusReverse_Pair(allFollowTmp,mark));
				++mark;
			}
			//向dfa中插入这个转移
			dfa->insert(pair<pair<int,char>,int>(pair<int,char>(hasmarked,ch),statusReverse->find(allFollowTmp)->second));
		}
		//当前状态检测结束
		hasmarked++;
		delete(letterMap);
	}
	//确定终止状态
	map<int,set<int> >::iterator status_pIter;
	for(status_pIter = status->begin();status_pIter!=status->end();status_pIter++)
	{
		//如果发现状态包含的字母集中有终止字母#，该状态是终止状态
		if (status_pIter->second.find(expressionTree->GetLetterCount()) != status_pIter->second.end())
		{
			endState->insert(status_pIter->first);
		}
	}
}

void DFA::MinimizeDFA()
{
	map<int,set<char> > stateHasTransferChar;
	map<pair<int,char>,int>::iterator dfa_pIter;
	for (dfa_pIter = dfa->begin();dfa_pIter !=dfa->end();dfa_pIter++)	
	{
		if (stateHasTransferChar.find(dfa_pIter->first.first) != stateHasTransferChar.end())
		{
			stateHasTransferChar.find(dfa_pIter->first.first)->second.insert(dfa_pIter->first.second);
		}
		else
		{
			stateHasTransferChar.insert(pair<int,set<char> >(dfa_pIter->first.first,set<char>()));
			stateHasTransferChar.find(dfa_pIter->first.first)->second.insert(dfa_pIter->first.second);
		}
	}
	int statusCount = status->size();
	int* partition = new int[statusCount];
	set<pair<set<int>,int> > partitionSet;
	set<pair<set<int>,int> > partitionNewSet;
	set<int> start;
	set<int> close = *endState;
	for (int i=0;i<statusCount;i++)
	{
		if (close.find(i) == close.end())
		{
			start.insert(i);
			partition[i] = 0;
		}
		else
		{
			partition[i] = 1;
		}
	}
	int partitionNum = 1;
	partitionSet.insert(pair<set<int>,int>(start,0));
	partitionSet.insert(pair<set<int>,int>(close,1));
	do
	{
		//遍历分割集合
		set<pair<set<int>,int> >::iterator partitionSet_pIter;
		partitionNewSet = partitionSet;
		bool hasErased = false;
		for(partitionSet_pIter = partitionSet.begin();partitionSet_pIter != partitionSet.end();partitionSet_pIter++)
		{
			set<int> newOnePartitionSet;//新的分割集合
			//遍历分割集合中的一个集合
			set<int> onePartitionSet = partitionSet_pIter->first;
			set<int>::iterator onePartitionSet_pIter;
			pair<set<int>,int> oldPartition(*partitionSet_pIter);//存储有两个元素以上的onePartitionSet
			for (onePartitionSet_pIter = ++onePartitionSet.begin(); onePartitionSet_pIter != onePartitionSet.end(); onePartitionSet_pIter++)
			{
				if (stateHasTransferChar.find(*onePartitionSet_pIter) == stateHasTransferChar.end() || stateHasTransferChar.find(*onePartitionSet.begin()) == stateHasTransferChar.end() || stateHasTransferChar.find(*onePartitionSet_pIter)->second != stateHasTransferChar.find(*onePartitionSet.begin())->second)
				{
					if (!hasErased)
					{
						partitionSet.erase(partitionSet_pIter);//将该onePartitionSet从PartitionSet中移除
						hasErased = true;
					}
					partition[*onePartitionSet_pIter] = partitionNum+1;
					newOnePartitionSet.insert(*onePartitionSet_pIter);
					oldPartition.first.erase(*onePartitionSet_pIter);
				}
				else
				{
					set<char> charSet = stateHasTransferChar.find(*onePartitionSet_pIter)->second;
					set<char>::iterator charSet_pIter;
					for (charSet_pIter = charSet.begin(); charSet_pIter != charSet.end(); charSet_pIter++)
					{
						if (partition[dfa->find(pair<int,char>(*onePartitionSet.begin(),*charSet_pIter))->second] != partition[dfa->find(pair<int,char>(*onePartitionSet_pIter,*charSet_pIter))->second])
						{
							if (!hasErased)
							{
								partitionSet.erase(partitionSet_pIter);//将该onePartitionSet从PartitionSet中移除
								hasErased = true;
							}
							partition[*onePartitionSet_pIter] = partitionNum+1;
							newOnePartitionSet.insert(*onePartitionSet_pIter);
							oldPartition.first.erase(*onePartitionSet_pIter);
							break;
						}
					}
				}
				/*
				//遍历DFA
				map<pair<int,char>,int>::iterator dfa_pIter;
				for (dfa_pIter = dfa->begin();dfa_pIter !=dfa->end();dfa_pIter++)	
				{
					if (dfa_pIter->first.first == *onePartitionSet.begin())
					{
						if (dfa->find(pair<int,char>(*onePartitionSet_pIter,dfa_pIter->first.second)) == dfa->end() || partition[dfa->find(pair<int,char>(dfa_pIter->first.first,dfa_pIter->first.second))->second] != partition[dfa->find(pair<int,char>(*onePartitionSet_pIter,dfa_pIter->first.second))->second])
						{
							if (!hasErased)
							{
								partitionSet.erase(partitionSet_pIter);//将该onePartitionSet从PartitionSet中移除
								hasErased = true;
							}
							partition[*onePartitionSet_pIter] = partitionNum+1;
							newOnePartitionSet.insert(*onePartitionSet_pIter);
							oldPartition.first.erase(*onePartitionSet_pIter);
							break;
						}
					}
					//如果dfa起状态与当前遍历的元素相同
					if (dfa_pIter->first.first == *onePartitionSet_pIter)
					{
						if (dfa->find(pair<int,char>(*onePartitionSet.begin(),dfa_pIter->first.second)) == dfa->end() || partition[dfa->find(pair<int,char>(dfa_pIter->first.first,dfa_pIter->first.second))->second] != partition[dfa->find(pair<int,char>(*onePartitionSet.begin(),dfa_pIter->first.second))->second])
						{
							if (!hasErased)
							{
								partitionSet.erase(partitionSet_pIter);//将该onePartitionSet从PartitionSet中移除
								hasErased = true;
							}
							partition[*onePartitionSet_pIter] = partitionNum+1;
							newOnePartitionSet.insert(*onePartitionSet_pIter);
							oldPartition.first.erase(*onePartitionSet_pIter);
							break;
						}
					}
				}
				*/
			}
			if (newOnePartitionSet.size() !=0)
			{
				partitionSet.insert(oldPartition);
				pair<set<int>,int> newPartition(newOnePartitionSet,partitionNum+1);
				partitionSet.insert(newPartition);
				partitionNum++;
				break;
			}
		}
	}
	while (partitionSet != partitionNewSet);
	//遍历DFA,更换节点标记
	map<pair<int,char>,int> *newdfa = new map<pair<int,char>,int>();
	for (dfa_pIter = dfa->begin();dfa_pIter !=dfa->end();dfa_pIter++)	
	{
		pair<pair<int,char>,int> newTranslate(pair<int,char>(dfa_pIter->first.first,dfa_pIter->first.second),dfa_pIter->second);
		newTranslate.first.first = partition[newTranslate.first.first];
		newTranslate.second = partition[newTranslate.second];
		newdfa->insert(newTranslate);
	}
	delete(dfa);
	dfa = newdfa;
	beginState = partition[beginState];
	set<int>::iterator endState_pIter;
	set<int>* newEndState = new set<int>();
	for (endState_pIter = endState->begin(); endState_pIter != endState->end(); endState_pIter++)
	{
		newEndState->insert(partition[*endState_pIter]);
	}
	delete(endState);
	endState = newEndState;
	delete[](partition);
}

bool DFA::CheckWord(string word)
{
	//起始的状态为开始
	int stateNow = beginState;
	string::iterator word_pIter;
	//遍历单词的每个符号
	for (word_pIter = word.begin();word_pIter != word.end();word_pIter++)
	{
		//如果没有发现当前状态使用字符能转移，单词不属于自动机
		if (dfa->find(pair<int,char>(stateNow,*word_pIter)) == dfa->end())
		{
			return false;
		}
		else
		{
			//转移状态
			stateNow = dfa->find(pair<int,char>(stateNow,*word_pIter))->second;
		}
	}
	//如果最后状态没有停在结束状态，单词不属于自动机，否则单词属于自动机
	if (endState->find(stateNow) == endState->end())
	{
		return false;
	}
	else
	{
		return true;
	}
}
