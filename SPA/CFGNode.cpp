/**
 * \file	CFGNode.cpp
 *
 * \brief	Implements the configuration node class.
 */

#include "StdAfx.h"
#include "CFGNode.h"
#include <assert.h>

CFGNode::CFGNode(void)
{
	//TODO: Fix Coding standards later
	_start = -1;
	_end = -1;
	_type = CFG_NORMAL_BLOCK;
}


CFGNode::~CFGNode(void)
{
}

/**
 * \fn	void CFGNode::setStartStatement( STMT startStatement )
 *
 * \brief	Sets start statement.
 *
 * \author	Wang Zhongliang
 * \date	2013/2/5
 *
 * \param	startStatement	The start statement.
 */

void CFGNode::setStartStatement( STMT startStatement )
{
	assert(startStatement < 50);
	_start = startStatement;
}

/**
 * \fn	void CFGNode::setEndStatement( STMT endStatement )
 *
 * \brief	Sets end statement.
 *
 * \author	Wang Zhongliang
 * \date	2013/2/5
 *
 * \param	endStatement	The end statement.
 */

void CFGNode::setEndStatement( STMT endStatement )
{
	_end = endStatement;
}

/**
 * \fn	STMT CFGNode::getStartStatement()
 *
 * \brief	Gets start statement.
 *
 * \author	Wang Zhongliang
 * \date	2013/2/5
 *
 * \return	The start statement.
 */

STMT CFGNode::getStartStatement()
{
	return _start;
}

/**
 * \fn	STMT CFGNode::getEndStatement()
 *
 * \brief	Gets End statement.
 *
 * \author	Wang Zhongliang
 * \date	2013/2/5
 *
 * \return	The end statement.
 */

STMT CFGNode::getEndStatement()
{
	return _end;
}

/**
 * \fn	void CFGNode::addEdge( CFGNode* nextPath )
 *
 * \brief	Adds an edge.
 * \deprecated 
 * \see connectTo
 * \see connectBackTo			   
 * \author	Wang Zhongliang
 * \date	2013/2/5
 *
 * \param [in]	nextPath	If non-null, the edge connecting to next CFGNode.
 */

void CFGNode::addEdge( CFGNode* nextPath )
{
	//In case of duplicates
	for (vector<CFGNode*>::iterator it = _edges.begin(); it != _edges.end(); ++it)
		if(*it == nextPath) return;

	_edges.push_back(nextPath);
}

/**
 * \fn	int CFGNode::getEdgeSize()
 *
 * \brief	Gets the number of next possible paths (edges).
 *
 * \author	Wang Zhongliang
 * \date	2013/2/5
 *
 * \return	Number of next possible paths.
 */

int CFGNode::getEdgeSize()
{
	return _edges.size();
}

/**
 * \fn	bool CFGNode::containStatement( STMT stmtNumber )
 *
 * \brief	Query if a statement is in this block of CFGNode.
 *
 * \author	Wang Zhongliang
 * \date	2013/2/5
 *
 * \param	stmtNumber	The Statement Number.
 *
 * \return	true if it is in this block of CFGNode, false otherwise.
 */

bool CFGNode::containStatement( STMT stmtNumber )
{
	assert(_start <= _end);
	return (stmtNumber >= _start && stmtNumber <= _end);
}

/**
 * \fn	CFGType CFGNode::getCFGType()
 *
 * \brief	Gets configuration type.
 *
 * \author	Wang Zhongliang
 * \date	2013/2/5
 *
 * \return	The configuration type.
 */

CFGType CFGNode::getCFGType()
{
	return _type;
}

/**
 * \fn	void CFGNode::setCFGType( CFGType type )
 *
 * \brief	Sets configuration type.
 *
 * \author	Wang Zhongliang
 * \date	2013/2/5
 *
 * \param	type	The type.
 */

void CFGNode::setCFGType( CFGType type )
{
	_type = type;
}

/**
 * \fn	void CFGNode::popLastEdge()
 *
 * \brief	Pops the last edge.
 *
 * \author	Wang Zhongliang
 * \date	2013/2/6
 */

void CFGNode::popLastEdge()
{
	assert(_edges.size() > 0);
	_edges.pop_back();
}

/**
 * \fn	void CFGNode::connectTo( CFGNode* nextPath )
 *
 * \brief	Connects to.
 *
 * \author	Wang Zhongliang
 * \date	2013/2/19
 *
 * \param [in,out]	nextPath	If non-null, full pathname of the next file.
 */

void CFGNode::connectTo( CFGNode* nextPath )
{
	this->addEdge(nextPath);
}

/**
 * \fn	void CFGNode::connectBackTo( CFGNode* prevPath )
 *
 * \brief	Connects a back to.
 *
 * \author	Wang Zhongliang
 * \date	2013/2/19
 *
 * \param [in,out]	prevPath	If non-null, full pathname of the previous file.
 */

void CFGNode::connectBackTo( CFGNode* prevPath )
{
	prevPath->connectTo(this);
}
