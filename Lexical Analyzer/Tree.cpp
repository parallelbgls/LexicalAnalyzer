#include "Tree.h"


Tree::Tree()
{
	leaf = new map<int,Node*>();
	count = 0;
}

Tree::~Tree()
{
	delete(leaf);
	Release(root);
}

void Tree::Release(Node *node)	
{
	if (node != NULL)
	{
		Release(node->leftchild);
		Release(node->rightchild);
		delete(node);
	}
}

void Tree::AddChild(Node* child, Node* father, Addpos addpos)
{
	if (addpos == leftChild)
	{
		child->father = father;
		father->leftchild = child;
	}
	else if (addpos == rightChild)
	{
		child->father = father;
		father->rightchild = child;
	}
	else throw addpos;
}

void Tree::Fix()
{
	if (root == NULL) throw root;
	while (root->father != NULL) root = root->father;
	Fix(root);
}

void Tree::Fix(Node* node)
{
	//左儿子不为空，递归左儿子
	if (node->leftchild != NULL)
	{
		Fix(node->leftchild);
		//右儿子不为空，递归右儿子
		if (node->rightchild != NULL)
		{
			Fix(node->rightchild);
			//左右儿子求出后
			//如果节点是并，firstPos是左右儿子的firstPos的并集，lastPos是左右儿子lastPos的并集，nullable是左右儿子nullable的并
			if (node->value == '|')
			{
				node->nullable = node->leftchild->nullable || node->rightchild->nullable;
				node->firstpos->insert(node->leftchild->firstpos->begin(),node->leftchild->firstpos->end());
				node->firstpos->insert(node->rightchild->firstpos->begin(),node->rightchild->firstpos->end());
				node->lastpos->insert(node->leftchild->lastpos->begin(),node->leftchild->lastpos->end());
				node->lastpos->insert(node->rightchild->lastpos->begin(),node->rightchild->lastpos->end());
			}
			//如果节点是连接，
			else if (node->value == '&')
			{
				//nullable是左右儿子的与
				node->nullable = node->leftchild->nullable && node->rightchild->nullable;
				//如果左儿子是可空的，firstPos是左右儿子firstPos的并集
				if (node->leftchild->nullable)
				{
					node->firstpos->insert(node->leftchild->firstpos->begin(),node->leftchild->firstpos->end());					
					node->firstpos->insert(node->rightchild->firstpos->begin(),node->rightchild->firstpos->end());			
				}
				//否则firstPos是左儿子的firstPos
				else
				{
					node->firstpos->insert(node->leftchild->firstpos->begin(),node->leftchild->firstpos->end());					
				}
				//如果右儿子是可空的，lastPos是左右儿子lastPos的并集
				if (node->rightchild->nullable)
				{
					node->lastpos->insert(node->leftchild->lastpos->begin(),node->leftchild->lastpos->end());
					node->lastpos->insert(node->rightchild->lastpos->begin(),node->rightchild->lastpos->end());
				}
				//否则lastPos是右儿子的lastPos
				else
				{
					node->lastpos->insert(node->rightchild->lastpos->begin(),node->rightchild->lastpos->end());
				}
			}
		}
		//左儿子不空，右儿子为空
		else
		{
			//如果节点值是*，nullable为真，firstPos是儿子的firstPos，lastPos是儿子的lastPos。
			if (node->value == '*')
			{
				node->nullable = true;
				node->firstpos->insert(node->leftchild->firstpos->begin(),node->leftchild->firstpos->end());
				node->lastpos->insert(node->leftchild->lastpos->begin(),node->leftchild->lastpos->end());
			}
			else throw node;
		}
	}
	//左右儿子都为空
	else
	{
		//如果节点不是空节点，或它是字母'$'
		if (node->value!='$' || (node->value == '$' && node->nullable == false))
		{
			//儿子个数+1
			count++;
			//id赋值
			node->id = count;
			//可空属性为false
			node->nullable = false;
			//firstPos和lastPos赋为自己的节点值
			node->firstpos->insert(node->id);
			node->lastpos->insert(node->id);
		}
		else
		{
			//可空属性为真，其余属性不需要赋值。
			node->nullable = true;
		}
		//增加节点到叶子节点集合中
		leaf->insert(pair<int,Node*>(node->id,node));
	}
}
