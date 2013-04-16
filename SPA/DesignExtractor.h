#pragma once
#include "AST.h"
#include "ASTNode.h"
#include "PKBController.h"
#include "SPAType.h"
#include "CFGNode.h"
#include "CFG.h"
#include <stack>

using namespace std;

class PKBController;
class AST;
class CFG;

class DesignExtractor
{
private:
AST* _ast;
PKBController* _pkb;
inline IfPreprocessingPhase __getParsingPhase(stack<IfPreprocessingPhase> &s);
inline void __smartConnectThisCFGToNext( CFGNode* nextNode, CFGNode* thisNode );
inline void __connectAssignmentAndCall( stack<IfPreprocessingPhase> &phaseStack, vector<statement>::iterator &it, stack<statement> &scope, CFGNode* nextNode, bool bipEnabled, CFGNode* thisNode, CFG* cfg );

public:
DesignExtractor(void);
~DesignExtractor(void);

void connectCFG(CFG* cfg, bool bipEnabled);


void addNext();
void addModifies();
void addUses();
BOOLEAN getAllStmt(STMT_LIST*);
BOOLEAN getAllAssignment(STMT_LIST*);
BOOLEAN getAllWhile(STMT_LIST*);
BOOLEAN getAllIf(STMT_LIST*);
BOOLEAN getAllCall(STMT_LIST*);
BOOLEAN getAllConstant(vector<int> *);
BOOLEAN getAllStmtList(vector<int> *);
BOOLEAN pattern(vector<int>*, vector<int>*, string,PatternQueryType, int);
BOOLEAN affects(STMT_LIST*, STMT_LIST*, int);
BOOLEAN affectsStar(STMT_LIST*, STMT_LIST*, int);
BOOLEAN contains(vector<int>*, vector<int>*, ASTNodeType argA, ASTNodeType argB, int arg);
BOOLEAN containsStar(vector<int>*, vector<int>*, ASTNodeType argA, ASTNodeType argB, int arg);
BOOLEAN sibling(vector<int>* indexAs, vector<int>* indexBs, ASTNodeType argA, ASTNodeType argB, int arg);
BOOLEAN isContains(int indexA, int indexB, ASTNodeType argA, ASTNodeType argB);
BOOLEAN isContainsStar(int indexA, int indexB, ASTNodeType argA, ASTNodeType argB);
BOOLEAN isSibling(int indexA, int indexB, ASTNodeType argA, ASTNodeType argB);

BOOLEAN isAffects(STMT, STMT);
BOOLEAN isAffectsStar(STMT, STMT);

private:
	BOOLEAN assignPattern(vector<int>*, vector<int>*, string, int, int);
	BOOLEAN whilePattern(vector<int>*, vector<int>*, string, int);
	BOOLEAN ifPattern(vector<int>*, vector<int>*, string, int);
	BOOLEAN affects00(STMT_LIST*, STMT_LIST*);
	BOOLEAN affects01(STMT_LIST*, STMT_LIST*);
	BOOLEAN affects10(STMT_LIST*, STMT_LIST*);
	BOOLEAN affects11(STMT_LIST*, STMT_LIST*);
	BOOLEAN affectsStar00(STMT_LIST*, STMT_LIST*);
	BOOLEAN affectsStar01(STMT_LIST*, STMT_LIST*);
	BOOLEAN affectsStar10(STMT_LIST*, STMT_LIST*);
	BOOLEAN affectsStar11(STMT_LIST*, STMT_LIST*);
	
	STMT_LIST getAffectsFirst(STMT, BOOLEAN);
	//STMT_LIST getAffectsFirstHelper(CFGNode*, vector<CFGNode*>*, VAR_INDEX_LIST, BOOLEAN);
	void getAffectsFirstHelper(CFGNode*, vector<CFGNode*>*, VAR_INDEX_LIST, BOOLEAN, STMT_LIST*);
	STMT_LIST getAffectsSecond(STMT, BOOLEAN);
	//STMT_LIST getAffectsSecondHelper(CFGNode*, vector<CFGNode*>*, VAR_INDEX_LIST, BOOLEAN);
	void getAffectsSecondHelper(CFGNode*, vector<CFGNode*>*, VAR_INDEX_LIST, BOOLEAN, STMT_LIST*);
	
	void isAffectsStarHelper(STMT stmt1, STMT stmt2, STMT_LIST* visited_p, BOOLEAN* result_p);
	STMT_LIST getAffectsStarFirst(STMT, BOOLEAN);
	void getAffectsStarFirstHelper(STMT, STMT_LIST*);
	STMT_LIST getAffectsStarSecond(STMT, BOOLEAN);
	void getAffectsStarSecondHelper(STMT, STMT_LIST*);

	STMT_LIST getContainsFirst(int indexB, ASTNodeType argA, ASTNodeType argB);
	STMT_LIST getContainsSecond(int indexA, ASTNodeType argA, ASTNodeType argB);
	BOOLEAN contains_00(vector<int>* indexAs, vector<int> *indexBs, ASTNodeType argA, ASTNodeType argB);
	BOOLEAN contains_01(vector<int>* indexAs, vector<int> *indexBs, ASTNodeType argA, ASTNodeType argB);
	BOOLEAN contains_10(vector<int>* indexAs, vector<int> *indexBs, ASTNodeType argA, ASTNodeType argB);
	BOOLEAN contains_11(vector<int>* indexAs, vector<int> *indexBs, ASTNodeType argA, ASTNodeType argB);
	BOOLEAN containsStar_00(vector<int>* indexAs, vector<int>* indexBs, ASTNodeType argA, ASTNodeType argB);
	BOOLEAN containsStar_01(vector<int>* indexAs, vector<int>* indexBs, ASTNodeType argA, ASTNodeType argB);
	BOOLEAN containsStar_10(vector<int>* indexAs, vector<int>* indexBs, ASTNodeType argA, ASTNodeType argB);
	BOOLEAN containsStar_11(vector<int>* indexAs, vector<int>* indexBs, ASTNodeType argA, ASTNodeType argB);
	BOOLEAN sibling_00(vector<int>* indexAs, vector<int> *indexBs, ASTNodeType argA, ASTNodeType argB);
	BOOLEAN sibling_01(vector<int>* indexAs, vector<int> *indexBs, ASTNodeType argA, ASTNodeType argB);
	BOOLEAN sibling_10(vector<int>* indexAs, vector<int> *indexBs, ASTNodeType argA, ASTNodeType argB);
	BOOLEAN sibling_11(vector<int>* indexAs, vector<int> *indexBs, ASTNodeType argA, ASTNodeType argB);
	void containsStarCase1aRecursive(ASTNode* root, ASTNodeType type, BOOLEAN* result);
	void containsStarCase1bRecursive(ASTNode* root, ASTNodeType type, vector<int> val, BOOLEAN* result);
	void containsStarCase2aRecursive(ASTNode* root, ASTNodeType type, vector<int>* indexs);
	void containsStarCase2bRecursive(ASTNode* root, ASTNodeType type, vector<int> vals, vector<int>* indexs);
	void containsStarCase4aRecursive(ASTNode* root, ASTNodeType type, vector<int>* indexs, int* result);
	void containsStarCase4bRecursive(ASTNode* root, ASTNodeType type, vector<int> vals, vector<int>* indexs, int* result);
};

