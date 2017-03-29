#include "GrammarReader.h"

GrammarReader::GrammarReader(string contextFreeGrammarSourceFile)
{
	grammarCollector = new GrammarCollector();
	ifstream fin;
	fin.open("ContextFreeGrammar.txt");
	char c;
	int n=0;
	string s="";
	string ts="";
	string sa[5]={"","","","",""};
	bool flag=true;
	while((c=fin.get())!=EOF)
	{
		s=s+c;
		if(c==' '&&flag==true)
		{
			 string ss=s.substr(0,s.length()-1);
			 string sss=ss+"' -> "+ss;
			 if (grammarCollector->Insert(sss))
			 {
				 cout<<"语法："<<sss<<" 插入成功"<<endl;
			 }
			 else
			 {
				 cout<<"语法："<<sss<<" 插入失败，请检查"<<endl;
			 }
			 flag=false;
		}
		if(c=='|')
		{
			ts=s.substr(0,s.length()-1);
			if (grammarCollector->Insert(ts))
			{
				cout<<"语法："<<ts<<" 插入成功"<<endl;
			}
			else
			{
				cout<<"语法："<<ts<<" 插入失败，请检查"<<endl;
			}
			int i=s.find_first_of('>');
			s=s.substr(0,i+1);
		}
		if(c=='\n')
		{
			 if(n==0)
			 {
				s = s.substr(0,s.length()-1);
				if (grammarCollector->Insert(s))
				{
					 cout<<"语法："<<s<<" 插入成功"<<endl;
				}
				else
				{
					 cout<<"语法："<<s<<" 插入失败，请检查"<<endl;
				}
			 }
			 s="";
		}
	}
	fin.close();
}

GrammarReader::~GrammarReader()
{
	delete(grammarCollector);
}
