/**
 * \file	VarTable.cpp
 * \class	VarTable
 * \brief	Store the information of all variables and provide funtions for retrieving information of variables.
 */

#include "stdafx.h"
#include "VarTable.h"


/**
 * \fn	VAR_INDEX VarTable::addVar(STRING varName)
 *
 * \brief	If the variable as given by the name 'varName' is not in the variable table, add it into the table and return its assigned index in the table. Otherwise, return its index.
 * 
 *
 * \param	varName	The name of the variable that is being added.
 *
 * \return	VAR_INDEX.
 */
VAR_INDEX VarTable::addVar(STRING varName)
{
	int index = getVarIndex(varName);

	if (index == -1) { // variable not yet in the table
		variables.push_back(varName);
		//size++;
		//return size-1;
		return(variables.size() - 1);
	}else {
		return(index);
	}
}

/**
 * \fn	STRING VarTable::getVarName(VAR_INDEX ind)
 *
 * \brief	Return the name of the variable that has the given index. If no variables has the given index, exception.
 * 
 *
 * \param	ind	The index of the variable.
 *
 * \return	STRING.
 */
STRING VarTable::getVarName(VAR_INDEX ind)
{
	if ((ind < 0) || (ind >= variables.size())) {
		throw exception("Given index out of range!");
	}else {
		return(variables.at(ind));
	}
}

/**
 * \fn	STRING_LIST VarTable::getAllVarName(VAR_INDEX_LIST inds)
 *
 * \brief	For each index from the given list of variable indices, add the corresponding variable name into a result list. If any index is invalid, throw an exception.
 * 
 *
 * \param	inds	A list of variable indices.
 *
 * \return	The produced result list.
 */
STRING_LIST VarTable::getAllVarName(VAR_INDEX_LIST inds)
{
	vector<string> resultLst;
	for (int i = 0; i < inds.size(); i++) {
		int ind = inds.at(i);
		if ((ind < 0) || (ind >= variables.size())) {
			throw exception("Given index out of range!");
		}else {
			resultLst.push_back(variables.at(ind));
		}
	}
	return(resultLst);
}

/**
 * \fn	STRING_LIST VarTable::getAllVarName()
 *
 * \brief	Return the names of all variables.
 * 
 *
 *
 * \return	STRING_LIST.
 */
STRING_LIST VarTable::getAllVarName()
{
	return(variables);
}

/**
 * \fn	VAR_INDEX VarTable::getVarIndex(STRING varName)
 *
 * \brief	Return the index of the variable that has the given name. If no variable has the given name, return -1.
 * 
 *
 * \param	varName	The name of the variable.
 *
 * \return	VAR_INDEX.
 */
VAR_INDEX VarTable::getVarIndex(STRING varName)
{
	int size = variables.size();

	for (int i = 0; i < size; i++) {
		if (variables.at(i).compare(varName) == 0) {
			return(i);
		}
	}
	return(-1); // variable not found, return -1
}
