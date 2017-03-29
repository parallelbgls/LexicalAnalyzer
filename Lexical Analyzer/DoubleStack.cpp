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
	//初始化返回队列
	queue<Triple<Node*,Node*,Node*> >* popNodeList = new queue<Triple<Node*,Node*,Node*> >();
	//如果前面隐藏了一个&
	if (preLetter)
	{
		//压入&
		delete(popNodeList);
		Node* node = new Node('&');
		popNodeList = InsertSymbol(node);
	}
	//压入字母
	letterStack->push(letter);
	preLetter = true;
	return popNodeList;
}

queue<Triple<Node*,Node*,Node*> >* DoubleStack::InsertSymbol(Node* symbol)
{
	//初始化返回队列
	queue<Triple<Node*,Node*,Node*> >* popNodeList = new queue<Triple<Node*,Node*,Node*> >();
	//左括号需要判断与增加隐藏&
	//（，|，&后面不可能隐藏&，），*后面可能隐藏&
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
	//压入右括号前弹出所有第一个（前的所有符号和相应的字母，加入返回队列
	if (symbol->value == ')')
	{
		while (symbolStack->top()->value != '(')
		{
			popNodeList->push(Pop());
		}
		//弹出（
		symbolStack->pop();
	}
	
	else
	{
		//如果栈不空，或者遇到左括号（左括号很特殊，压入是优先级最高，弹出时优先级最低），或栈顶符号的优先级比当前压入符号的优先级高
		while (!symbolStack->empty() && symbolStack->top()->value != '(' && symbolPriorityLevel->find(symbolStack->top()->value)->second >= symbolPriorityLevel->find(symbol->value)->second )
		{
			//弹出该符号和对应字母，加入返回队列
			popNodeList->push(Pop());
		}
		//压入该符号
		symbolStack->push(symbol);
	}
	return popNodeList;
}

queue<Triple<Node*,Node*,Node*> >* DoubleStack::PopAll()
{
	//初始化所有队列
	queue<Triple<Node*,Node*,Node*> >* popNodeList = new queue<Triple<Node*,Node*,Node*> >();
	//当符号栈不为空，弹出相应的符号与字母，加入返回队列
	while (!symbolStack->empty())
	{
		popNodeList->push(Pop());
	}
	return popNodeList;
}

Triple<Node*,Node*,Node*> DoubleStack::Pop()
{
	//如果符号是*
	if (symbolStack->top()->value == '*')
	{
		//弹出*和一个字母
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
		//构造
		Triple<Node*,Node*,Node*> node(child, NULL, father);
		letterStack->push(father);
		return node;
	}
	else
	{
		//弹出&或|和两个字母
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
		//构造
		Triple<Node*,Node*,Node*> node(leftChild,rightChild,father);
		letterStack->push(father);
		return node;
	}
}
