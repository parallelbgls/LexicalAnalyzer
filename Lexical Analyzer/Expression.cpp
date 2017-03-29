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
	//以空格为分界符把正则表达式分为三个部分。
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
		//截取的字符
		string token = expression.substr(startpos,endpos-startpos);
		startpos = endpos;
		bool isSetDFA = false;
		if (token != "")
		{
			switch (i)
			{
			//设置或自动机名称
			case 0:
				{
					//是否设置插入位置
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
			//自动机赋值号
			case 1:
				{
					if (token != "::=")
					{
						cout<<"正则表达式赋值符错误，请修改"<<endl;
						isSetDFA = true;
					}
					break;
				}
			//正则表达式
			case 2:
				{
					//正则表达式
					re = token;
					//只要发现宏替换符(\\，就把\到)间的替换自动机名称找到，然后把宏替换成这个自动机的正则表达式
					while ((int)(re.find("(\\\\")) >= 0)
					{
						int startpos = re.find("(\\\\")+1;
						int endpos = startpos+1;
						while (re[endpos]!=')')
						{
							endpos++; 
						}
						//自动机名称
						string dfaName = re.substr(startpos+2,endpos-startpos-2);
						if (letter->find(dfaName) == letter->end())
						{
							if (regularExpression->find(dfaName) == regularExpression->end())
							{
								cout<<name<<"自动机构造错误，上文中没有相应的正则表达式——"<<dfaName<<"，请检查"<<endl;
								isSetDFA = true;
								break;
							}
							else
							{
								//宏替换
								re.replace(startpos,endpos-startpos,regularExpression->find(dfaName)->second->GetExpression());
							}
						}
						else
						{
							//宏替换
							re.replace(startpos,endpos-startpos,letter->find(dfaName)->second->GetExpression());
						}
					}
					if (isSetDFA)
					{
						break;
					}
					try
					{
						//构建自动机
						dfa = new DFA(name,re,previousNum);
					}
					catch(DFA *dfa)
					{
						cout<<"正则表达式"<<name<<"无法构造自动机，请检查"<<endl;
						isSetDFA = true;
						break;
					}
					switch(insertInto)
					{
					//插入字母表
					case 0:
						{
							letter->insert(pair<string,DFA*>(name,dfa));
							break;
						}
					//插入抢占符表和自动机表
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
					//插入自动机表
					case 2:
						{
							regularExpression->insert(pair<string,DFA*>(name,dfa));
							break;
						}
					//插入非抢占符表
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
					cout<<"正则表达式"<<name<<"读入完毕"<<endl;
					break;
				}
			}
		}
		//如果没有发现字符，终止循环
		else
		{
			break;
		}
		//如果是设置DFA插入位置，终止循环
		if (isSetDFA)
		{
			break;
		}
	}
}

string Expression::CheckWord(string word)
{
	//循环遍历所有regularExpression里的自动机，如果自动机的优先级更高（数字更小），单词就判断为该自动机
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
