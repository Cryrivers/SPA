#pragma once
#include <vector>
#include "CFGNode.h"
#include "PKBController.h"

class PKBController;
class CFG
{
public:
	CFG(void);
	~CFG(void);
	void addToCFG(CFGNode* cfgnode);
	CFGNode* getCFGNodeByStmtNumber(STMT stmtNumber);
	CFGNode* getFollowingCFGNodeByCurrentStmtNumber(STMT stmtNumber);
	CFGNode* getNextCFGNodeByCurrentStatement(statement s, bool bipEnabled);
	CFGNode* getBipDummyNodeByProcIndex(PROC_INDEX index);
	vector<CFGNode*> getAllCFGNodes();
	void __printDotGraphForGraphviz();
	int __indexOf(vector<CFGNode*> list, CFGNode* val);
private:
	vector<CFGNode*> _CFGBlocks;
};

