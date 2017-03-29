#include<stack>
#include<queue>
#include<map>
#ifndef NodeTitle
#define NodeTitle
#include"Node.h"
#endif
using namespace std;

template <typename T1, typename T2, typename T3>
//��Ԫ��
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
	//��ĸջ
	stack<Node*>* letterStack;
	//����ջ
	stack<Node*>* symbolStack;
	//�������ȼ���Խ�����ȼ�Խ��
	map<char,int>* symbolPriorityLevel;
	typedef pair<char, int> mapPair;
	//ǰ����ַ������Ƿ���Ե���һ����ĸ���������ص�&�ڵ�ʹ��
	bool preLetter;
	//�ֶ�����һ��ڵ㣬�ֱ�������ӣ��Ҷ��Ӻ͸���
	Triple<Node*,Node*,Node*> Pop();

public:
	DoubleStack();
	~DoubleStack();
	//ѹ��һ����ĸ
	queue<Triple<Node*,Node*,Node*> >* InsertLetter(Node* letter);
	//ѹ��һ������
	queue<Triple<Node*,Node*,Node*> >* InsertSymbol(Node* symbol);
	//��������ջ��Ԫ�أ������ĸջ�����һ��&��
	queue<Triple<Node*,Node*,Node*> >* PopAll();
};
