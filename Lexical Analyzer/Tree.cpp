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
	//����Ӳ�Ϊ�գ��ݹ������
	if (node->leftchild != NULL)
	{
		Fix(node->leftchild);
		//�Ҷ��Ӳ�Ϊ�գ��ݹ��Ҷ���
		if (node->rightchild != NULL)
		{
			Fix(node->rightchild);
			//���Ҷ��������
			//����ڵ��ǲ���firstPos�����Ҷ��ӵ�firstPos�Ĳ�����lastPos�����Ҷ���lastPos�Ĳ�����nullable�����Ҷ���nullable�Ĳ�
			if (node->value == '|')
			{
				node->nullable = node->leftchild->nullable || node->rightchild->nullable;
				node->firstpos->insert(node->leftchild->firstpos->begin(),node->leftchild->firstpos->end());
				node->firstpos->insert(node->rightchild->firstpos->begin(),node->rightchild->firstpos->end());
				node->lastpos->insert(node->leftchild->lastpos->begin(),node->leftchild->lastpos->end());
				node->lastpos->insert(node->rightchild->lastpos->begin(),node->rightchild->lastpos->end());
			}
			//����ڵ������ӣ�
			else if (node->value == '&')
			{
				//nullable�����Ҷ��ӵ���
				node->nullable = node->leftchild->nullable && node->rightchild->nullable;
				//���������ǿɿյģ�firstPos�����Ҷ���firstPos�Ĳ���
				if (node->leftchild->nullable)
				{
					node->firstpos->insert(node->leftchild->firstpos->begin(),node->leftchild->firstpos->end());					
					node->firstpos->insert(node->rightchild->firstpos->begin(),node->rightchild->firstpos->end());			
				}
				//����firstPos������ӵ�firstPos
				else
				{
					node->firstpos->insert(node->leftchild->firstpos->begin(),node->leftchild->firstpos->end());					
				}
				//����Ҷ����ǿɿյģ�lastPos�����Ҷ���lastPos�Ĳ���
				if (node->rightchild->nullable)
				{
					node->lastpos->insert(node->leftchild->lastpos->begin(),node->leftchild->lastpos->end());
					node->lastpos->insert(node->rightchild->lastpos->begin(),node->rightchild->lastpos->end());
				}
				//����lastPos���Ҷ��ӵ�lastPos
				else
				{
					node->lastpos->insert(node->rightchild->lastpos->begin(),node->rightchild->lastpos->end());
				}
			}
		}
		//����Ӳ��գ��Ҷ���Ϊ��
		else
		{
			//����ڵ�ֵ��*��nullableΪ�棬firstPos�Ƕ��ӵ�firstPos��lastPos�Ƕ��ӵ�lastPos��
			if (node->value == '*')
			{
				node->nullable = true;
				node->firstpos->insert(node->leftchild->firstpos->begin(),node->leftchild->firstpos->end());
				node->lastpos->insert(node->leftchild->lastpos->begin(),node->leftchild->lastpos->end());
			}
			else throw node;
		}
	}
	//���Ҷ��Ӷ�Ϊ��
	else
	{
		//����ڵ㲻�ǿսڵ㣬��������ĸ'$'
		if (node->value!='$' || (node->value == '$' && node->nullable == false))
		{
			//���Ӹ���+1
			count++;
			//id��ֵ
			node->id = count;
			//�ɿ�����Ϊfalse
			node->nullable = false;
			//firstPos��lastPos��Ϊ�Լ��Ľڵ�ֵ
			node->firstpos->insert(node->id);
			node->lastpos->insert(node->id);
		}
		else
		{
			//�ɿ�����Ϊ�棬�������Բ���Ҫ��ֵ��
			node->nullable = true;
		}
		//���ӽڵ㵽Ҷ�ӽڵ㼯����
		leaf->insert(pair<int,Node*>(node->id,node));
	}
}
