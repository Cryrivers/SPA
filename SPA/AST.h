#pragma once
#include "PKBController.h"
#include "ASTNode.h"
#include <regex>
#include <string>
#include <vector>
#include <stack>

using namespace std;

class PKBController;
class AST
{
private:
	ASTNode* _root;
	inline int _operatorPrecedence(char op);
	string _calcHashcode(ASTNode* node);
	bool _containTree(ASTNode* container, ASTNode* object);
	vector<ASTNode*> _astIndex;
public:
	AST(void);
	~AST(void);
	ASTNode* getRoot();
	ASTNode* buildSubtree(string expression);
	ASTNode* buildAssignmentSubtree(int varIndex, string expression);
	vector<int>* containSubtree(ASTNode* tree);
	vector<int>* containTree(ASTNode* tree);
	vector<int>* matchWhileLoop(VAR_INDEX ind);
	vector<int>* matchIfBranch(VAR_INDEX ind);
	void calculateHashcode();
	void setASTNodeIndex(ASTNode* node);
	ASTNode* getASTNodeById(int id);
};

