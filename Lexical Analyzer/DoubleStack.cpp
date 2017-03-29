#include"DoubleStack.h"

DoubleStack::DoubleStack()
{
	letterStack = new stack<Node*>();
	symbolStack = new stack<Node*>();
	symbolPriorityLevel = new map<char,int>();
	symbolPriorityLevel->insert(mapPair('(',1000));
	symbolPriorityLevel->insert(mapPair('*',3));
	symbolPriorityLevel->insert(mapPair('&',2));
	symbolPriorityLevel->insert(mapPair('|',1));
	preLetter = false;
}

DoubleStack::~DoubleStack()
{
	delete(letterStack);
	delete(symbolStack);
	delete(symbolPriorityLevel);
}

queue<Triple<Node*,Node*,Node*> >* DoubleStack::InsertLetter(Node* letter)
{
	//��ʼ�����ض���
	queue<Triple<Node*,Node*,Node*> >* popNodeList = new queue<Triple<Node*,Node*,Node*> >();
	//���ǰ��������һ��&
	if (preLetter)
	{
		//ѹ��&
		delete(popNodeList);
		Node* node = new Node('&');
		popNodeList = InsertSymbol(node);
	}
	//ѹ����ĸ
	letterStack->push(letter);
	preLetter = true;
	return popNodeList;
}

queue<Triple<Node*,Node*,Node*> >* DoubleStack::InsertSymbol(Node* symbol)
{
	//��ʼ�����ض���
	queue<Triple<Node*,Node*,Node*> >* popNodeList = new queue<Triple<Node*,Node*,Node*> >();
	//��������Ҫ�ж�����������&
	//����|��&���治��������&������*�����������&
	if (symbol->value == '(')
	{
		if (preLetter)
		{
			delete(popNodeList);
			Node* node = new Node('&');
			popNodeList = InsertSymbol(node);
		}
		preLetter = false;
	}
	else if (symbol->value == '|' || symbol->value == '&')	
	{
		preLetter = false;
	}
	else
	{
		preLetter = true;
	}
	//ѹ��������ǰ�������е�һ����ǰ�����з��ź���Ӧ����ĸ�����뷵�ض���
	if (symbol->value == ')')
	{
		while (symbolStack->top()->value != '(')
		{
			popNodeList->push(Pop());
		}
		//������
		symbolStack->pop();
	}
	
	else
	{
		//���ջ���գ��������������ţ������ź����⣬ѹ�������ȼ���ߣ�����ʱ���ȼ���ͣ�����ջ�����ŵ����ȼ��ȵ�ǰѹ����ŵ����ȼ���
		while (!symbolStack->empty() && symbolStack->top()->value != '(' && symbolPriorityLevel->find(symbolStack->top()->value)->second >= symbolPriorityLevel->find(symbol->value)->second )
		{
			//�����÷��źͶ�Ӧ��ĸ�����뷵�ض���
			popNodeList->push(Pop());
		}
		//ѹ��÷���
		symbolStack->push(symbol);
	}
	return popNodeList;
}

queue<Triple<Node*,Node*,Node*> >* DoubleStack::PopAll()
{
	//��ʼ�����ж���
	queue<Triple<Node*,Node*,Node*> >* popNodeList = new queue<Triple<Node*,Node*,Node*> >();
	//������ջ��Ϊ�գ�������Ӧ�ķ�������ĸ�����뷵�ض���
	while (!symbolStack->empty())
	{
		popNodeList->push(Pop());
	}
	return popNodeList;
}

Triple<Node*,Node*,Node*> DoubleStack::Pop()
{
	//���������*
	if (symbolStack->top()->value == '*')
	{
		//����*��һ����ĸ
		Node* father = symbolStack->top();
		Node* child = NULL;
		symbolStack->pop();
		if (letterStack->empty())
		{
			throw this;
		}
		else
		{
			child = letterStack->top();
			letterStack->pop();
		}
		//����
		Triple<Node*,Node*,Node*> node(child, NULL, father);
		letterStack->push(father);
		return node;
	}
	else
	{
		//����&��|��������ĸ
		Node* father = symbolStack->top();
		symbolStack->pop();
		Node* rightChild = NULL;
		if (letterStack->empty())
		{
			throw this;
		}
		else
		{
			rightChild = letterStack->top();
			letterStack->pop();
		}
		Node* leftChild = NULL;
		if (letterStack->empty())
		{
			throw this;
		}
		else
		{
			leftChild = letterStack->top();
			letterStack->pop();
		}
		//����
		Triple<Node*,Node*,Node*> node(leftChild,rightChild,father);
		letterStack->push(father);
		return node;
	}
}
