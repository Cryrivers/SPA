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
	int getEdgeSize();
	bool containStatement(STMT stmtNumber);
	bool isValidCFGNode();
	CFGType getCFGType();
	void setCFGType(CFGType type);
	vector<CFGNode*> getNextEdges();
	vector<CFGNode*> getPrevEdges();
	void setProcIndex(PROC_INDEX procIndex);
	PROC_INDEX getProcIndex();
	void setPairedCFGNode(CFGNode* cfgNode);
	CFGNode* getPairedCFGNode();
	CFGBipType getBipType();
	void setBipType(CFGBipType type);
protected:
	void addBacktraceEdges(CFGNode* prevPath);
private:
	CFGBipType _bipType;
	int _start;
	int _end;
	PROC_INDEX _procIndex;
	vector<CFGNode*> _edges;
	vector<CFGNode*> _backtrace_edges;
	CFGType _type;
	CFGNode* _pairNode;
};

