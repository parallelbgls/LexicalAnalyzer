#include<fstream>
#include<iostream>
#include<string>
#include"Analyser.h"
using namespace std;

int main()
{
	Analyser *analyser = new Analyser("SourceFile.txt","RegularExpression.txt","LexicalFile.txt");
	cout<<"�������"<<endl;
	cin.get();
	delete(analyser);
}
