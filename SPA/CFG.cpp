/**
 * \file	CFG.cpp
 *
 * \brief	Implements the configuration class.
 */

#include "StdAfx.h"
#include <assert.h>
#include "CFG.h"


CFG::CFG(void)
{
}


CFG::~CFG(void)
{
}

/**
 * \fn	void CFG::addToCFG( CFGNode* cfgnode )
 *
 * \brief	Adds to the configuration.
 *
 * \author	Wang Zhongliang
 * \date	2013/2/5
 *
 * \param [in]	cfgnode	If non-null, the cfgnode.
 */

void CFG::addToCFG( CFGNode* cfgnode )
{
	assert(cfgnode->getStartStatement() <= cfgnode->getEndStatement());
	_CFGBlocks.push_back(cfgnode);
}

/**
 * \fn	CFGNode* CFG::getCFGNodeByStmtNumber( STMT stmtNumber )
 *
 * \brief	Gets configuration node by statement number.
 *
 * \author	Wang Zhongliang
 * \date	2013/2/6
 *
 * \param	stmtNumber	The statement number.
 *
 * \return	null if it fails, else the configuration node by statement number.
 */

CFGNode* CFG::getCFGNodeByStmtNumber( STMT stmtNumber )
{
	for(vector<CFGNode*>::iterator it = _CFGBlocks.begin(); it != _CFGBlocks.end();++it)
		if((*it)->containStatement(stmtNumber))
			return *it;
	return NULL;
}

/**
 * \fn	vector<CFGNode*> CFG::getAllCFGNodes()
 *
 * \brief	Get a copy of all CFGNodes
 *
 * \author	Wang Zhongliang
 * \date	2013/2/26
 *
 * \return	null if it fails, else all configuration nodes.
 */

vector<CFGNode*> CFG::getAllCFGNodes()
{
	return _CFGBlocks;
}
