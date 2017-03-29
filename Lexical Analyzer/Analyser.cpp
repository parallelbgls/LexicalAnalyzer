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
		//如果行为空，跳过读取
		if (lan == "") return true;
		//按空格拆分单词
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
		
		//按抢占符号表拆分单词
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
	//拆分符号表的遍历指针
	list<string>::iterator letterTable_pIter;
	for (letterTable_pIter = letterTable->begin(); letterTable_pIter != letterTable->end(); letterTable_pIter++)
	{
		//当指针所指的单词中有拆分符号表里的符号
		while ((int)(wordList_pIter->third.find(*letterTable_pIter)) >=0)
		{
			//将单词拆为三个部分
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
				//由于指针前后移动具有破坏性，重新移动指针到原始的位置
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
			//移除原始单词
			wordList->erase(wordList_pIter);
			//指针指向新构造的单词中最早构造的那个，由于之前的单词不可能能拆分，所以无需从头开始遍历
			wordList_pIter = wordList_pIter_tmp;
		}
	}
}

void Analyser::Analyze()
{
	//获取非抢占符号表
	list<string>* unGrabLetter = expression->GetUnGrabLetter();
	list<Triple<int,int,string> >::iterator wordList_pIter;
	for (wordList_pIter = wordList->begin();wordList_pIter != wordList->end();wordList_pIter++)
	{
		//检查该单词属于哪个类型
		string selection = expression->CheckWord(wordList_pIter->third);
		//如果类型不明
		if (selection == "unknown")
		{
			//用非抢占符号表拆分单词
			DivideWord(unGrabLetter,wordList_pIter);
			//重新选择则拆分后的单词属于哪个类型
			selection = expression->CheckWord(wordList_pIter->third);
		}
		if (selection == "unknown")
		{
			unknownWord.push_back(*wordList_pIter);
		}
		//写文件
		WriteFile(wordList_pIter->first,wordList_pIter->second,wordList_pIter->third,selection);
		//如果是注释符，该行剩下的单词都不需要判断
		if (selection == "comment") break;
	}
	//清空单词链表，开始读取下一行
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
