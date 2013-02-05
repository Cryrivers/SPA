/**
 * \file	CFG.cpp
 *
 * \brief	Implements the configuration class.
 */

#include "StdAfx.h"
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
	_CFGBlocks.push_back(cfgnode);
}
