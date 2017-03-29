#include"Expression.h"
#include<fstream>
#include<list>
#include<string>

using namespace std;
const int LINE_READ = 300;

class Analyser
{
protected:
	//������ʽ
	Expression* expression;
	ifstream fin;
	ofstream fout;
	//�в�ֺ�ĵ�������
	list<Triple<int,int,string> >* wordList;
	list<Triple<int,int,string> > unknownWord;
	//��ǰ��������
	int rowCount;
	//���ļ�
	bool ReadFile();
	//�����ļ�
	void Analyze();
	//д�ļ�
	void WriteFile(int row, int col, string word, string dfaName);
	//��ֵ��ʣ�������ַ��ű��͵�������ǰ��ָ��ָ��
	void DivideWord(list<string>* letterTable,list<Triple<int,int,string> >::iterator &wordList_pIter);
	void WriteUnknownWord();
public:
	//Դ�ļ���ַ��������ʽ�ļ���ַ��Ŀ���ļ���ַ
	Analyser(string sourceFilePath, string regularExpressionPath, string destinationFile);
	~Analyser();
};
