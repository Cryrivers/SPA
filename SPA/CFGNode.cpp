/**
 * \file	CFGNode.cpp
 *
 * \brief	Implements the configuration node class.
 */

#include "StdAfx.h"
#include "CFGNode.h"


CFGNode::CFGNode(void)
{

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
 *
 * \author	Wang Zhongliang
 * \date	2013/2/5
 *
 * \param [in]	nextPath	If non-null, the edge connecting to next CFGNode.
 */

void CFGNode::addEdge( CFGNode* nextPath )
{
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
	return (stmtNumber >= _start && stmtNumber <= _end);
}
