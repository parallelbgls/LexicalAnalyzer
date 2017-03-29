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
	//������������Ԫ�ص�followpos�����ߵ�#����Ȼ�ᷢ�����ա�
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
				//ǿ����ĸѹ��˫ջ
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
					//����ѹ�����ջ
					if (ch == '(' || ch == ')' || ch == '|' || ch == '&' || ch == '*')
					{
						queue<Triple<Node*,Node*,Node*> >* returnQueue = doubleStack->InsertSymbol(node);
						BuildTree(returnQueue);
					}
					//��ĸѹ����ĸջ
					else
					{
						queue<Triple<Node*,Node*,Node*> >* returnQueue = doubleStack->InsertLetter(node);
						BuildTree(returnQueue);
					}
				}
			}
		}
		//�������л���ջ�е�Ԫ��
		queue<Triple<Node*,Node*,Node*> >* returnQueue = doubleStack->PopAll();
		//�������еĸ��ڵ�
		expressionTree->SetRoot(returnQueue->back().third);
		//����
		BuildTree(returnQueue);
	}
	catch(DoubleStack *doubleStack)
	{
		throw this;
	}
	//������е�firstPos��lastPos��nullable
	FixTree(expressionTree);
	//��followPos
	followPos = new set<int>[expressionTree->GetLetterCount()+1];
	SolveFollowPos(expressionTree);
	//����DFA
	ConstructDFA();
	//��С��DFA
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
		//ȡ���ض��е���Ԫ��
		Triple<Node*,Node*,Node*> triple = returnQueue->front();
		//��������
		expressionTree->AddChild(triple.first,triple.third,leftChild);
		if (triple.second != NULL)
		{
			//��������
			expressionTree->AddChild(triple.second,triple.third,rightChild);
		}
		//������Ԫ��
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
			//�ڵ���&����������lastPos��ÿ���ڵ��followPos����������firstPos
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
			//�ڵ���|��ʲô������
			else if (node->value == '|')
			{
			}
			else throw node;
		}
		else
		{
			//�ڵ���*,���ӽڵ��ÿ��lastPos�ڵ��followPos���ж��ӽڵ��firstPos
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
	//��ĸֵ��Ӧ��λ��
	map<char,set<int> >* letterMap;
	int mark = 0;
	int hasmarked = 0;
	beginState = mark;
	//��ʼ�ɴ���ĸ
	set<int> firstPosRoot = *(expressionTree->GetRoot()->firstpos);
	//���
	status->insert(status_Pair(mark,firstPosRoot));
	statusReverse->insert(statusReverse_Pair(firstPosRoot,mark));
	mark++;
	while (hasmarked < mark)
	{
		letterMap = new map<char,set<int> >();
		//��ǰ��ǵ�״̬
		set<int> hasmarkedSet = status->find(hasmarked)->second;
		set<int>::iterator hasmarkedSet_pIter;
		for(hasmarkedSet_pIter = hasmarkedSet.begin();hasmarkedSet_pIter != hasmarkedSet.end();hasmarkedSet_pIter++)
		{
			//�ҵ���ĸ��Ƕ�Ӧ����ĸ�ڵ�
			Node* node = expressionTree->GetLeaf()->find(*hasmarkedSet_pIter)->second;
			//����ýڵ㲻���ս��
			if (node->id != expressionTree->GetLetterCount())
			{
				//�����ĸ����ĸλ�ñ����Ѵ��ڣ�ֱ����ӵ�ǰλ��
				if (letterMap->find(node->value) != letterMap->end())
				{
					letterMap->find(node->value)->second.insert(node->id);
				}
				else
				{
					//�������ĸ��
					letterMap->insert(pair<char,set<int> >(node->value,set<int>()));
					//���λ��
					letterMap->find(node->value)->second.insert(node->id);
				}
			}
		}
		map<char,set<int> >::iterator letterMap_pIter;
		for(letterMap_pIter = letterMap->begin();letterMap_pIter != letterMap->end(); letterMap_pIter++)
		{
			//��ÿ����ĸ���������ǵ�����λ�õ�followPos��Ȼ��ϲ���һ������
			char ch = letterMap_pIter->first;
			set<int> now = letterMap_pIter->second;
			set<int> allFollowTmp;
			set<int>::iterator now_pIter;
			for(now_pIter = now.begin();now_pIter!=now.end();now_pIter++)
			{
				set<int> followTmp = followPos[*now_pIter];
				allFollowTmp.insert(followTmp.begin(),followTmp.end());
			}
			//������������һ���µļ��ϣ����뵽Ϊ����״̬����
			if (statusReverse->find(allFollowTmp) == statusReverse->end())
			{
				status->insert(status_Pair(mark,allFollowTmp));
				statusReverse->insert(statusReverse_Pair(allFollowTmp,mark));
				++mark;
			}
			//��dfa�в������ת��
			dfa->insert(pair<pair<int,char>,int>(pair<int,char>(hasmarked,ch),statusReverse->find(allFollowTmp)->second));
		}
		//��ǰ״̬������
		hasmarked++;
		delete(letterMap);
	}
	//ȷ����ֹ״̬
	map<int,set<int> >::iterator status_pIter;
	for(status_pIter = status->begin();status_pIter!=status->end();status_pIter++)
	{
		//�������״̬��������ĸ��������ֹ��ĸ#����״̬����ֹ״̬
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
		//�����ָ��
		set<pair<set<int>,int> >::iterator partitionSet_pIter;
		partitionNewSet = partitionSet;
		bool hasErased = false;
		for(partitionSet_pIter = partitionSet.begin();partitionSet_pIter != partitionSet.end();partitionSet_pIter++)
		{
			set<int> newOnePartitionSet;//�µķָ��
			//�����ָ���е�һ������
			set<int> onePartitionSet = partitionSet_pIter->first;
			set<int>::iterator onePartitionSet_pIter;
			pair<set<int>,int> oldPartition(*partitionSet_pIter);//�洢������Ԫ�����ϵ�onePartitionSet
			for (onePartitionSet_pIter = ++onePartitionSet.begin(); onePartitionSet_pIter != onePartitionSet.end(); onePartitionSet_pIter++)
			{
				if (stateHasTransferChar.find(*onePartitionSet_pIter) == stateHasTransferChar.end() || stateHasTransferChar.find(*onePartitionSet.begin()) == stateHasTransferChar.end() || stateHasTransferChar.find(*onePartitionSet_pIter)->second != stateHasTransferChar.find(*onePartitionSet.begin())->second)
				{
					if (!hasErased)
					{
						partitionSet.erase(partitionSet_pIter);//����onePartitionSet��PartitionSet���Ƴ�
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
								partitionSet.erase(partitionSet_pIter);//����onePartitionSet��PartitionSet���Ƴ�
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
				//����DFA
				map<pair<int,char>,int>::iterator dfa_pIter;
				for (dfa_pIter = dfa->begin();dfa_pIter !=dfa->end();dfa_pIter++)	
				{
					if (dfa_pIter->first.first == *onePartitionSet.begin())
					{
						if (dfa->find(pair<int,char>(*onePartitionSet_pIter,dfa_pIter->first.second)) == dfa->end() || partition[dfa->find(pair<int,char>(dfa_pIter->first.first,dfa_pIter->first.second))->second] != partition[dfa->find(pair<int,char>(*onePartitionSet_pIter,dfa_pIter->first.second))->second])
						{
							if (!hasErased)
							{
								partitionSet.erase(partitionSet_pIter);//����onePartitionSet��PartitionSet���Ƴ�
								hasErased = true;
							}
							partition[*onePartitionSet_pIter] = partitionNum+1;
							newOnePartitionSet.insert(*onePartitionSet_pIter);
							oldPartition.first.erase(*onePartitionSet_pIter);
							break;
						}
					}
					//���dfa��״̬�뵱ǰ������Ԫ����ͬ
					if (dfa_pIter->first.first == *onePartitionSet_pIter)
					{
						if (dfa->find(pair<int,char>(*onePartitionSet.begin(),dfa_pIter->first.second)) == dfa->end() || partition[dfa->find(pair<int,char>(dfa_pIter->first.first,dfa_pIter->first.second))->second] != partition[dfa->find(pair<int,char>(*onePartitionSet.begin(),dfa_pIter->first.second))->second])
						{
							if (!hasErased)
							{
								partitionSet.erase(partitionSet_pIter);//����onePartitionSet��PartitionSet���Ƴ�
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
	//����DFA,�����ڵ���
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
	//��ʼ��״̬Ϊ��ʼ
	int stateNow = beginState;
	string::iterator word_pIter;
	//�������ʵ�ÿ������
	for (word_pIter = word.begin();word_pIter != word.end();word_pIter++)
	{
		//���û�з��ֵ�ǰ״̬ʹ���ַ���ת�ƣ����ʲ������Զ���
		if (dfa->find(pair<int,char>(stateNow,*word_pIter)) == dfa->end())
		{
			return false;
		}
		else
		{
			//ת��״̬
			stateNow = dfa->find(pair<int,char>(stateNow,*word_pIter))->second;
		}
	}
	//������״̬û��ͣ�ڽ���״̬�����ʲ������Զ��������򵥴������Զ���
	if (endState->find(stateNow) == endState->end())
	{
		return false;
	}
	else
	{
		return true;
	}
}
