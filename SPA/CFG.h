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
private:
	vector<CFGNode*> _CFGBlocks;
};

