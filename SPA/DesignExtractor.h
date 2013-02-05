#pragma once
#include "AST.h"
#include "ASTNode.h"
#include "PKBController.h"
#include "SPAType.h"

using namespace std;

class PKBController;
class AST;

class DesignExtractor
{
private:
AST* _ast;
PKBController* _pkb;

public:
DesignExtractor(void);
~DesignExtractor(void);

void addModifies();
void addUses();
BOOLEAN getAllStmt(STMT_LIST*);
BOOLEAN getAllAssignment(STMT_LIST*);
BOOLEAN getAllWhile(STMT_LIST*);
BOOLEAN getAllIf(STMT_LIST*);
BOOLEAN getAllCall(STMT_LIST*);
BOOLEAN getAllConstant(vector<int> *);
BOOLEAN pattern(vector<int>*, vector<int>*, string, int);
BOOLEAN with(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr,int arg1,int arg2);

};

