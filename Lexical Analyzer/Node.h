#include<string>
#include<set>

using namespace std;

struct Node
{
	//节点标记
	int id;

	//节点值
	char value;

	//求followPos的三个属性
	bool nullable;
	set<int>* firstpos;
	set<int>* lastpos;

	//父亲节点
	Node* father;
	//左儿子节点
	Node* leftchild;
	//右儿子节点
	Node* rightchild;

	//构造函数
	Node(char value)
	{
		this->value = value;
		firstpos = new set<int>();
		lastpos = new set<int>();
		father = NULL;
		leftchild = NULL;
		rightchild = NULL;
		id = -1;
		nullable = true;
	}

	//析构函数
	~Node()
	{
		delete(firstpos);
		delete(lastpos);
	}
};