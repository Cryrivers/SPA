#pragma once
#include <vector>
#include "CFGNode.h"
class CFG
{
public:
	CFG(void);
	~CFG(void);
	void addToCFG(CFGNode* cfgnode);
	CFGNode* getCFGNodeByStmtNumber(STMT stmtNumber);
	vector<CFGNode*> getAllCFGNodes();
	void __printDotGraphForGraphviz();
	int __indexOf(vector<CFGNode*> list, CFGNode* val);
private:
	vector<CFGNode*> _CFGBlocks;
};

