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

CFGNode* CFG::getFollowingCFGNodeByCurrentStmtNumber( STMT stmtNumber )
{
	for(vector<CFGNode*>::iterator it = _CFGBlocks.begin(); it != _CFGBlocks.end();++it)
	{	
		if((*it)->containStatement(stmtNumber))
		{
			if(it != _CFGBlocks.end() - 1)
				return *(++it);
			else
				return NULL;
		}
	}
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
	FILE* f = fopen("../current-cfg.dot","w");
	fprintf(f,"digraph graphname {\n");

	for(vector<CFGNode*>::iterator it = _CFGBlocks.begin(); it != _CFGBlocks.end();++it)
	{
		CFGNode* c = *it;
		vector<CFGNode*> edges = c->getNextEdges();
		string label = "";
		//Print the label
		if(c->getCFGType() != CFG_DUMMY)
		{
			for(int i = c->getStartStatement(); i<= c->getEndStatement(); i++)
			{
				label += to_string((long long)i);
				if(i<c->getEndStatement()) label += ",";
			}
			fprintf(f,"%d [label=\"%s\"]\n",__indexOf(_CFGBlocks,*it), label.c_str());
		}
		else
		{
			fprintf(f,"%d [label=\"Dummy %d\"]\n",__indexOf(_CFGBlocks,*it), c->getProcIndex());
		}

		//Connect edges
		for(vector<CFGNode*>::iterator n = edges.begin(); n!= edges.end(); ++n)
		{
			fprintf(f,"%d -> %d\n", __indexOf(_CFGBlocks,*it), __indexOf(_CFGBlocks,*n));
		}
	}
	fprintf(f,"}");
	fclose(f);
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

CFGNode* CFG::getBipDummyNodeByProcIndex( PROC_INDEX index )
{
	for(vector<CFGNode*>::iterator it = _CFGBlocks.begin(); it != _CFGBlocks.end();++it)
	{	
		if((*it)->getCFGType() == CFG_DUMMY && (*it)->getProcIndex() == index)
			return (*it);
	}
	return NULL;
}

vector<CFGNode*> CFG::getNextCFGNodeByCurrentStatement(statement s, bool bipEnabled )
{
	CFGNode* nextNode;
	vector<CFGNode*> finalResult;

	STMT_LIST first;
	first.push_back(s.stmtNumber);
	STMT_LIST result;

	PKBController::createInstance()->next(&first, &result, 1);
	if(result.empty())
	{
		if(bipEnabled)
		finalResult.push_back(this->getBipDummyNodeByProcIndex(s.procIndex));
	}
	else
	{
		for(vector<int>::iterator it =  result.begin(); it != result.end(); ++it)
		{
			bool duplicated = false;
			CFGNode* node = this->getCFGNodeByStmtNumber(*it);
			
			for(vector<CFGNode*>::iterator it2 = finalResult.begin(); it2 != finalResult.end(); ++it2)
				if(node == *it2) duplicated = true;

			if(!duplicated) finalResult.push_back(node);
		}
	}

	return finalResult;
}
