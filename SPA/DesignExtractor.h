#pragma once
#include "AST.h"
#include "ASTNode.h"
#include "PKBController.h"
#include "SPAType.h"
#include <stack>

using namespace std;

class PKBController;
class AST;

class DesignExtractor
{
private:
AST* _ast;
PKBController* _pkb;
inline IfPreprocessingPhase getParsingPhase(stack<IfPreprocessingPhase> &s);
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

private:
	BOOLEAN assignPattern(vector<int>*, vector<int>*, string, int, int);
	BOOLEAN whilePattern(vector<int>*, vector<int>*, string, int);
	BOOLEAN ifPattern(vector<int>*, vector<int>*, string, int);
};

