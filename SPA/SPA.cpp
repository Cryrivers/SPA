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
	AST    *ast = new AST();
	Parser *parser = new Parser(ast);

	string program, line;
	program = "";

	ifstream myfile("../SampleCode/CS3202Test1Code.c");

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

	
	const int querySize = 1;
	string query[querySize];	
	//string declares("call c; stmt s1,s2,s3,s4,s5,s6; procedure p1,p2; while w; prog_line n1,n2; variable v;");				  			 
	//query[0]=declares+"Select c such that Calls(p2,p1) with p1.procName = c.procName and p2.procName = \"Main\" ";										  
	//query[1]=declares+"Select w such that Parent(s1,s2) and Parent(s2,s3) and Parent(s3,s4) and Parent(s4,s5) and Parent(s5,w)";	
	//query[3]=declares+"Select s6 such that Parent(s1,s2) and Parent(s2,s3) and Parent(s3,s4) and Parent(s4,s5) and Parent(s5,s6)";	
	//query[2]=declares+"Select s6 such that Parent(s1,s2) and Parent(s2,s3) and Parent(s3,s4) and Parent(s4,s5) and Parent(s5,s6) with s6.stmt#=w.stmt#";								  
	query[0]="assign a1, a2;Select BOOLEAN such that Affects(3, 12)";

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
