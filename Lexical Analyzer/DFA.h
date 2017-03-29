#include"Tree.h"
#include"DoubleStack.h"
#include<map>

class DFA
{
protected:	
	typedef pair<int,set<int> > status_Pair;	
	typedef pair<set<int>,int> statusReverse_Pair;
	//�Զ�������
	string name;
	//�Զ������ʽ
	string expression;
	//�Զ������ȼ�������ԽС���ȼ�Խ��
	int previousNum;
	//�Զ���״̬ת�ƣ��ڵ㾭��ĸת�Ƶ���һ�ڵ�
	map<pair<int,char>,int>* dfa;
	//״̬�ڵ����״̬�ڵ��Ӧ����ĸ�ڵ㼯��
	map<int,set<int> >* status;
	//��һ���Եķ��򼯺�
	map<set<int>,int>* statusReverse;
	//״̬��������ı��ʽ��
	Tree* expressionTree;
	//������ʽ���õ�˫ջ
	DoubleStack* doubleStack;
	//�ڵ��followPos
	set<int>* followPos;
	//��ʼ״̬
	int beginState;
	//����״̬
	set<int>* endState;
	//����returnQueue�������ڵ�ĸ��ӹ�ϵ
	void BuildTree(queue<Triple<Node*,Node*,Node*> >* returnQueue);
	//ȷ�����е�firstPos��lastPos��nullable
	void FixTree(Tree* tree);
	//���������еĽڵ��followPos
	void SolveFollowPos(Tree* tree);
	//��ڵ�Node������followPos
	void SolveFollowPos(Node* node);
	//����DFA
	void ConstructDFA();
	//��С��DFA
	void MinimizeDFA();

public:
	DFA(string name, string expression, int previousNum);
	~DFA();
	//��鵥���Ƿ������DFA
	bool CheckWord(string word);
	string GetName()
	{
		return name;
	}
	string GetExpression()
	{
		return expression;
	}
	//map<pair<int,char>,int>* GetDFA();

	map<int,Node*>* GetLeaf()
	{
		return (expressionTree->GetLeaf());
	}
	int GetPreviousNum()
	{
		return previousNum;
	}
	void SetPriousNum(int previousNum)
	{
		this->previousNum = previousNum;
	}
};
