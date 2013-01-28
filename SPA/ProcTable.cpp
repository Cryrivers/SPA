#include "StdAfx.h"
#include "ProcTable.h"



VAR_INDEX ProcTable::addProc(STRING procName)
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


STRING ProcTable::getProcName(VAR_INDEX ind)
{
	if ((ind < 0) || (ind >= procedures.size())) {
		throw exception("Given index out of range!");
	}else {
		return(procedures.at(ind));
	}
}


STRING_LIST ProcTable::getAllProcName(VAR_INDEX_LIST inds)
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


STRING_LIST ProcTable::getAllProcName()
{
	return(procedures);
}


VAR_INDEX ProcTable::getProcIndex(STRING procName)
{
	int size = procedures.size();

	for (int i = 0; i < size; i++) {
		if (procedures.at(i).compare(procName) == 0) {
			return(i);
		}
	}
	return(-1); // prociable not found, return -1
}
