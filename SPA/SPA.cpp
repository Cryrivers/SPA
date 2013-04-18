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

string _trim(string str, char del){
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
   
bool compare_number (std::string first, std::string second)
{
	return atoi(first.c_str())<atoi(second.c_str());
}

int _tmain(int argc, _TCHAR *argv[])
{
	AST    *ast = PKBController::createInstance()->getAST();
	Parser *parser = new Parser(ast);

	string program, line;
	program = "";

	//ifstream myfile("../SampleCode/CS3202Test1Code.c");
	ifstream myfile("../SampleCode/CS3202Test2Code.c");

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
			}else if(_trim(_trim(_trim(line,'\t'),'}'), ' ').size()<1){
				cout<<"     "<<line<<endl;
			}else if(_trim(_trim(_trim(line,'\t'),'{'), ' ').size()<1){
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

	
	const int querySize = 1;
	string query[querySize];	   
	//query[0]="";
	query[0]="stmt s; Select s such that Siblings(1,s)"; 
	
	for(int i=0; i<querySize; i++){						 						 
		cout<<"-------------------------------------------------------------------------------------"<<endl;
		cout<<"Query"<<i+1<<":\n"<<query[i]<<"\n"<<endl;
		SPA_TIME_MEASURE("Processor",processor->evaluate(query[i],result));
		set<string> s(result.begin(), result.end());
		result.assign(s.begin(), s.end());
		cout<<"Result"<<i+1<<":"<<endl;
		result.sort(compare_number);
		while(result.size()>0)
		{
			cout << result.front() << ", ";
			result.pop_front();
		}
		cout << endl;							
	}
	cout<<"-------------------------------------------------------------------------------------"<<endl;
			

	getchar();
	return(0);
}
