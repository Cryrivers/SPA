/**
 * \file	ProcTable.cpp
 * \class	ProcTable
 * \brief	Store the information of all procedures and provide functions for retrieving information of procedures.
 */

#include "StdAfx.h"
#include "ProcTable.h"


/**
 * \fn	PROC_INDEX ProcTable::addProc(STRING procName, STMT start, STMT end)
 *
 * \brief	If the procedure as given by the name 'procName' is not in the procedure table, add it into the table and return its assigned index in the table. Otherwise, return its index.
 * 
 *
 * \param	procName	The name of the procedure that is being added.
 *
 * \return	PROC_INDEX.
 */
PROC_INDEX ProcTable::addProc(STRING procName, STMT start, STMT end)
{
	int index = getProcIndex(procName);

	if (index == -1) { // procedure not yet in the table
		procedureNames.push_back(procName);
		startStmts.push_back(start);
		endStmts.push_back(end);
		//size++;
		//return size-1;
		return(procedureNames.size() - 1);
	}else {
		return(index);
	}
}

/**
 * \fn	STRING ProcTable::getProcName(PROC_INDEX ind)
 *
 * \brief	Return the name of the procedure that has the given index. If no procedure has the given index, exception.
 * 
 *
 * \param	ind	The index of the procedure.
 *
 * \return	STRING.
 */
STRING ProcTable::getProcName(PROC_INDEX ind)
{
	if ((ind < 0) || (ind >= procedureNames.size())) {
		throw exception("Given index out of range!");
	}else {
		return(procedureNames.at(ind));
	}
}

/**
 * \fn	STMT ProcTable::getProcStart(PROC_INDEX ind)
 *
 * \brief	Return the start statement of the procedure that has the given index. If no procedure has the given index, exception.
 * 
 *
 * \param	ind	The index of the procedure.
 *
 * \return	STMT.
 */
STMT ProcTable::getProcStart(PROC_INDEX ind)
{
	if ((ind < 0) || (ind >= startStmts.size())) {
		throw exception("Given index out of range!");
	}else {
		return(startStmts.at(ind));
	}
}

/**
 * \fn	STMT ProcTable::getProcEnd(PROC_INDEX ind)
 *
 * \brief	Return the end statement of the procedure that has the given index. If no procedure has the given index, exception.
 * 
 *
 * \param	ind	The index of the procedure.
 *
 * \return	STMT.
 */
STMT ProcTable::getProcEnd(PROC_INDEX ind)
{
	if ((ind < 0) || (ind >= endStmts.size())) {
		throw exception("Given index out of range!");
	}else {
		return(endStmts.at(ind));
	}
}

/**
 * \fn	STRING_LIST ProcTable::getAllProcName(PROC_INDEX_LIST inds)
 *
 * \brief	For each index from the given list of procedure indices, add the corresponding procedure name into a result list. If any index is invalid, throw an exception.
 * 
 *
 * \param	inds	A list of procedure indices.
 *
 * \return	The produced result list.
 */
STRING_LIST ProcTable::getAllProcName(PROC_INDEX_LIST inds)
{
	vector<string> resultLst;
	for (int i = 0; i < inds.size(); i++) {
		int ind = inds.at(i);
		if ((ind < 0) || (ind >= procedures.size())) {
			throw exception("Given index out of range!");
		}else {
			resultLst.push_back(procedures.at(ind));
		}
	}
	return(resultLst);
}

/**
 * \fn	STRING_LIST ProcTable::getAllProcName()
 *
 * \brief	Return the names of all procedures.
 * 
 *
 *
 * \return	STRING_LIST.
 */
STRING_LIST ProcTable::getAllProcName()
{
	return(procedureNames);
}

/**
 * \fn	PROC_INDEX ProcTable::getProcIndex(STRING procName)
 *
 * \brief	Return the index of the procedure that has the given name. If no procedure has the given name, return -1.
 * 
 *
 * \param	procName	The name of the procedure.
 *
 * \return	PROC_INDEX.
 */
PROC_INDEX ProcTable::getProcIndex(STRING procName)
{
	int size = procedureNames.size();

	for (int i = 0; i < size; i++) {
		if (procedureNames.at(i).compare(procName) == 0) {
			return(i);
		}
	}
	return(-1); // procedure not found, return -1
}

int ProcTable::size(){
	return procedureNames.size();
}
