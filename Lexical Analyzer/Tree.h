#ifndef NodeTitle
#define NodeTitle
#include "Node.h"
#endif
#include <map>
#include <stack>
enum Addpos{leftChild, rightChild};

class Tree
{
protected:
	//根节点
	Node* root;
	//叶子节点
	map<int,Node*>* leaf;

	//不是空的叶子节点的个数
	int count;

public:

	//构造函数
	Tree();
	//析构函数
	~Tree();
	//设置根节点
	void SetRoot(Node* root)
	{
		this->root = root;
	}
	//获取根节点
	Node* GetRoot()
	{
		return root;
	}
	//在father节点的左儿子或右儿子添加child节点
	void AddChild(Node* child,Node* father,Addpos addpos);
	//补齐树中所有Node的值的nullable，firstPos，lastPos
	void Fix();
	//获取不是空的叶子节点的个数
	int GetLetterCount()
	{
		return count;
	}
	//获取叶子节点
	map<int,Node*>* GetLeaf()
	{
		return leaf;
	}

protected:
	//补齐Node节点的nullable，firstPos，lastPos
	void Fix(Node* node);
	//释放Node节点，析构使用
	void Release(Node* node);
};