#include "stdafx.h"
#include "Utility.h"
#include "CFGNode.h"
#include "PKBController.h"

int indexOf(vector<int> list, int val)
{
	for (int i = 0; i < list.size(); i++) {
		if (list.at(i) == val) {
			return(i);
		}
	}
	return(-1);
}

int indexOf(vector<CFGNode*> list, CFGNode* val)
{
	for (int i = 0; i < list.size(); i++) {
		if (list.at(i) == val) {
			return(i);
		}
	}
	return(-1);
}

StmtType getTypeOfStmt(STMT st){
	vector<statement>* program = PKBController::createInstance()->getPreprocessedProgram();
	for (int i = 0; i < program->size(); i++)
	{
		if (program->at(i).stmtNumber == st)
		{
			StmtType t = program->at(i).type;
			if(t == STMT_ASSIGNMENT ||
			   t == STMT_CALL ||
			   t == STMT_IF ||
			   t == STMT_WHILE)
			{
				return program->at(i).type;
			}
		}
	}
	return STMT_NONE;
}
