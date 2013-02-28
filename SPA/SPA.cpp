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
	
	/*
	string query("assign a1,a2,a3; stmt s1,s2,s3,s4; if f; while w; call c; procedure p1, p2; variable v1, v2, v3; constant c1,c2; prog_line pg1, pg2;");
	query += "Select pg1 with pg1=s1.stmt#";
	query += " with a3.stmt#=s1.stmt# and v1.varName=v2.varName";
	query += " and p1.procName=v1.varName and s2.stmt#=s3.stmt# and s2.stmt#=f.stmt#";
	query += " and c1.value=3 and c1.value=c2.value and v1.varName=\"soc\"";
	query += " and pg1=s4.stmt# and pg1=pg2 and 1=1 and a1.stmt#=a1.stmt#";				 
	query += " pattern a1(v1, _) and f(v2,_,_) and w(v3,_) "; 
	query += " such that Parent(2,3) and Follows(w, a3) and Follows*(w,f) and Parent*(s3, s4)";
	query += " and Parent(_,_) and Affects*(_,_)";
	query += " ";
	query += " ";
	query += " ";
	query += " ";
	query += " ";
	*/
	string query("variable v;Select v such that Modifies(11, v)");						 
	
	SPA_TIME_MEASURE("Processor",processor->evaluate(query,result));

	while(result.size()>0)
	{
		cout << result.front() << "; ";
		result.pop_front();
	}
	cout << endl;
	/*
	string query1("assign a; variable v; Select a pattern a(v,_\"((k+e)*c)+2*(5-i)\"_) such that Uses(a,v)");
	string query2("assign a; while w; Select a such that Follows*(a,w) pattern a(_,_\"k*e\"_)");
	string query3("assign a; stmt s;Select a pattern a(_,_\"(k*e)\"_) such that Follows*(s,a)");
	//string query("stmt s; Select s such that Modifies(1,\"z\")");
	for(int i = 0; i < 10; i++){ 
	processor->evaluate(query1,result);processor->evaluate(query2, result);processor->evaluate(query3, result);
	while(result.size()>0)
	{
		cout << result.front() << endl;
		result.pop_front();
	}
	}
	*/
	//getchar();
	//string a("a=a+10+20+30+z");
	//regex m("/([0-9]+)/g");
	//smatch result;
	//regex_match(a,result,m);
	getchar();
	return(0);
}
