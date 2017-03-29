#include"Analyser.h"

Analyser::Analyser(string sourceFilePath, string regularExpressionPath, string destinationFile)
{
	expression = new Expression(regularExpressionPath);
	fin.open(sourceFilePath.c_str());
	fout.open(destinationFile.c_str());
	rowCount = 0;
	wordList = new list<Triple<int,int,string> >();
	while (ReadFile())
	{
		Analyze();
	}
	WriteUnknownWord();
	unknownWord.clear();
}

Analyser::~Analyser()
{
	delete(expression);
	delete(wordList);
	fin.close();
	fout.close();
}

bool Analyser::ReadFile()
{
	rowCount++;
	char str[LINE_READ];
	if (!fin.getline(str,LINE_READ))
	{
		return false;
	}
	else
	{
		int headSpace = 0;
		string lan = string(str);
		//�����Ϊ�գ�������ȡ
		if (lan == "") return true;
		//���ո��ֵ���
		while (lan[0] == ' ') {lan = lan.substr(1);headSpace++;}
		wordList->insert(wordList->begin(),Triple<int,int,string>(rowCount,headSpace+1,lan));
		while ((int)(wordList->rbegin()->third.find(' ')) >= 0)
		{
			int pos = wordList->rbegin()->third.find(' ');
			Triple<int,int,string> wordOld = *(wordList->rbegin());
			wordList->pop_back();
			string wordtmp = wordOld.third.substr(0,pos);
			if (wordtmp != "")
			{
				Triple<int,int,string> wordNew1(wordOld.first,wordOld.second,wordtmp); 
				wordList->push_back(wordNew1);
			}
			wordtmp = wordOld.third.substr(pos+1);
			if (wordtmp != "")
			{
				Triple<int,int,string> wordNew2(wordOld.first,wordOld.second+pos+1,wordtmp);
				wordList->push_back(wordNew2);
			}
		}
		
		//����ռ���ű��ֵ���
		list<Triple<int,int,string> >::iterator wordList_pIter;
		list<string>* grabLetter = expression->GetGrabLetter();
		for (wordList_pIter = wordList->begin(); wordList_pIter != wordList->end(); wordList_pIter++)
		{
			DivideWord(grabLetter,wordList_pIter);
		}
		return true;
	}
}

void Analyser::DivideWord(list<string>* letterTable,list<Triple<int,int,string> >::iterator &wordList_pIter)
{
	//��ַ��ű�ı���ָ��
	list<string>::iterator letterTable_pIter;
	for (letterTable_pIter = letterTable->begin(); letterTable_pIter != letterTable->end(); letterTable_pIter++)
	{
		//��ָ����ָ�ĵ������в�ַ��ű���ķ���
		while ((int)(wordList_pIter->third.find(*letterTable_pIter)) >=0)
		{
			//�����ʲ�Ϊ��������
			list<Triple<int,int,string> >::iterator wordList_pIter_tmp;
			bool isInsert = false;
			if (wordList_pIter->third == *letterTable_pIter)
			{
				break;
			}
			int pos = wordList_pIter->third.find(*letterTable_pIter);
			Triple<int,int,string> wordOld = *wordList_pIter;
			string wordtmp = wordOld.third.substr(0,pos);
			if (wordtmp != "")
			{
				isInsert = true;
				Triple<int,int,string> wordNew1(wordOld.first,wordOld.second,wordtmp);
				wordList->insert(wordList_pIter,wordNew1);
				wordList_pIter_tmp = --wordList_pIter;
				//����ָ��ǰ���ƶ������ƻ��ԣ������ƶ�ָ�뵽ԭʼ��λ��
				wordList_pIter++;
			}
			wordtmp = wordOld.third.substr(pos,letterTable_pIter->size());
			Triple<int,int,string> wordNew2(wordOld.first,wordOld.second+pos,wordtmp);
			wordList->insert(wordList_pIter,wordNew2);
			if (!isInsert)
			{
				isInsert = true;
				wordList_pIter_tmp = --wordList_pIter;
				wordList_pIter++;
			}
			wordtmp = wordOld.third.substr(pos+letterTable_pIter->size());
			if (wordtmp!= "")
			{
				Triple<int,int,string> wordNew3(wordOld.first,wordOld.second+pos+letterTable_pIter->size(),wordtmp);
				wordList->insert(wordList_pIter,wordNew3);
				if (!isInsert)
				{
					isInsert = true;
					wordList_pIter_tmp = --wordList_pIter;
					wordList_pIter++;
				}
			}
			//�Ƴ�ԭʼ����
			wordList->erase(wordList_pIter);
			//ָ��ָ���¹���ĵ��������繹����Ǹ�������֮ǰ�ĵ��ʲ������ܲ�֣����������ͷ��ʼ����
			wordList_pIter = wordList_pIter_tmp;
		}
	}
}

void Analyser::Analyze()
{
	//��ȡ����ռ���ű�
	list<string>* unGrabLetter = expression->GetUnGrabLetter();
	list<Triple<int,int,string> >::iterator wordList_pIter;
	for (wordList_pIter = wordList->begin();wordList_pIter != wordList->end();wordList_pIter++)
	{
		//���õ��������ĸ�����
		string selection = expression->CheckWord(wordList_pIter->third);
		//������Ͳ���
		if (selection == "unknown")
		{
			//�÷���ռ���ű��ֵ���
			DivideWord(unGrabLetter,wordList_pIter);
			//����ѡ�����ֺ�ĵ��������ĸ�����
			selection = expression->CheckWord(wordList_pIter->third);
		}
		if (selection == "unknown")
		{
			unknownWord.push_back(*wordList_pIter);
		}
		//д�ļ�
		WriteFile(wordList_pIter->first,wordList_pIter->second,wordList_pIter->third,selection);
		//�����ע�ͷ�������ʣ�µĵ��ʶ�����Ҫ�ж�
		if (selection == "comment") break;
	}
	//��յ���������ʼ��ȡ��һ��
	wordList->clear();
}

void Analyser::WriteFile(int row, int col, string word, string dfaName)
{
	fout<<row<<" "<<col<<" "<<word<<" "<<dfaName<<endl;
}

void Analyser::WriteUnknownWord()
{
	ofstream fout2;
	fout2.open("LexicalAnalyzerError.txt");
	fout2<<"The follow is the word Analyzer con't recognized"<<endl;
	list<Triple<int,int,string> >::iterator unknownWord_pIter;
	for (unknownWord_pIter = unknownWord.begin(); unknownWord_pIter != unknownWord.end(); unknownWord_pIter++)
	{
		fout2<<unknownWord_pIter->first<<" "<<unknownWord_pIter->second<<" "<<unknownWord_pIter->third<<" unknown"<<endl;
	}
	fout2.close();
}
