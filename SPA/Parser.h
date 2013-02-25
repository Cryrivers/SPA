#pragma once

#include "AST.h"
#include "ASTNode.h"
#include "CFG.h"
#include "CFGNode.h"
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
	//unsigned int - the index of statement in vector preprocProgram
	stack<unsigned int> statementScope;
	IfPreprocessingPhase preprocessingStatus;

	ASTNode* previousASTNode;
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
	ASTNode* _buildCallAST(statement* s);
	inline int _findValidMinimumOfThree(size_t lb, size_t rb, size_t sc);
	inline int _operatorPrecedence(char op);
	void _parseLine();
	void _preprocessProgram(string program);
	PROC_INDEX _findAssumedProcIndexByName(string name);

public:
	Parser(AST* ast);
	~Parser(void);
	void parse(string program);
	
};

