#pragma once

#include "AST.h"
#include "ASTNode.h"
#include "PKBController.h"
#include <string>
#include <stack>
#include <vector>
#include "SPAType.h"

using namespace std;

class Parser
{

private:

	PKBController* _pkb;
	vector<statement>* preprocProgram;

	ASTNode* previousNode;
	bool sameLevelAtNext;

	AST* _ast;
	stack<ASTNode*> _parentStack;
	stack<ASTNode*> _parentStackNoStmtLst;
	ASTNode* _newParent;
	ASTNode* _newParentNoStmtLst;

	ASTNode* _buildAssignmentAST(statement* s);
	ASTNode* _buildWhileLoopAST(statement* s);
	ASTNode* _buildProcedureAST(statement* s);
	ASTNode* _buildIfAST(statement* s);
	ASTNode* _buildElseAST(statement* s);
	inline int _findValidMinimumOfThree(size_t lb, size_t rb, size_t sc);
	inline int _operatorPrecedence(char op);

public:
	Parser(AST* ast);
	~Parser(void);
	void parse(string program);
	void parseLine();
	void preprocessProgram(string program);
};

