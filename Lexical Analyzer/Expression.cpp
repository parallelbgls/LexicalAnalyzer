#include"Expression.h"

Expression::Expression()
{
	Expression("");
}

Expression::Expression(string title)
{
	dfaCount = 0;
	letter = new map<string,DFA*>();
	grabLetter = new list<string>();
	unGrabLetter = new list<string>();
	regularExpression = new map<string,DFA*>();
	if (title != "")
	{
		const int LINE_LENGTH = 300;
		char str[LINE_LENGTH];
		ifstream fin(title.c_str());
		while (fin.getline(str,LINE_LENGTH))
		{
			string re(str);
			Insert(re, dfaCount);
			dfaCount++;
		}
	}
}

Expression::~Expression()
{
	delete(grabLetter);
	delete(unGrabLetter);
	map<string,DFA*>::iterator dfa_pIter;
	for (dfa_pIter = letter->begin(); dfa_pIter != letter->end(); dfa_pIter++)
	{
		delete(dfa_pIter->second);
	}
	delete(letter);
	for (dfa_pIter = regularExpression->begin(); dfa_pIter != regularExpression->end(); dfa_pIter++)
	{
		delete(dfa_pIter->second);
	}
	delete(regularExpression);
}

void Expression::Insert(string expression, int previousNum)
{
	string name;
	string re;
	DFA* dfa;
	int startpos = 0;
	int endpos = 0;
	//�Կո�Ϊ�ֽ����������ʽ��Ϊ�������֡�
	for (int i=0;i<3;i++)
	{
		while (startpos < (int)expression.size() && endpos < (int)expression.size() && expression[startpos] == ' ')
		{
			startpos++;
		}
		endpos = startpos;
		while (startpos < (int)expression.size() && endpos < (int)expression.size() && expression[endpos] != ' ')
		{
			endpos++;
		}
		//��ȡ���ַ�
		string token = expression.substr(startpos,endpos-startpos);
		startpos = endpos;
		bool isSetDFA = false;
		if (token != "")
		{
			switch (i)
			{
			//���û��Զ�������
			case 0:
				{
					//�Ƿ����ò���λ��
					isSetDFA = true;
					if (token.substr(0,2) == "//")
					{
						break;
					}
					if (token == "Letter:") insertInto = 0;
					else if (token == "GrabLetter:") insertInto = 1;
					else if (token == "Expression:") insertInto = 2;
					else if (token == "UnGrabLetter:") insertInto = 3;
					else
					{
						isSetDFA = false;
						name = token;
					}
					break;
				}
			//�Զ�����ֵ��
			case 1:
				{
					if (token != "::=")
					{
						cout<<"������ʽ��ֵ���������޸�"<<endl;
						isSetDFA = true;
					}
					break;
				}
			//������ʽ
			case 2:
				{
					//������ʽ
					re = token;
					//ֻҪ���ֺ��滻��(\\���Ͱ�\��)����滻�Զ��������ҵ���Ȼ��Ѻ��滻������Զ�����������ʽ
					while ((int)(re.find("(\\\\")) >= 0)
					{
						int startpos = re.find("(\\\\")+1;
						int endpos = startpos+1;
						while (re[endpos]!=')')
						{
							endpos++; 
						}
						//�Զ�������
						string dfaName = re.substr(startpos+2,endpos-startpos-2);
						if (letter->find(dfaName) == letter->end())
						{
							if (regularExpression->find(dfaName) == regularExpression->end())
							{
								cout<<name<<"�Զ����������������û����Ӧ��������ʽ����"<<dfaName<<"������"<<endl;
								isSetDFA = true;
								break;
							}
							else
							{
								//���滻
								re.replace(startpos,endpos-startpos,regularExpression->find(dfaName)->second->GetExpression());
							}
						}
						else
						{
							//���滻
							re.replace(startpos,endpos-startpos,letter->find(dfaName)->second->GetExpression());
						}
					}
					if (isSetDFA)
					{
						break;
					}
					try
					{
						//�����Զ���
						dfa = new DFA(name,re,previousNum);
					}
					catch(DFA *dfa)
					{
						cout<<"������ʽ"<<name<<"�޷������Զ���������"<<endl;
						isSetDFA = true;
						break;
					}
					switch(insertInto)
					{
					//������ĸ��
					case 0:
						{
							letter->insert(pair<string,DFA*>(name,dfa));
							break;
						}
					//������ռ������Զ�����
					case 1:
						{
							string::iterator token_pIter;
							for (token_pIter = token.begin(); token_pIter != token.end(); token_pIter++)	
							{
								if (*token_pIter == '\\')
								{
									token.erase(token_pIter);
								}
							}
							string letter;
							while ((int)(token.find('|')) >= 0)
							{
								int pos = token.find('|');
								letter = token.substr(0,pos);
								grabLetter->push_back(letter);
								token = token.substr(pos+1,token.size()-pos-1);
							}
							if (token != "")
							{
								grabLetter->push_back(token);
							}
							regularExpression->insert(pair<string,DFA*>(name,dfa));
							break;
						}
					//�����Զ�����
					case 2:
						{
							regularExpression->insert(pair<string,DFA*>(name,dfa));
							break;
						}
					//�������ռ����
					case 3:
						{
							string::iterator token_pIter;
							for (token_pIter = token.begin(); token_pIter != token.end(); token_pIter++)	
							{
								if (*token_pIter == '\\')
								{
									token.erase(token_pIter);
								}
							}
							string letter;
							while ((int)(token.find('|')) >= 0)
							{
								int pos = token.find('|');
								letter = token.substr(0,pos);
								unGrabLetter->push_back(letter);
								token = token.substr(pos+1,token.size()-pos-1);
							}
							if (token != "")
							{
								unGrabLetter->push_back(token);
							}
							regularExpression->insert(pair<string,DFA*>(name,dfa));
							break;
						}
					}
					cout<<"������ʽ"<<name<<"�������"<<endl;
					break;
				}
			}
		}
		//���û�з����ַ�����ֹѭ��
		else
		{
			break;
		}
		//���������DFA����λ�ã���ֹѭ��
		if (isSetDFA)
		{
			break;
		}
	}
}

string Expression::CheckWord(string word)
{
	//ѭ����������regularExpression����Զ���������Զ��������ȼ����ߣ����ָ�С�������ʾ��ж�Ϊ���Զ���
	int previousNum = INT_MAX;
	string whichSelect = "unknown";
	map<string,DFA*>::iterator dfa_pIter;
	for (dfa_pIter = regularExpression->begin(); dfa_pIter != regularExpression->end(); dfa_pIter++)
	{
		if (dfa_pIter->second->CheckWord(word))
		{
			if (dfa_pIter->second->GetPreviousNum()<previousNum)
			{
				previousNum = dfa_pIter->second->GetPreviousNum();
				whichSelect = dfa_pIter->second->GetName();
			}
		}
	}
	return whichSelect;
}
