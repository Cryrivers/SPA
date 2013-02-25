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
	bool isValidCFGNode();
	CFGType getCFGType();
	void setCFGType(CFGType type);
	vector<CFGNode*> getNextEdges();
	void setProcIndex(PROC_INDEX procIndex);
	PROC_INDEX getProcIndex();
	void setPairedCFGNode(CFGNode* cfgNode);
	CFGNode* getPairedCFGNode();
private:
	int _start;
	int _end;
	PROC_INDEX _procIndex;
	vector<CFGNode*> _edges;
	CFGType _type;
	CFGNode* _pairNode;
};

