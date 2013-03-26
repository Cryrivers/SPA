#pragma once
#include "SPAType.h"
#include "Utility.h"

using namespace std;

class Operators
{
private:
	vector<int> _ASTNodeId;

public:
	Operators(void);
	~Operators(void);
	void addOperatorByASTNodeId(int ASTNodeId);
};

