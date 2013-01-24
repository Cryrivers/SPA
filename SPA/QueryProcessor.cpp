#include "stdafx.h"
#include "QueryProcessor.h"

QueryProcessor::QueryProcessor(void){ 
	/****************************************************************
	* qPre and qEval already created???
	****************************************************************/
	cout<<"QueryProcessor constructed!"<<endl;
}

QueryProcessor::~QueryProcessor(void){	
	cout<<"QueryProcessor deconstructed!"<<endl;
}

void QueryProcessor::evaluate(string query, list<string>& results){
	if (qPre.parse(query) == 0) {
		// parse success
		qEval.evaluate(qPre.getQueryClauseTable(), qPre.getQueryVariableTable(), qPre.getTargetVariable(), results);
	}
}

/*
void QueryProcessor::printTargetVariable(int s){
cout<<"TargetVariable: \n\t\t["<<s<<"]"<<endl;
}

void QueryProcessor::printQueryVariableTable(vector<Query_variable> v){
cout<<"QueryVariableTable:"<<endl;
for(int i = 0; i< (int) v.size(); i++){
cout<<"\tind["<<i<<"]\ttyp["<<v[i].type<<"]\tdep["<<v[i].dependency<<"]\tcont[\""<<v[i].content<<"\"] count["<<v[i].countAppear<<"]"<<endl;
}
}

void QueryProcessor::printQueryClauseTable(vector<Query_clause> v){
cout<<"QueryClauseTable:"<<endl;
for(int i = 0; i< (int) v.size(); i++){
if(v[i].relation==v[i].QC_PATTERN){
cout<<"\tind["<<v[i].index<<"]\trel["<<v[i].relation<<"]\tvar1["<<v[i].variable1<<"]\tvar2["<<v[i].variable2<<"]"<<" var3["<<v[i].variable3<<"]"<<endl;
}else{
cout<<"\tind["<<v[i].index<<"]\trel["<<v[i].relation<<"]\tvar1["<<v[i].variable1<<"]\tvar2["<<v[i].variable2<<"]"<<endl;
}
}
}



int QueryProcessor::test() {

QueryPreprocessor q = QueryPreprocessor::QueryPreprocessor(); 

q.flagerr=1;
int showerr=1;


int res = 0;
string query = "";

//test
query += "assign a; variable x; stmt s;\n";
query += "Select a such that ModifiesS (s,\"x\");"; 
cout<<"\n\n======================================================================="<<endl;
cout<<"Query: \n\t"<<query.substr(0,query.find("\n"))<<"\n\t"<<query.substr(query.find("\n")+1)<<endl;
res = q.parse(query);
if(res!=0){
cout<<"\tQuery processing error"+res<<endl;
}
if(res==0||showerr==1){
printTargetVariable(q.getTargetVariable()); 
printQueryVariableTable(q.getQueryVariableTable());
printQueryClauseTable(q.getQueryClauseTable());
}
query.clear();

//test
query += "assign a; variable x;\n";
query += "Select a such that ModifiesS (a,x);"; 
cout<<"\n\n======================================================================="<<endl;
cout<<"Query: \n\t"<<query.substr(0,query.find("\n"))<<"\n\t"<<query.substr(query.find("\n")+1)<<endl;
res = q.parse(query);
if(res!=0){
cout<<"\tQuery processing error"+res<<endl;
}
if(res==0||showerr==1){
printTargetVariable(q.getTargetVariable()); 
printQueryVariableTable(q.getQueryVariableTable());
printQueryClauseTable(q.getQueryClauseTable());
}
query.clear();

//test
query += "assign a; variable x;\n";
query += "Select a such that pattern a(x,\"_x_\");"; 
cout<<"\n\n======================================================================="<<endl;
cout<<"Query: \n\t"<<query.substr(0,query.find("\n"))<<"\n\t"<<query.substr(query.find("\n")+1)<<endl;
if(q.parse(query)==0||showerr==1){
printTargetVariable(q.getTargetVariable()); 
printQueryVariableTable(q.getQueryVariableTable());
printQueryClauseTable(q.getQueryClauseTable());
}
query.clear();

//test
query += "assign a;\n";
query += "Select a such that Parent(a,2) and Follows(a,8);";
cout<<"\n\n======================================================================="<<endl;
cout<<"Query: \n\t"<<query.substr(0,query.find("\n"))<<"\n\t"<<query.substr(query.find("\n")+1)<<endl;
if(q.parse(query)==0||showerr==1){
printTargetVariable(q.getTargetVariable()); 
printQueryVariableTable(q.getQueryVariableTable());
printQueryClauseTable(q.getQueryClauseTable());
}
query.clear();

//test
query += "stmt s; assign a; while w; stmt s2; variable v;\n";
query += "Select s such that Parents(w,a) and Follows(s2,w) pattern a(v,\"expr\");";
cout<<"\n\n======================================================================="<<endl;
cout<<"Query: \n\t"<<query.substr(0,query.find("\n"))<<"\n\t"<<query.substr(query.find("\n")+1)<<endl;
if(q.parse(query)==0||showerr==1){
printTargetVariable(q.getTargetVariable()); 
printQueryVariableTable(q.getQueryVariableTable());
printQueryClauseTable(q.getQueryClauseTable());
}
query.clear();

//test
query += "assign a; while w; stmt s1; stmt s2; variable x; variable y;\n";
query += "Select w such that Parent (a,2) and Follows (w,3) and Follows (s1,s2) and Parent (s2,s1) and ModifiesS (a,x) and UsesS(w,y) pattern a(x, \"_x+y_\");";
cout<<"\n\n======================================================================="<<endl;
cout<<"Query: \n\t"<<query.substr(0,query.find("\n"))<<"\n\t"<<query.substr(query.find("\n")+1)<<endl;
if(q.parse(query)==0||showerr==1){
printTargetVariable(q.getTargetVariable()); 
printQueryVariableTable(q.getQueryVariableTable());
printQueryClauseTable(q.getQueryClauseTable());
}
query.clear();




getchar();
return 0;
}
*/