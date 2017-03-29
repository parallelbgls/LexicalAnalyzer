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
				 cout<<"�﷨��"<<sss<<" ����ɹ�"<<endl;
			 }
			 else
			 {
				 cout<<"�﷨��"<<sss<<" ����ʧ�ܣ�����"<<endl;
			 }
			 flag=false;
		}
		if(c=='|')
		{
			ts=s.substr(0,s.length()-1);
			if (grammarCollector->Insert(ts))
			{
				cout<<"�﷨��"<<ts<<" ����ɹ�"<<endl;
			}
			else
			{
				cout<<"�﷨��"<<ts<<" ����ʧ�ܣ�����"<<endl;
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
					 cout<<"�﷨��"<<s<<" ����ɹ�"<<endl;
				}
				else
				{
					 cout<<"�﷨��"<<s<<" ����ʧ�ܣ�����"<<endl;
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
