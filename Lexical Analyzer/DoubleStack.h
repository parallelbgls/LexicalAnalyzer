#include<stack>
#include<queue>
#include<map>
#ifndef NodeTitle
#define NodeTitle
#include"Node.h"
#endif
using namespace std;

template <typename T1, typename T2, typename T3>
//三元组
struct Triple
{
	T1 first;
	T2 second;
	T3 third;

	Triple(T1 first, T2 second, T3 third)
	{
		this->first = first;
		this->second = second;
		this->third = third;
	}
};

class DoubleStack
{
protected:
	//字母栈
	stack<Node*>* letterStack;
	//符号栈
	stack<Node*>* symbolStack;
	//符号优先级，越大优先级越高
	map<char,int>* symbolPriorityLevel;
	typedef pair<char, int> mapPair;
	//前面的字符集合是否可以当做一个字母，发现隐藏的&节点使用
	bool preLetter;
	//手动弹出一组节点，分别是左儿子，右儿子和父亲
	Triple<Node*,Node*,Node*> Pop();

public:
	DoubleStack();
	~DoubleStack();
	//压入一个字母
	queue<Triple<Node*,Node*,Node*> >* InsertLetter(Node* letter);
	//压入一个符号
	queue<Triple<Node*,Node*,Node*> >* InsertSymbol(Node* symbol);
	//弹出所有栈中元素（最后字母栈里会有一个&）
	queue<Triple<Node*,Node*,Node*> >* PopAll();
};
