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
	//���ڵ�
	Node* root;
	//Ҷ�ӽڵ�
	map<int,Node*>* leaf;

	//���ǿյ�Ҷ�ӽڵ�ĸ���
	int count;

public:

	//���캯��
	Tree();
	//��������
	~Tree();
	//���ø��ڵ�
	void SetRoot(Node* root)
	{
		this->root = root;
	}
	//��ȡ���ڵ�
	Node* GetRoot()
	{
		return root;
	}
	//��father�ڵ������ӻ��Ҷ������child�ڵ�
	void AddChild(Node* child,Node* father,Addpos addpos);
	//������������Node��ֵ��nullable��firstPos��lastPos
	void Fix();
	//��ȡ���ǿյ�Ҷ�ӽڵ�ĸ���
	int GetLetterCount()
	{
		return count;
	}
	//��ȡҶ�ӽڵ�
	map<int,Node*>* GetLeaf()
	{
		return leaf;
	}

protected:
	//����Node�ڵ��nullable��firstPos��lastPos
	void Fix(Node* node);
	//�ͷ�Node�ڵ㣬����ʹ��
	void Release(Node* node);
};