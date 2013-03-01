// SPA.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Parser.h"
#include "SPAType.h"
#include "AST.h"
#include "ASTNode.h"
#include "QueryProcessor.h"
#include "PKBController.h"
#include <regex>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

string trim(string str, char del){
	string::size_type pos = str.find_last_not_of(del);
	if(pos != string::npos) {
		str.erase(pos + 1);
		pos = str.find_first_not_of(del);
		if(pos != string::npos) 
			str.erase(0, pos);
	}
	else 
		str.erase(str.begin(), str.end());
	return str;
} 

int _tmain(int argc, _TCHAR *argv[])
{
	AST    *ast = new AST();
	Parser *parser = new Parser(ast);

	string program, line;
	program = "";

	ifstream myfile("../SampleCode/CS3202-Test1Code.c");

	if (myfile.is_open())
	{
	int i=1;
		while ( myfile.good() )
		{
			getline(myfile,line);
			program += line;
			program += "\n";
			if(line.find("procedure")<line.size()){ 
				cout<<"     "<<line<<endl;
			}else if(line.find("else")<line.size()){
				cout<<"     "<<line<<endl;
			}else if(trim(line,'\t').size()<2){
				cout<<"     "<<line<<endl;
			}else{
				printf("%3d. ", i);
				cout<<line<<endl;
				i++;
			}
		}
		myfile.close();
	}else{
		cout<<"Read error!"<<endl;
	}
	
	SPA_TIME_MEASURE("Parser", parser->parse(program));
	
	QueryProcessor *processor = new QueryProcessor();
	list<string> result;
	
	string query("prog_line n1; stmt s; if ifstat; while w; variable v; Select n1 such that Next (n1, s) and Parent (ifstat, s) and Modifies (w, v) and Uses (s, v)");
	
	SPA_TIME_MEASURE("Processor",processor->evaluate(query,result));

	//Remove duplicates
	set<string> s(result.begin(), result.end());
	result.assign(s.begin(), s.end());

	while(result.size()>0)
	{
		cout << result.front() << "; ";
		result.pop_front();
	}
	cout << endl;

	getchar();
	return(0);
}
