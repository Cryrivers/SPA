#pragma once
#ifndef QUERYVALIDATOR_H
/**
 * include guards\n
 * Explain in wiki: 
 * <a href="http://en.wikipedia.org/wiki/Include%5Fguard">link</a>
 */
#define QUERYVALIDATOR_H
/********************************************//**
 * @file QueryValidator.h
 * @brief Header file for query validator
 * @author Zhang Haoqiang
 * @date 30.01.2012
 * @version 2.01
 ***********************************************/
#include "QueryDataType.h"
#include "SPAType.h"
/********************************************//** 
 * @class QueryValidator 
 * @brief Validate query elements according to PQL
 * 		  
 * @details
 * Used to validate query format based on PQL standards \n\n
 * Lexical rules: \n
 * LETTER : A-Z | a-z -- capital or small letter \n
 * DIGIT : 0-9 \n
 * IDENT : LETTER ( LETTER | DIGIT | '#' )* \n
 * INTEGER : DIGIT+ \n
 * Auxiliary grammar rules: \n\n
 * tuple : elem | '<' elem ( ',' elem )* '>' \n
 * elem : synonym | attrRef \n
 * synonym : IDENT \n
 * attrName : 'procName' | 'varName' | 'value' | 'stmt#' \n
 * entRef : synonym | '_' | '"' IDENT '"' | INTEGER \n
 * stmtRef : synonym | '_' | INTEGER \n
 * lineRef : synonym | '_' | INTEGER * \n
 * varRef : synonym | '_' | INTEGER * \n
 * expression-spec : '"' expr '"' | '_' '"' expr '"' '_' \n
 * @author Zhang Haoqiang
 * @date 30.01.2012
 * @version 1.00
 ***********************************************/
class QueryValidator{ 
public:
	QueryValidator(void);
	~QueryValidator(void);
	//Lexical rules:
	bool isLetter(char a);
	bool isDigit(char a);
	bool isIndent(string str);
	bool isInteger(string str);

	//Auxiliary grammar rules
	bool isElem(string str);
	bool isRef(string str);
	bool isRefpl(string str);
	bool isAttrRef(string str);
	bool isSynonym(string str);
	bool isStmtRef(string str);
	bool isEntRef(string str);
	bool isLineRef(string str);	
	bool isVarRef(string str);
	bool isNodeRef(string str);
	bool isExprSpec(string str);
};
#endif
