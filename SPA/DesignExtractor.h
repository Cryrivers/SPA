#pragma once
#include "AST.h"
#include "ASTNode.h"
#include "PKBController.h"
#include "SPAType.h"
#include "CFGNode.h"
#include <stack>

using namespace std;

class PKBController;
class AST;

class DesignExtractor
{
private:
AST* _ast;
PKBController* _pkb;
inline IfPreprocessingPhase __getParsingPhase(stack<IfPreprocessingPhase> &s);
inline void __smartConnectThisCFGToNext( CFGNode* nextNode, CFGNode* thisNode );

public:
DesignExtractor(void);
~DesignExtractor(void);

void connectCFG();
void addNext();
void addModifies();
void addUses();
BOOLEAN getAllStmt(STMT_LIST*);
BOOLEAN getAllAssignment(STMT_LIST*);
BOOLEAN getAllWhile(STMT_LIST*);
BOOLEAN getAllIf(STMT_LIST*);
BOOLEAN getAllCall(STMT_LIST*);
BOOLEAN getAllConstant(vector<int> *);
BOOLEAN pattern(vector<int>*, vector<int>*, string,PatternQueryType, int);
BOOLEAN affects(STMT_LIST*, STMT_LIST*, int);
BOOLEAN affectsStar(STMT_LIST*, STMT_LIST*, int);

private:
	BOOLEAN assignPattern(vector<int>*, vector<int>*, string, int, int);
	BOOLEAN whilePattern(vector<int>*, vector<int>*, string, int);
	BOOLEAN ifPattern(vector<int>*, vector<int>*, string, int);
	BOOLEAN affects00(STMT_LIST*, STMT_LIST*);
	BOOLEAN affects01(STMT_LIST*, STMT_LIST*);
	BOOLEAN affects10(STMT_LIST*, STMT_LIST*);
	BOOLEAN affects11(STMT_LIST*, STMT_LIST*);
	BOOLEAN isAffects(STMT, STMT);
	STMT_LIST getAffectsFirst(STMT, BOOLEAN);
	STMT_LIST getAffectsFirstHelper(CFGNode*, vector<CFGNode*>*, VAR_INDEX_LIST, BOOLEAN);
	STMT_LIST getAffectsSecond(STMT, BOOLEAN);
	STMT_LIST getAffectsSecondHelper(CFGNode*, vector<CFGNode*>*, VAR_INDEX_LIST, BOOLEAN);
};

