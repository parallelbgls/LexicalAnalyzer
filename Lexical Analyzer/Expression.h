#include<fstream>
#include<string>
#include"DFA.h"
#include<map>
#include<list>
#include<iostream>
using namespace std;

class Expression
{
protected:
	//����Ҫ�����Զ���������ĸ��
	map<string,DFA*>* letter;
	//������ʽ�Զ���
	map<string,DFA*>* regularExpression;
	//��ռ���ż�
	list<string>* grabLetter;
	//����ռ���ż�
	list<string>* unGrabLetter;
	//������ʽ����λ��
	int insertInto;
	//�Զ�������
	int dfaCount;
public:
	Expression();
	Expression(string title);
	~Expression();
	void Insert(string expression, int previousNum);
	map<string,DFA*>* GetLetter()
	{
		return letter;
	}
	map<string,DFA*>* GetRegularExpression()
	{
		return regularExpression;
	}
	list<string>* GetGrabLetter()
	{
		return grabLetter;
	}
	list<string>* GetUnGrabLetter()
	{
		return unGrabLetter;
	}
	string CheckWord(string word);
};