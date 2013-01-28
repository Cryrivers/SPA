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

struct stmtDetail {
	int stmtNumber;
	int procIndex;
	string stmtLine;
	StmtType type;
	//Extra fields for PROCEDURE
	string extraName;
	//Extra fields for IF-ELSE and WHILE
	string extraCond;
	//Extra fields for assignments
	string extraVar;
	string extraExpr;
};

typedef struct stmtDetail statement;
