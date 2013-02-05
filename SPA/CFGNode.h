#pragma once
#include <vector>
#include "SPAType.h"
class CFGNode
{
public:
	CFGNode(void);
	~CFGNode(void);
	void setStartStatement(STMT startStatement);
	void setEndStatement(STMT endStatement);
	STMT getStartStatement();
	STMT getEndStatement();
	void addEdge(CFGNode* nextPath);
	int getEdgeSize();
	bool containStatement(STMT stmtNumber);
	CFGType getCFGType();
	void setCFGType(CFGType type);
private:
	int _start;
	int _end;
	vector<CFGNode*> _edges;
	CFGType _type;
};

