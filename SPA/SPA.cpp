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
	ifstream myfile("../SampleCode/AffectsStar.c");

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

	
	const int querySize = 3;
	string query[querySize];	
	//string declares("call c; stmt s1,s2,s3,s4,s5,s6; procedure p1,p2; while w; prog_line n1,n2; variable v;");				  			 
	//query[0]=declares+"Select c such that Calls(p2,p1) with p1.procName = c.procName and p2.procName = \"Main\" ";										  
	//query[1]=declares+"Select w such that Parent(s1,s2) and Parent(s2,s3) and Parent(s3,s4) and Parent(s4,s5) and Parent(s5,w)";	
	//query[3]=declares+"Select s6 such that Parent(s1,s2) and Parent(s2,s3) and Parent(s3,s4) and Parent(s4,s5) and Parent(s5,s6)";	
	//query[2]=declares+"Select s6 such that Parent(s1,s2) and Parent(s2,s3) and Parent(s3,s4) and Parent(s4,s5) and Parent(s5,s6) with s6.stmt#=w.stmt#";								  
	/*
	query[0]="Select BOOLEAN such that Affects (1, 2)"; //t
	query[1]="Select BOOLEAN such that Affects (1,3)"; //t
	query[2]="Select BOOLEAN such that Affects (1,5)"; //f
	query[3]="Select BOOLEAN such that Affects (2, 6)"; //t
	query[4]="Select BOOLEAN such that Affects (2, 8)"; //f
	query[5]="Select BOOLEAN such that Affects (3, 12)"; //f
	query[6]="Select BOOLEAN such that Affects (3, 13)"; //f
	query[7]="Select BOOLEAN such that Affects (16, 21)"; //f
	query[8]="assign a1;Select a1 such that Affects (a1, 1)"; //none
	query[9]="assign a1;Select a1 such that Affects (a1, 10)"; //12, 17
	query[10]="assign a2;Select a2 such that Affects (1, a2)"; //2, 3
	query[11]="assign a2;Select a2 such that Affects (13, a2)"; //16
	*/
	
	/*
	query[0]="Select BOOLEAN such that Affects* (1, 2)"; //true
	query[1]="Select BOOLEAN such that Affects* (1, 3)"; //true
	query[2]="Select BOOLEAN such that Affects* (1, 6)"; //true
	query[3]="Select BOOLEAN such that Affects* (1, 8)"; //true
	query[4]="Select BOOLEAN such that Affects* (11, 14)"; //false
	query[5]="Select BOOLEAN such that Affects* (20, 20)"; //false
	query[6]="assign a1;Select a1 such that Affects* (a1, 26)"; //25
	query[7]="assign a1;Select a1 such that Affects* (a1, 12)"; //12
	query[8]="assign a2;Select a2 such that Affects* (6, a2)"; //8
	query[9]="assign a2;Select a2 such that Affects* (20, a2)"; //none
	*/
	//query[0]="";
	query[0]="stmtLst s1,s2; if f; variable i; Select BOOLEAN such that Affects(3,3)"; 
	query[1]="stmtLst s1; while w; variable i; Select BOOLEAN pattern w(i,_)"; 	   
	query[2]="assign a; variable v; Select v such that Contains(a,v)"; 
	
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
