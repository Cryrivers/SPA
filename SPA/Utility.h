#include <iostream>
#include <vector>
#include "CFGNode.h"
#include "ASTNode.h"
using namespace std;

int indexOf(vector<int>, int); //{
//for (int i = 0; i < list.size(); i++){
//	if(list.at(i) == val) return i;
//}
//return -1;
//}
int indexOf(vector<CFGNode*>, CFGNode*);
int indexOf(vector<ASTNode*>, ASTNode*);
StmtType getTypeOfStmt(STMT);