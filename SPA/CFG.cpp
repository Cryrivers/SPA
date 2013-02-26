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

void CFG::__printDotGraphForGraphviz()
{
	printf("digraph graphname {\n");

	for(vector<CFGNode*>::iterator it = _CFGBlocks.begin(); it != _CFGBlocks.end();++it)
	{
		CFGNode* c = *it;
		vector<CFGNode*> edges = c->getNextEdges();
		string label = "";
		//Print the label
		for(int i = c->getStartStatement(); i<= c->getEndStatement(); i++)
		{
			label += to_string((long long)i);
			if(i<c->getEndStatement()) label += ",";
		}
		printf("%d [label=\"%s\"]\n",__indexOf(_CFGBlocks,*it), label.c_str());

		//Connect edges
		for(vector<CFGNode*>::iterator n = edges.begin(); n!= edges.end(); ++n)
		{
			printf("%d -> %d\n", __indexOf(_CFGBlocks,*it), __indexOf(_CFGBlocks,*n));
		}
	}
	printf("}");
}

int CFG::__indexOf(vector<CFGNode*> list, CFGNode* val)
{
	for (int i = 0; i < list.size(); i++) {
		if (list.at(i) == val) {
			return(i);
		}
	}
	return(-1);
}
