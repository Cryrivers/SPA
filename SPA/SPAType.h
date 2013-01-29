#pragma once
#include <vector>
#include <string>
using namespace std;

typedef int STMT;
typedef vector<int> STMT_LIST;
typedef bool BOOLEAN;
typedef int VAR_INDEX;
typedef int PROC_INDEX;
typedef vector<int> VAR_INDEX_LIST;
typedef vector<int> PROC_INDEX_LIST;
typedef string STRING;
typedef vector<string> STRING_LIST;

enum ASTNodeType {
	AST_UNKNOWN,
	AST_PROGRAM,
	AST_PROCEDURE,
	AST_STATEMENT_LIST,
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
	STMT_CLOSE_BRACKET,
	STMT_IF,
	STMT_ELSE,
	STMT_PROCEDURE,
	STMT_NONE
};

enum WithType{
	WITH_VARNAME,
	WITH_PROCNAME,
	WITH_CALLPROCNAME,
	WITH_STMTNUMBER
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
