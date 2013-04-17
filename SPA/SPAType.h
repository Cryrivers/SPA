#pragma once
#include <vector>
#include <string>
#include <time.h> 
#include <math.h> 
using namespace std;
//Never change these values
#define CFG_BIP_DISABLED false
#define CFG_BIP_ENABLED true
#define SPA_TIME_MEASURE(r,cmd) do {clock_t __tm_t;__tm_t = clock();(cmd);__tm_t = clock() - __tm_t; printf ("\n\t\t");printf ("%s: %ld milliseconds.\n",(r), __tm_t);} while(0)
typedef int STMT;
typedef vector<int> STMT_LIST;
typedef bool BOOLEAN;
typedef int VAR_INDEX;
typedef int PROC_INDEX;
typedef int ASTNODE_INDEX;
typedef vector<int> VAR_INDEX_LIST;
typedef vector<int> PROC_INDEX_LIST;
typedef vector<int> ASTNODE_INDEX_LIST;
typedef string STRING;
typedef vector<string> STRING_LIST;

enum ASTNodeType {
	AST_UNKNOWN,
	AST_PROGRAM,
	AST_PROCEDURE,
	AST_STATEMENT_LIST,
	AST_STMT,
	AST_VARIABLE,
	AST_CONSTANT,
	AST_PLUS,
	AST_MINUS,
	AST_MULTIPLY,
	AST_ASSIGNMENT,
	AST_WHILE_LOOP,
	AST_IF_BRANCH,
	AST_ELSE_BRANCH,
	AST_CALL,
	AST_ANY
};

enum StmtType {
	STMT_ASSIGNMENT,
	STMT_WHILE,
	STMT_CALL,
	STMT_OPEN_BRACKET,
	STMT_CLOSE_BRACKET_END_OF_PROC,
	STMT_CLOSE_BRACKET_END_OF_THEN,
	STMT_CLOSE_BRACKET_END_OF_ELSE,
	STMT_CLOSE_BRACKET_END_OF_WHILE,
	STMT_IF,
	STMT_ELSE,
	STMT_PROCEDURE,
	STMT_NONE
};

/**
 * \enum	CFGType
 * \deprecated
 * \brief	Values that represent CFGType.
 */

enum CFGType{
	CFG_NORMAL_BLOCK,
	CFG_IF_STATEMENT,
	CFG_WHILE_STATEMENT,
	CFG_CALL_STATEMENT,
	CFG_DUMMY
};

enum CFGBipType {
	CFG_BIP_NORMAL,
	CFG_BIP_IN,
	CFG_BIP_OUT
};

enum WithType{
	WITH_VARNAME,
	WITH_PROCNAME,
	WITH_CALLPROCNAME,
	WITH_STMTNUMBER,
	WITH_VALUE
};

/**
 * \struct	stmtDetail
 *
 * \brief	The data structure containing all information of each line of code during the pre-
 * 			process stage.
 *
 * \author	Wang Zhongliang
 * \date	2013/1/29
 */

struct stmtDetail {

	/**
	 * \brief	The Statement number.
	 */

	int stmtNumber; 

	/**
	 * \brief	Procedure Index, if any.
	 */

	int procIndex;

	/**
	 * \brief	 The actual program statement.
	 */

	string stmtLine;

	/**
	 * \brief	Statement Type.
	 */

	StmtType type;

	/**
	 * \brief	If the type is STMT_CLOSE_BRACKET, startOfTheScope must be set to record the start of current scope.
	 */

	STMT startOfTheScope;

	/**
	 * \brief	If the type is IF, midOfScope must be set to recoard the end of THEN block.
	 */

	STMT midOfTheScope;

	/**
	 * \brief	If the type is PROC, WHILE or IF, startOfTheScope must be set to record the end of current scope.
	 */

	STMT endOfTheScope;

	/**
	 * \brief	The extra field for procedure name.
	 * 			If a statement <b>st</b> which is a pre-processed statement object of
	 * 			statement "procedure foo {", Then there is:
	 * \code {cpp}
	 * 		 st.stmtLine = "procedure foo {";
	 * \endcode
	 * 		   and the extraName of <b>st</b> is:
	 * \code {cpp}
	 * 		 st.extraName = "foo";
	 * \endcode
	 */

	string extraName;

	/**
	 * \brief	Extra field for the condition in IF and WHILE blocks.
	 */

	string extraCond;

	/**
	 * \brief	Varible Name at left-hand side of an assignment. For instance,
	 * 			in the assignment "a=b+c*(d-e);", the extraVar of this assignment is <b>a</b>.
	 */

	string extraVar; 

	/**
	 * \brief	Expression at right-hand side of an assignment. For instance,
	 * 			in the assignment "a=b+c*(d-e);", the extraExpr of this assignment is <b>b+c*(d-e)</b>.
	 * 			
	 */

	string extraExpr;
};

typedef struct stmtDetail statement;

enum PatternQueryType
{
	PATTERN_IF,
	PATTERN_WHILE,
	PATTERN_ASSIGN_UNDERSCORE_ONLY,
	PATTERN_ASSIGN_WITH_UNDERSCORE,
	PATTERN_ASSIGN_NO_UNDERSCORE
};

enum IfPreprocessingPhase
{
	PREPROCESS_THEN,
	PREPROCESS_ELSE,
	PREPROCESS_NORMAL_BLOCK,
	PREPROCESS_WHILE
};

struct visitedStatus
{
	STMT stmtNum;
	STMT_LIST visited;
};