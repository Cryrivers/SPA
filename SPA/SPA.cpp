// SPA.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Parser.h"
#include "AST.h"
#include "ASTNode.h"
#include "QueryProcessor.h"
#include "PKBController.h"
#include <regex>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

int _tmain(int argc, _TCHAR *argv[])
{
	AST    *ast = new AST();
	Parser *parser = new Parser(ast);

	string program, line;
	program = "";
	ifstream myfile("C:\\SampleWhileLoop.c");
	if (myfile.is_open())
	{
		while ( myfile.good() )
		{
			getline(myfile,line);
			program += line;
			program += "\n";
		}
		myfile.close();
		cout<<program<<endl;
	}

	parser->parse(program);

	QueryProcessor *processor = new QueryProcessor();
	list<string> result;
	
	string query("assign a; variable v; procedure p; Select a such that Modifies(a, \"x\")");
	processor->evaluate(query,result);

	while(result.size()>0)
	{
		cout << result.front() << endl;
		result.pop_front();
	}

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
	getchar();
	//string a("a=a+10+20+30+z");
	//regex m("/([0-9]+)/g");
	//smatch result;
	//regex_match(a,result,m);
	//getchar();
	return(0);
}
