#include "StdAfx.h"
#include "Operators.h"


Operators::Operators(void)
{
}


Operators::~Operators(void)
{
}

void Operators::addOperatorByASTNodeId( int ASTNodeId )
{
	_ASTNodeId.push_back(ASTNodeId);
}
