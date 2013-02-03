/**
 * \file	ModifiesP.cpp
 * \class	ModifiesP
 * \brief	Store all 'Modifies' relations for procedures (call them 'ModifiesP' relation) in the form (proc, var) where 'proc' is a procedure index and 'var' is a variable index. Provide funtions for query on Modifies relation for procedures (i.e. ModifiesP relation). The storage is referred to as ModifiesPTable.
 */

#include "stdafx.h"
#include "ModifiesP.h"

BOOLEAN ModifiesP::isModifiesP(PROC_INDEX proc, VAR_INDEX var)
{
	int size = procs.size();

	for (int i = 0; i < size; i++) {
		if ((procs.at(i) == proc) && (vars.at(i) == var)) {
			return(true);
		}
	}
	return(false);
}


PROC_INDEX_LIST ModifiesP::getModifiesPFirst(VAR_INDEX var)
{
	vector<int> resultLst;

	int size = vars.size();
	for (int i = 0; i < size; i++) {
		if (vars.at(i) == var) {
			// one match found, add in the result list
			resultLst.push_back(procs.at(i));
		}
	}
	return(resultLst);
}


VAR_INDEX_LIST ModifiesP::getModifiesPSecond(PROC_INDEX proc)
{
	vector<int> resultLst;

	int size = procs.size();
	for (int i = 0; i < size; i++) {
		if (procs.at(i) == proc) {
			// one match found, add in the result list
			resultLst.push_back(vars.at(i));
		}
	}
	return(resultLst);
}

/**
 * \fn	void ModifiesP::addModifiesP(PROC_INDEX proc, VAR_INDEX var)
 *
 * \brief	Add a new 'ModifiesP' relation in the form (proc, var) to ModifiesTable where 'proc' is a procedure index and 'var' is a variable index. If the relation already exists, do nothing.
 * 
 *
 * \param	proc	The index of the procedure in this ModifiesP relation.
 * \param	var	    The index of the variable that is being modified in the variable table.
 *
 * \return	void.
 */
void ModifiesP::addModifiesP(PROC_INDEX proc, VAR_INDEX var)
{
	if (isModifiesP(proc, var)) {
		// ModifiesP(proc1, proc2) already in the table, do nothing
	}else {
		// ModifiesP(proc1, proc2) not in the table, add in
		procs.push_back(proc);
		vars.push_back(var);
	}
}

/**
 * \fn	BOOLEAN ModifiesP::modifiesP(PROC_INDEX_LIST *ps_ptr, VAR_INDEX_LIST *vs_ptr, int arg)
 *
 * \brief	A function for query on 'ModifiesP' relations.
 * 
 * To handle a query in the form ModifiesP(List A, List B, arg).

 * Case 1: arg = 00 = 0
 *	1a) Both lists are empty
 *	Return TRUE if ModifiesPTable is not empty, FASLE otherwise.
 *	1b) List A empty, List B non-empty
 *	Return TRUE if there exists one entry (proc, var) in the ModifiesPTable where var equals to at least one of the element in List B, FASLE otherwise.
 *	1c) List A non-empty, List B empty
 *	Return TRUE if there exists one entry (proc, var) in the ModifiesPTable where proc equals to at least one of the element in List A, FASLE otherwise.
 *	1d) Both lists are non-empty
 *	Let ai be the i-th element of List A and bj be the j-th element of List B. Return TRUE if there exist at least one <ai, bj> pair such that (ai, bj) is an entry in ModifiesPTable, FASLE otherwise.

 * Case 2: arg = 01 = 1
 *  2a) Both lists are empty
 *	Fill up list B with all the distinct values in the second column of ModifiesPTable.
 *	2b) List A nont-empty, List B empty
 *	Fill up list B with all the values in the second column of ModifiesPTable where the corresponding first column equals to the elements in list A.
 *	2c) List A empty, List B non-empty
 *	Remove elements from list B that are not found in the second column of ModifiesPTable.
 *	2d) Both lists are non-empty
 *	Find all values (say V2) in the second column of ModifiesPTable where the corresponding first column value equals to the elements in list A and remove elements from list B that are not in V2.
 *	For case 2a, 2b, 2c and 2d: Return TRUE if at the end, list B is not empty, FASLE otherwise.

 * Case 3: arg = 10 = 2
 *  3a) Both lists are empty
 *	Fill up list A with all the distinct values in the first column of ModifiesPTable.
 *	3b) List A empty, List B non-empty
 *	Fill up list A with all the values in the first column of ModifiesPTable where the corresponding second column equals to the elements in list B.
 *	3c) List A non-empty, List B empty
 *	Remove elements from list A that are not found in the first column of ModifiesPTable.
 *	3d) Both lists are non-empty
 *	Find all values (say V1) in the first column of ModifiesPTable where the corresponding second column value equals to the elements in list B and remove elements from list A that are not in V1.
 *	For case 3a, 3b, 3c and 3d: Return TRUE if at the end, list A is not empty, FASLE otherwise.

 * Case 4: arg = 11 = 3
 *  4a) Both lists are empty
 *	Fill up the two lists with all entries of ModifiesPTable.
 *	4b) List A empty, List B non-empty
 *	For each element (bi) in list B, find values in the first column of ModifiesPTable where the second column equals to bi.
	(i) If found no values, remove bi from list B.
	(ii) If found 1 single value, add this value to list A.
	(iii) if found n (n>1) values, add these values to list A and duplicate bi n-1 times in list B.
 *	4c) List A non-empty, List B empty
 *	For each element (ai) in list A, find values in the second column of ModifiesPTable where the first column equals to ai.
	(i) If found no values, remove ai from list A.
	(ii) If found 1 single value, add this value to list B.
	(iii) if found n (n>1) values, add these values to list B and duplicate ai n-1 times in list A.
 *	4d) Both lists are non-empty
 *	Sizes of the two lists must be the same, otherwise exception. Say the sizes are both n. 
 *	Let ai be the i-th element of list A and b-i be the i-th element of list B. For i from 1 to n, remove ai, bi from list A, list B if (ai, bi) is not an entry in ModifiesPTable.
 *	For case 4a, 4b, 4c and 4d: At the end, size of list A = size of list B. Return TRUE if at the end, list A is not empty, FASLE otherwise.
 
 * Case: arg = other values
 *	Exception

 * \param	ps_ptr	A pointer to a procedure index list.
 * \param	vs_ptr	A pointer to a variable index list.
 *
 * \return	A Boolean value as specified in the detailed description.
 */
BOOLEAN ModifiesP::modifiesP(PROC_INDEX_LIST *ps_ptr, VAR_INDEX_LIST *vs_ptr, int arg)
{
	switch (arg) {
	case 0:
		return(modifiesP_00(ps_ptr, vs_ptr));

	case 1:
		return(modifiesP_01(ps_ptr, vs_ptr));

	case 2:
		return(modifiesP_10(ps_ptr, vs_ptr));

	case 3:
		return(modifiesP_11(ps_ptr, vs_ptr));

	default:
		throw "arg value invalid";
	}
}


BOOLEAN ModifiesP::modifiesP_00(PROC_INDEX_LIST *ps_p, VAR_INDEX_LIST *vs_p)
{
	int size1 = ps_p->size();
	int size2 = vs_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 1a
		if (procs.size() == 0) {
			return(false);
		}else {
			return(true);
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 1b
		for (int i = 0; i < vars.size(); i++) {
			for (int j = 0; j < size2; j++) {
				if (vars.at(i) == vs_p->at(j)) {
					return(true);
				}
			}
		}
		return(false);
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 1c
		for (int i = 0; i < procs.size(); i++) {
			for (int j = 0; j < size1; j++) {
				if (procs.at(i) == ps_p->at(j)) {
					return(true);
				}
			}
		}
		return(false);
	}else {  //size1!=0 && size2!=0, case 1d
		for (int i = 0; i < size1; i++) {
			for (int j = 0; j < size2; j++) {
				if (isModifiesP(ps_p->at(i), vs_p->at(j))) {
					return(true);
				}
			}
		}
		return(false);
	}
}


BOOLEAN ModifiesP::modifiesP_01(PROC_INDEX_LIST *ps_p, VAR_INDEX_LIST *vs_p)
{
	int size1 = ps_p->size();
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
			vector<int> val = getModifiesPSecond(ps_p->at(i));
			for (int j = 0; j < val.size(); j++) {
				int current = val.at(j);
				if (indexOf(*vs_p, current) == -1) {
					vs_p->push_back(current);
				}
			}
		}
	}else {  //size1!=0 && size2!=0, case 2d
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
		int index = 0;
		for (int i = 0; i < size2; i++) {
			BOOLEAN noMatch = true;
			for (int j = 0; j < size1; j++) {
				if (isModifiesP(ps_p->at(j), vs_p->at(index))) {
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


BOOLEAN ModifiesP::modifiesP_10(PROC_INDEX_LIST *ps_p, VAR_INDEX_LIST *vs_p)
{
	int size1 = ps_p->size();
	int size2 = vs_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 3a
		for (int i = 0; i < procs.size(); i++) {
			int current = procs.at(i);
			if (indexOf(*ps_p, current) == -1) {
				ps_p->push_back(current);
			}
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 3b
		for (int i = 0; i < size2; i++) {
			vector<int> val = getModifiesPFirst(vs_p->at(i));
			for (int j = 0; j < val.size(); j++) {
				int current = val.at(j);
				if (indexOf(*ps_p, current) == -1) {
					ps_p->push_back(current);
				}
			}
		}
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 3c
		int index = 0;
		for (int i = 0; i < size1; i++) {
			if (indexOf(procs, ps_p->at(index)) == -1) {
				ps_p->erase(ps_p->begin() + index);
				// index remain the same
			}else {
				// do not remove the element
				index++;
			}
		}
	}else {  //size1!=0 && size2!=0, case 3d
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
		int index = 0;
		for (int i = 0; i < size1; i++) {
			BOOLEAN noMatch = true;
			for (int j = 0; j < size2; j++) {
				if (isModifiesP(ps_p->at(index), vs_p->at(j))) {
					noMatch = false;
					break;
				}
			}
			if (noMatch) {
				ps_p->erase(ps_p->begin() + index);
				// index remain the same
			}else {
				// do not remove the element
				index++;
			}
		}
	}

	if (ps_p->size() > 0) {
		return(true);
	}else {
		return(false);
	}
}


BOOLEAN ModifiesP::modifiesP_11(PROC_INDEX_LIST *ps_p, VAR_INDEX_LIST *vs_p)
{
	int size1 = ps_p->size();
	int size2 = vs_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 4a
		//throw "arg is 11, but both vectors are empty.";
		for (int i = 0; i < procs.size(); i++) {
			ps_p->push_back(procs.at(i));
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
			vector<int> temp = getModifiesPFirst(currentEleVs);
			int tempSize = temp.size();
			if (tempSize == 0) {
				//do not fill in vector
			}else {
				for (int j = 0; j < tempSize; j++) {
					ps_p->push_back(temp.at(j));
					vs_p->push_back(currentEleVs);
				}
			}
		}
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 4c
		vector<int> sts_copy;
		for (int i = 0; i < size1; i++) {
			sts_copy.push_back(ps_p->at(i));
		}
		ps_p->clear();

		for (int i = 0; i < size1; i++) {
			int currentEleSts = sts_copy.at(i);
			vector<int> temp = getModifiesPSecond(currentEleSts);
			int tempSize = temp.size();
			if (tempSize == 0) {
				//do not fill in vector
			}else {
				for (int j = 0; j < tempSize; j++) {
					ps_p->push_back(currentEleSts);
					vs_p->push_back(temp.at(j));
				}
			}
		}
	}else {           //size1!=0 && size2!=0, case 4d
		if (size1 == size2) { //case 4d
			int index = 0;
			for (int i = 0; i < size1; i++) {
				if (isModifiesP(ps_p->at(index), vs_p->at(index))) {
					index++;
				}else {
					ps_p->erase(ps_p->begin() + index);
					vs_p->erase(vs_p->begin() + index);
				}
			}
		}else {  //exception
			throw "arg is 11, but sizes of both vectors are not the same.";
		}
	}
	if (ps_p->size() > 0) {
		return(true);
	}else {
		return(false);
	}
}

/**
 * \fn	PROC_INDEX_LIST ModifiesP::getAllModifiesPFirst()
 *
 * \brief	Return all the procedure indices stored in ModifiesPTable as a list.
 * 
 *
 * 
 * 
 *
 * \return	PROC_INDEX_LIST.
 */
PROC_INDEX_LIST ModifiesP::getAllModifiesPFirst()
{
	return(procs);
}

/**
 * \fn	VAR_INDEX_LIST ModifiesP::getAllModifiesPSecond()
 *
 * \brief	Return all the variable indices stored in ModifiesPTable as a list.
 * 
 *
 * 
 * 
 *
 * \return	VAR_INDEX_LIST.
 */
VAR_INDEX_LIST ModifiesP::getAllModifiesPSecond()
{
	return(vars);
}
