/**
 * \file	ProcTable.cpp
 * \class	ProcTable
 * \brief	Store the information of all procedures and provide funtions for retrieving information of procedures.
 */

#include "StdAfx.h"
#include "ProcTable.h"


/**
 * \fn	PROC_INDEX ProcTable::addProc(STRING procName)
 *
 * \brief	If the procedure as given by the name 'procName' is not in the procedure table, add it into the table and return its assigned index in the table. Otherwise, return its index.
 * 
 *
 * \param	procName	The name of the procedure that is being added.
 *
 * \return	PROC_INDEX.
 */
PROC_INDEX ProcTable::addProc(STRING procName)
{
	int index = getProcIndex(procName);

	if (index == -1) { // prociable not yet in the table
		procedures.push_back(procName);
		//size++;
		//return size-1;
		return(procedures.size() - 1);
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
	if ((ind < 0) || (ind >= procedures.size())) {
		throw exception("Given index out of range!");
	}else {
		return(procedures.at(ind));
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
	return(procedures);
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
	int size = procedures.size();

	for (int i = 0; i < size; i++) {
		if (procedures.at(i).compare(procName) == 0) {
			return(i);
		}
	}
	return(-1); // prociable not found, return -1
}

int ProcTable::size(){
	return procedures.size();
}
