#include "stdafx.h"
#include "VarTable.h"



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


STRING VarTable::getVarName(VAR_INDEX ind)
{
	if ((ind < 0) || (ind >= variables.size())) {
		throw exception("Given index out of range!");
	}else {
		return(variables.at(ind));
	}
}


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


STRING_LIST VarTable::getAllVarName()
{
	return(variables);
}


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
