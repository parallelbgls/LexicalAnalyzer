#include"Tree.h"
#include"DoubleStack.h"
#include<map>

class DFA
{
protected:	
	typedef pair<int,set<int> > status_Pair;	
	typedef pair<set<int>,int> statusReverse_Pair;
	//自动机名称
	string name;
	//自动机表达式
	string expression;
	//自动机优先级，数字越小优先级越高
	int previousNum;
	//自动机状态转移，节点经字母转移到下一节点
	map<pair<int,char>,int>* dfa;
	//状态节点号与状态节点对应的字母节点集合
	map<int,set<int> >* status;
	//上一属性的反向集合
	map<set<int>,int>* statusReverse;
	//状态机构造出的表达式树
	Tree* expressionTree;
	//构造表达式树用的双栈
	DoubleStack* doubleStack;
	//节点的followPos
	set<int>* followPos;
	//开始状态
	int beginState;
	//结束状态
	set<int>* endState;
	//连接returnQueue中三个节点的父子关系
	void BuildTree(queue<Triple<Node*,Node*,Node*> >* returnQueue);
	//确定树中的firstPos，lastPos，nullable
	void FixTree(Tree* tree);
	//求树的所有的节点的followPos
	void SolveFollowPos(Tree* tree);
	//求节点Node包含的followPos
	void SolveFollowPos(Node* node);
	//构造DFA
	void ConstructDFA();
	//最小化DFA
	void MinimizeDFA();

public:
	DFA(string name, string expression, int previousNum);
	~DFA();
	//检查单词是否满足该DFA
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
