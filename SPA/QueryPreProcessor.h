#pragma once
#ifndef QUERYPREPROCESSOR_H
#define QUERYPREPROCESSOR_H
#define DEBUGMODE 0 /**<===== do not change  */
#define PRINTTABLE 0
#define DICCLAUSESIZE 4			/**< contain: "such that", "and", "pattern", "with"*/
#define DICDESIGNENTITYSIZE 10	/**< contain: "stmt", "assign", "while", "variable", "constant", "prog_line", "call", "if", "stmtlst", "procedure"*/
#define DICRELATIONREFSIZE 19	/**< contain: "Parent*", "Parent", "Follows*", "Follows", "Modifies", "Uses", "Affects*", "Affects", "Next*", "Next", "Calls*", "Calls"*/
#define DICATTRIBUTESIZE 5		/**< contain: "procName", "varName", "value", "stmt#", "prog_line#", "procName"*/
#include "PKBController.h"
/********************************************//**
 * @file QueryPreprocessor.h
 * @brief Header file for query preprocessor
 * @author Zhang Haoqiang
 * @date 30.01.2012
 * @version 2.01
 ***********************************************/
#include "QueryValidator.h"
/********************************************//**
 * @brief Parse in PQL query and build query tree 
 * @class  QueryPreprocessor
 * @details Used to parse in PQL query \n\n
 * Workflow: \n
 * 1. parse the query \n
 * 2. build query tree in table format \n
 * 3. validate the query \n
 * 4. make optimisation for fast performance \n
 * 5. set dependancy for query variables \n
 * @see QueryValidator
 * @see QueryProcessor
 * @see PKBController
 * @author Zhang Haoqiang
 * @date 30.01.2012
 * @version 1.00
 ***********************************************/
class QueryPreprocessor{  
private:
	/**@brief Used for merge same query variables. @see setupClaTable*/	  
	int mergedClause;  
	int discardClause;
	/**@brief  contain: "such that", "and", "pattern", "with"*/ 
	string dicClause[DICCLAUSESIZE];		
	/**@brief  contain: "stmt", "assign", "while", "variable", "constant", "prog_line", "call", "if", "stmtlst", "procedure"*/
	string dicDesignEntity[DICDESIGNENTITYSIZE];	
	/**@brief  contain: "Parent*", "Parent", "Follows*", "Follows", "Modifies", "Uses", "Affects*", "Affects", "Next*", "Next", "Calls*", "Calls"*/
	string dicRelationRef[DICRELATIONREFSIZE];		
	/**@brief  contain: "procName", "varName", "value", "stmt#", "prog_line#", "procName"*/
	string dicAttribute[DICATTRIBUTESIZE];			
	/**@brief  Provaide validate functions*/
	QueryValidator qv;	
	/**@brief  Used for getting indexes from pkb*/
	PKBController* pkb;
	vector<QueryVariable> queryVarTable;	/**< @see QueryVariable*/
	vector<QueryTarget>   queryTarTable;	/**< @see QueryTarget*/
	vector<QueryClause>   queryClaTable;	/**< @see QueryClause*/	   
	map<int, vector<QueryClause>> clasueTable;
	int getIndexOfFirstFrom (string b, string a[], int size);
	int getIndexOfNextFrom (string b, string a[], int size);
	int findNextIndexFrom (string b, string a[], int size);
	void trim (string &str, char del);
	void trim_all (string &str);
	vector<string> getDeclares (string &str); 
	vector<string> getTargets (string &str);
	vector<string> getClauses (string &str);
	vector<string> extractRelation (string str); 
	Type getAttributeOfVariable (int a, int b);
	int getIndexFromVarTable (string str, int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);
	bool setupVarTable (vector<string> declares);
	bool setupTarTable (vector<string> targets);
	bool setupClaTable (vector<string> clauses); 
	void changeMapTo(int from, int to);
	bool makeOptimize ();
	bool setDependency ();
public:
	//constructor
	QueryPreprocessor(void);
	~QueryPreprocessor(void);
	//parser 
	int parse(string input);
	//results
	vector<QueryVariable> getQueryVariableTable();
	vector<QueryTarget>   getQueryTargetTable();
	vector<QueryClause>   getQueryClauseTable(); 
	map<int, vector<QueryClause>> getClauseTable();
};
class QueryPreprocessorDebug{
public:
	QueryPreprocessorDebug(void);
	char* typeToString(Type t);
	void printQueryVariableTable (vector<QueryVariable> v);
	void printTargetVariableTable (vector<QueryTarget> v);
	void printQueryClauseTable (vector<QueryClause> v);
	void printClauseTable(map<int, vector<QueryClause>> v);
};
#endif
