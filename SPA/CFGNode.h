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
	void connectTo(CFGNode* nextPath);
	void connectBackTo(CFGNode* prevPath);
	int getEdgeSize();
	bool containStatement(STMT stmtNumber);
	CFGType getCFGType();
	void setCFGType(CFGType type);
	vector<CFGNode*> getNextEdges();
private:
	int _start;
	int _end;
	vector<CFGNode*> _edges;
	CFGType _type;
};

