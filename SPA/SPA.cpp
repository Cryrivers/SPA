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

int _tmain(int argc, _TCHAR *argv[])
{
	AST    *ast = new AST();
	Parser *parser = new Parser(ast);

	string program, line;
	program = "";

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

	const int querySize = 4;
	string query[querySize];	
	string declares("stmt s1,s2,s3,s4,s5,s6; variable v1, v2; procedure p1,p2; while w; ");				  			 
	query[0]=declares+"Select s1 such that Parent*(s1,s2) and Parent*(s2,s3) and Parent*(s3,s4) ";	
	query[1]=declares+"Select s1 such that Parent*(s1,s2) and Parent*(s2,s3) and Parent*(s3,s4) with s4.stmt#=w.stmt#";	
	query[2]=declares+"Select s1 such that Parent*(s1,s2) and Parent*(s2,s3) and Parent*(s4,s5) and Parent*(s5,s6) ";	
	query[3]=declares+"Select s1 such that Parent*(s1,s2) and Parent*(s2,s3) and Parent*(s4,s5) and Parent*(s5,s6) with s3.stmt#=s4.stmt# ";									  
	
	for(int i=0; i<querySize; i++){						 						 
		cout<<"-------------------------------------------------------------------------------------"<<endl;
		cout<<"Query"<<i+1<<":\n"<<query[i]<<"\n"<<endl;
		SPA_TIME_MEASURE("Processor",processor->evaluate(query[i],result));
		set<string> s(result.begin(), result.end());
		result.assign(s.begin(), s.end());
		cout<<"Result"<<i+1<<":"<<endl;
		while(result.size()>0)
		{
			cout << result.front() << "; ";
			result.pop_front();
		}
		cout << endl;							
	}	

	getchar();
	return(0);
}
