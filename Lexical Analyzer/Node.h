#include<string>
#include<set>

using namespace std;

struct Node
{
	//�ڵ���
	int id;

	//�ڵ�ֵ
	char value;

	//��followPos����������
	bool nullable;
	set<int>* firstpos;
	set<int>* lastpos;

	//���׽ڵ�
	Node* father;
	//����ӽڵ�
	Node* leftchild;
	//�Ҷ��ӽڵ�
	Node* rightchild;

	//���캯��
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

	//��������
	~Node()
	{
		delete(firstpos);
		delete(lastpos);
	}
};