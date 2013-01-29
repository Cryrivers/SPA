#include "stdafx.h"
#include "CallTable.h"

BOOLEAN CallTable::isCalls(STMT stmt, PROC_INDEX proc)
{
	int size = stmts.size();

	for (int i = 0; i < size; i++) {
		if ((stmts.at(i) == stmt) && (procs.at(i) == proc)) {
			return(true);
		}
	}
	return(false);
}


STMT_LIST CallTable::getCallsFirst(PROC_INDEX proc)
{
	vector<int> resultLst;

	int size = procs.size();
	for (int i = 0; i < size; i++) {
		if (procs.at(i) == proc) {
			// one match found, add in the result list
			resultLst.push_back(stmts.at(i));
		}
	}
	return(resultLst);
}


PROC_INDEX CallTable::getCallsSecond(STMT stmt)
{
	int size = stmts.size();
	for (int i = 0; i < size; i++) {
		if (stmts.at(i) == stmt) {
			// match found, return
			return (procs.at(i));
		}
	}
	return(-1);
}


void CallTable::addCall(STMT stmt, PROC_INDEX proc)
{
	if (isCalls(stmt, proc)) {
		// CallTable(stmt, proc) already in the table, do nothing
	}else {
		// CallTable(stmt, proc) not in the table, add in
		stmts.push_back(stmt);
		procs.push_back(proc);
	}
}


BOOLEAN CallTable::calls(STMT_LIST* sts_ptr, PROC_INDEX_LIST* ps_ptr, int arg)
{
	switch (arg) {
	case 0:
		return(calls_00(sts_ptr, ps_ptr));

	//case 1:
	//	return(modifies_01(sts_ptr, vs_ptr));

	//case 2:
	//	return(modifies_10(sts_ptr, vs_ptr));

	//case 3:
	//	return(modifies_11(sts_ptr, vs_ptr));

	default:
		throw "arg value invalid";
	}
}


BOOLEAN CallTable::calls_00(STMT_LIST* sts_p, PROC_INDEX_LIST* ps_p)
{
	int size1 = sts_p->size();
	int size2 = ps_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 1a
		if (stmts.size() == 0) {
			return(false);
		}else {
			return(true);
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 1b
		for (int i = 0; i < procs.size(); i++) {
			for (int j = 0; j < size2; j++) {
				if (procs.at(i) == ps_p->at(j)) {
					return(true);
				}
			}
		}
		return(false);
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 1c
		for (int i = 0; i < stmts.size(); i++) {
			for (int j = 0; j < size1; j++) {
				if (stmts.at(i) == sts_p->at(j)) {
					return(true);
				}
			}
		}
		return(false);
	}else {  //size1!=0 && size2!=0, case 1d
		for (int i = 0; i < size1; i++) {
			for (int j = 0; j < size2; j++) {
				if (isCalls(sts_p->at(i), ps_p->at(j))) {
					return(true);
				}
			}
		}
		return(false);
	}
}

/*
BOOLEAN CallTable::modifies_01(STMT_LIST *sts_p, VAR_INDEX_LIST *vs_p)
{
	int size1 = sts_p->size();
	int size2 = vs_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 2a
		for (int i = 0; i < vars.size(); i++) {
			int current = vars.at(i);
			if (indexOf(*vs_p, current) == -1) {
				vs_p->push_back(current);
			}
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 2c
		int index = 0;
		for (int i = 0; i < size2; i++) {
			if (indexOf(vars, vs_p->at(index)) == -1) {
				vs_p->erase(vs_p->begin() + index);
				// index remain the same
			}else {
				// do not remove the element
				index++;
			}
		}
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 2b
		for (int i = 0; i < size1; i++) {
			vector<int> val = getModifiesSecond(sts_p->at(i));
			for (int j = 0; j < val.size(); j++) {
				int current = val.at(j);
				if (indexOf(*vs_p, current) == -1) {
					vs_p->push_back(current);
				}
			}
		}
	}else {  //size1!=0 && size2!=0, case 2d
	*/

		 /*
		  * VectorA (a1, a2, бн, am) VectorB (b1, b2, бн, bn)
		  * for bi from b1 to bn {
		  * nomatch = true
		  * for aj from a1 to am {
		  * if isRelation(aj, bi) is true
		  *      nomatch = false
		  *      break
		  * }
		  * if (nomatch)
		  * remove bi
		  * }
		  */

/*
		int index = 0;
		for (int i = 0; i < size2; i++) {
			BOOLEAN noMatch = true;
			for (int j = 0; j < size1; j++) {
				if (isModifies(sts_p->at(j), vs_p->at(index))) {
					noMatch = false;
					break;
				}
			}
			if (noMatch) {
				vs_p->erase(vs_p->begin() + index);
				// index remain the same
			}else {
				// do not remove the element
				index++;
			}
		}
	}

	if (vs_p->size() > 0) {
		return(true);
	}else {
		return(false);
	}
}
*/
/*
BOOLEAN CallTable::modifies_10(STMT_LIST *sts_p, VAR_INDEX_LIST *vs_p)
{
	int size1 = sts_p->size();
	int size2 = vs_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 3a
		for (int i = 0; i < stmts.size(); i++) {
			int current = stmts.at(i);
			if (indexOf(*sts_p, current) == -1) {
				sts_p->push_back(current);
			}
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 3b
		for (int i = 0; i < size2; i++) {
			vector<int> val = getModifiesFirst(vs_p->at(i));
			for (int j = 0; j < val.size(); j++) {
				int current = val.at(j);
				if (indexOf(*sts_p, current) == -1) {
					sts_p->push_back(current);
				}
			}
		}
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 3c
		int index = 0;
		for (int i = 0; i < size1; i++) {
			if (indexOf(stmts, sts_p->at(index)) == -1) {
				sts_p->erase(sts_p->begin() + index);
				// index remain the same
			}else {
				// do not remove the element
				index++;
			}
		}
	}else {  //size1!=0 && size2!=0, case 3d
	*/
		 /*
		  * VectorA (a1, a2, бн, am) VectorB (b1, b2, бн, bn)
		  * for ai from a1 to am {
		  * nomatch = true
		  * for bj from b1 to bn {
		  * if isRelation(ai, bj) is true
		  *      nomatch = false
		  *      break
		  * }
		  * if (nomatch)
		  * remove ai
		  * }
		  */
/*
		int index = 0;
		for (int i = 0; i < size1; i++) {
			BOOLEAN noMatch = true;
			for (int j = 0; j < size2; j++) {
				if (isModifies(sts_p->at(index), vs_p->at(j))) {
					noMatch = false;
					break;
				}
			}
			if (noMatch) {
				sts_p->erase(sts_p->begin() + index);
				// index remain the same
			}else {
				// do not remove the element
				index++;
			}
		}
	}

	if (sts_p->size() > 0) {
		return(true);
	}else {
		return(false);
	}
}
*/
/*

BOOLEAN CallTable::modifies_11(STMT_LIST *sts_p, VAR_INDEX_LIST *vs_p)
{
	int size1 = sts_p->size();
	int size2 = vs_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 4a
		//throw "arg is 11, but both vectors are empty.";
		for (int i = 0; i < stmts.size(); i++) {
			sts_p->push_back(stmts.at(i));
			vs_p->push_back(vars.at(i));
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 4b
		vector<int> vs_copy;
		for (int i = 0; i < size2; i++) {
			vs_copy.push_back(vs_p->at(i));
		}
		vs_p->clear();

		for (int i = 0; i < size2; i++) {
			int currentEleVs = vs_copy.at(i);
			vector<int> temp = getModifiesFirst(currentEleVs);
			int tempSize = temp.size();
			if (tempSize == 0) {
				//do not fill in vector
			}else {
				for (int j = 0; j < tempSize; j++) {
					sts_p->push_back(temp.at(j));
					vs_p->push_back(currentEleVs);
				}
			}
		}
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 4c
		vector<int> sts_copy;
		for (int i = 0; i < size1; i++) {
			sts_copy.push_back(sts_p->at(i));
		}
		sts_p->clear();

		for (int i = 0; i < size1; i++) {
			int currentEleSts = sts_copy.at(i);
			vector<int> temp = getModifiesSecond(currentEleSts);
			int tempSize = temp.size();
			if (tempSize == 0) {
				//do not fill in vector
			}else {
				for (int j = 0; j < tempSize; j++) {
					sts_p->push_back(currentEleSts);
					vs_p->push_back(temp.at(j));
				}
			}
		}
	}else {           //size1!=0 && size2!=0, case 4d
		if (size1 == size2) { //case 4d
			int index = 0;
			for (int i = 0; i < size1; i++) {
				if (isModifies(sts_p->at(index), vs_p->at(index))) {
					index++;
				}else {
					sts_p->erase(sts_p->begin() + index);
					vs_p->erase(vs_p->begin() + index);
				}
			}
		}else {  //exception
			throw "arg is 11, but sizes of both vectors are not the same.";
		}
	}
	if (sts_p->size() > 0) {
		return(true);
	}else {
		return(false);
	}
}
*/

STMT_LIST CallTable::getAllCallsFirst()
{
	return(stmts);
}


PROC_INDEX_LIST CallTable::getAllCallsSecond()
{
	return(procs);
}
