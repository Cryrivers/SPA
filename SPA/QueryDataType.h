#pragma once
#ifndef QUERYDATATYPE_H
/**
 * include guards\n
 * Explain in wiki: 
 * <a href="http://en.wikipedia.org/wiki/Include%5Fguard">link</a>
 */
#define QUERYDATATYPE_H 
/********************************************//**
 * @file QueryDataType.h
 * @brief Global variables and enum for Query processor 
 * @warning This file must not be changed!\n 
       All processor will be affected
 * @author Zhang Haoqiang
 * @date 30.01.2012
 * @version 2.01
 ***********************************************/
#include <iostream>
#include <string>
#include <vector>
#include "stdafx.h"
using namespace std;
/********************************************//**
 * @enum Type
 * All the type info used for design entities, relations and special types\n\n
 * Classification: \n
 * DT: Types for design entities \n 
 * KT: Types for Known (Fixed) Variables \n 
 * AT: Types for Attributes \n 
 * RT: Types for design entity relationships \n 
 * CT: Types for other clauses \n 
 * ERROR: Placeholder \n 
 ***********************************************/
enum Type{
	//Types for design entities
	DT_PROCEDURE,		/**< design entity: procedure*/
	DT_STMTLST, 		/**< design entity: stmtLst*/
	DT_STMT, 			/**< design entity: stmt*/
	DT_ASSIGN, 			/**< design entity: assign part of stmt*/
	DT_CALL, 			/**< design entity: call   part of stmt*/
	DT_WHILE, 			/**< design entity: while  part of stmt*/
	DT_IF,				/**< design entity: if     part of stmt*/
	DT_VARIABLE, 		/**< design entity: variable*/
	DT_CONSTANT,		/**< design entity: constant*/
	DT_BOOLEAN,			/**< special type: boolean*/
	DT_UNDERSCORE,		/**< special type: '_'*/
	//Types for Known (Fixed) Variables
	KT_STMT_NUM,			/**< integers used in relation*/
	KT_KNOWN_VARIABLE,		/**< used in relation of form "INDENT"*/
	KT_KNOWN_PROCEDURE,		/**< used in relation of form "INDENT"*/
	KT_KNOWN_CONSTANT,		/**< constant c.vlaue=3*/
	KT_CONSTANT_INTEGER,	/**< integer for with and especially for calls*/	
	KT_CONSTANT_STRING,		/**< string for with and especially for calls*/
	//Types for Attributes
	AT_STMT_NUM,		/**< stmt.stmt#: INTEGER*/
	AT_VAR_NAME,		/**< variable.varName: NAME*/
	AT_PROC_NAME,		/**< procedure.procName: NAME*/
	
	AT_CALL_PROC_NAME,	/**< call.procName: NAME*/
	AT_VALUE,			/**< used for constant.value
*/	
	AT_PROCTABLEINDEX,	/**< used for calls in with clause*/
	//Types for design entity relationships
	RT_MODIFIESP,		/**< Relation: Modifies (procedure, variable)*/	
	RT_MODIFIESS,		/**< Relation: Modifies (stmt, variable)*/
	RT_USESP,			/**< Relation: Uses (procedure, variable)*/
	
	RT_USESS,			/**< Relation: Uses (stmt, variable)*/
	RT_CALLS,			/**< Relation: Calls (procedure 1, procedure 2)*/
	RT_CALLST,			/**< Relation: Calls* (procedure 1, procedure 2)*/
	RT_PARENT,			/**< Relation: Parent (stmt 1, stmt 2)*/
	RT_PARENTT,			/**< Relation: Parent* (stmt 1, stmt 2)*/
	RT_FOLLOWS,			/**< Relation: Follows (stmt 1, stmt 2)*/
	RT_FOLLOWST,		/**< Relation: Follows* (stmt 1, stmt 2)*/
	RT_NEXT,			/**< Relation: Next (prog_line 1, prog_line 2)*/
	RT_NEXTT,			/**< Relation: Next* (prog_line 1, prog_line 2)*/
	RT_AFFECTS,			/**< Relation: Affects (assign 1, assign2)*/
	RT_AFFECTST,		/**< Relation: Affects* (assign 1, assign2)*/
	//Types for other clauses
	CT_PATTERN,			/**< assign|if|while (entRef, expression-spec|'_')*/
	CT_WITH,			/**< with attriRef = attriRef*/
	//Other
	ERROR,				/**< place holder for setting attributes*/
};	
/********************************************//**
 * @class QueryVariable
 * @brief Table for all variables and interger in PQL
 * @sa QueryPreprocessor::setDependency
 * @author Zhang Haoqiang
 * @date 30.01.2012
 * @version 1.01
 ***********************************************/
class QueryVariable{
public:
	Type variableType;	/**< start with DT in enum TYpe*/
	int content; 		/**< if the variable has pkb index used for evaluator @todo content indicate known variable for var,proc and stmt, otherwise is set to -1*/
	string origin;		/**< used to indecate the original string, testing use*/
	int dependency;		/**< used to indecate the dependency among variables*/
	int countAppear;	/**< helper to count dependency*/
};
/********************************************//**
 * @class QueryTarget
 * @brief Table for query results: all elements in tuple
 * @author Zhang Haoqiang
 * @date 30.01.2012
 * @version 1.01
 ***********************************************/
class QueryTarget{
public:
	int varIndex;		/**< index from QueryVariableTable*/
	bool hasAttribute;	/**< if it has attribute, specially call*/
	Type attributeType;	/**< corrresponding type*/
};
/********************************************//**
 * @class QueryClause
 * @brief Table for all clauses: with, pattern, such that
 * @author Zhang Haoqiang
 * @date 30.01.2012
 * @version 1.01
 ***********************************************/
class QueryClause{
public:
	int index;			/**< used for optimising in evaluator*/
	Type relationType;	/**< clause and relation type*/
	int variable1;		/**< var1 in relation*/
	Type attribute1;	/**< only set if used in with clause*/
	int variable2;		/**< var2 in relation*/
	Type attribute2;	/**< only set if used in with clause*/
	string variable3;	/**< expr in pattern*/
	PatternQueryType patternType;	/**< differenciate assign, if and while*/
};

#endif
