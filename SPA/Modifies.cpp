/**
 * \file	Modifies.cpp
 * \class	Modifies
 * \brief	Store all 'Modifies' relations for statements (call them 'Modifies' relation) in the form (stmt, var) where 'stmt' is a statement number and 'var' is a variable index. Provide funtions for query on this relation for statements. The storage is referred to as ModifiesTable.
 */

#include "stdafx.h"
#include "Modifies.h"

BOOLEAN Modifies::isModifies(STMT stmt, VAR_INDEX var)
{
	int size = stmts.size();

	for (int i = 0; i < size; i++) {
		if ((stmts.at(i) == stmt) && (vars.at(i) == var)) {
			return(true);
		}
	}
	return(false);
}


STMT_LIST Modifies::getModifiesFirst(VAR_INDEX var)
{
	vector<int> resultLst;

	int size = vars.size();
	for (int i = 0; i < size; i++) {
		if (vars.at(i) == var) {
			// one match found, add in the result list
			resultLst.push_back(stmts.at(i));
		}
	}
	return(resultLst);
}


VAR_INDEX_LIST Modifies::getModifiesSecond(STMT stmt)
{
	vector<int> resultLst;

	int size = stmts.size();
	for (int i = 0; i < size; i++) {
		if (stmts.at(i) == stmt) {
			// one match found, add in the result list
			resultLst.push_back(vars.at(i));
		}
	}
	return(resultLst);
}

/**
 * \fn	void Modifies::addModifies(STMT stmt, VAR_INDEX var)
 *
 * \brief	Add a new 'Modifies' relation in the form (stmt, var) to ModifiesTable where 'stmt' is a statement number and 'var' is a variable index. If the relation already exists, do nothing.
 * 
 *
 * \param	stmt	The statement number of the statement in this Modifies relation.
 * \param	var	    The index of the variable that is being modified in the variable table.
 *
 * \return	void.
 */
void Modifies::addModifies(STMT stmt, VAR_INDEX var)
{
	if (isModifies(stmt, var)) {
		// Modifies(stmt1, stmt2) already in the table, do nothing
	}else {
		// Modifies(stmt1, stmt2) not in the table, add in
		stmts.push_back(stmt);
		vars.push_back(var);
	}
}

/**
 * \fn	BOOLEAN Modifies::modifies(STMT_LIST *sts_ptr, VAR_INDEX_LIST *vs_ptr, int arg)
 *
 * \brief	A function for query on 'Modifies' relations.
 * 
 * To handle a query in the form Modifies(List A, List B, arg).

 * Case 1: arg = 00 = 0
 *	1a) Both lists are empty
 *	Return TRUE if ModifiesTable is not empty, FASLE otherwise.
 *	1b) List A empty, List B non-empty
 *	Return TRUE if there exists one entry (stmt, var) in the ModifiesTable where var equals to at least one of the element in List B, FASLE otherwise.
 *	1c) List A non-empty, List B empty
 *	Return TRUE if there exists one entry (stmt, var) in the ModifiesTable where stmt equals to at least one of the element in List A, FASLE otherwise.
 *	1d) Both lists are non-empty
 *	Let ai be the i-th element of List A and bj be the j-th element of List B. Return TRUE if there exist at least one <ai, bj> pair such that (ai, bj) is an entry in ModifiesTable, FASLE otherwise.

 * Case 2: arg = 01 = 1
 *  2a) Both lists are empty
 *	Fill up list B with all the distinct values in the second column of ModifiesTable.
 *	2b) List A nont-empty, List B empty
 *	Fill up list B with all the values in the second column of ModifiesTable where the corresponding first column equals to the elements in list A.
 *	2c) List A empty, List B non-empty
 *	Remove elements from list B that are not found in the second column of ModifiesTable.
 *	2d) Both lists are non-empty
 *	Find all values (say V2) in the second column of ModifiesTable where the corresponding first column value equals to the elements in list A and remove elements from list B that are not in V2.
 *	For case 2a, 2b, 2c and 2d: Return TRUE if at the end, list B is not empty, FASLE otherwise.

 * Case 3: arg = 10 = 2
 *  3a) Both lists are empty
 *	Fill up list A with all the distinct values in the first column of ModifiesTable.
 *	3b) List A empty, List B non-empty
 *	Fill up list A with all the values in the first column of ModifiesTable where the corresponding second column equals to the elements in list B.
 *	3c) List A non-empty, List B empty
 *	Remove elements from list A that are not found in the first column of ModifiesTable.
 *	3d) Both lists are non-empty
 *	Find all values (say V1) in the first column of ModifiesTable where the corresponding second column value equals to the elements in list B and remove elements from list A that are not in V1.
 *	For case 3a, 3b, 3c and 3d: Return TRUE if at the end, list A is not empty, FASLE otherwise.

 * Case 4: arg = 11 = 3
 *  4a) Both lists are empty
 *	Fill up the two lists with all entries of ModifiesTable.
 *	4b) List A empty, List B non-empty
 *	For each element (bi) in list B, find values in the first column of ModifiesTable where the second column equals to bi.
	(i) If found no values, remove bi from list B.
	(ii) If found 1 single value, add this value to list A.
	(iii) if found n (n>1) values, add these values to list A and duplicate bi n-1 times in list B.
 *	4c) List A non-empty, List B empty
 *	For each element (ai) in list A, find values in the second column of ModifiesTable where the first column equals to ai.
	(i) If found no values, remove ai from list A.
	(ii) If found 1 single value, add this value to list B.
	(iii) if found n (n>1) values, add these values to list B and duplicate ai n-1 times in list A.
 *	4d) Both lists are non-empty
 *	Sizes of the two lists must be the same, otherwise exception. Say the sizes are both n. 
 *	Let ai be the i-th element of list A and b-i be the i-th element of list B. For i from 1 to n, remove ai, bi from list A, list B if (ai, bi) is not an entry in ModifiesTable.
 *	For case 4a, 4b, 4c and 4d: At the end, size of list A = size of list B. Return TRUE if at the end, list A is not empty, FASLE otherwise.
 
 * Case: arg = other values
 *	Exception

 * \param	sts_ptr	A pointer to a statement list, which contains a list of statement numbers.
 * \param	vs_ptr	A pointer to a variable index list.
 *
 * \return	A Boolean value as specified in the detailed description.
 */
BOOLEAN Modifies::modifies(STMT_LIST *sts_ptr, VAR_INDEX_LIST *vs_ptr, int arg)
{
	switch (arg) {
	case 0:
		return(modifies_00(sts_ptr, vs_ptr));

	case 1:
		return(modifies_01(sts_ptr, vs_ptr));

	case 2:
		return(modifies_10(sts_ptr, vs_ptr));

	case 3:
		return(modifies_11(sts_ptr, vs_ptr));

	default:
		throw "arg value invalid";
	}
}


BOOLEAN Modifies::modifies_00(STMT_LIST *sts_p, VAR_INDEX_LIST *vs_p)
{
	int size1 = sts_p->size();
	int size2 = vs_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 1a
		if (stmts.size() == 0) {
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
				if (isModifies(sts_p->at(i), vs_p->at(j))) {
					return(true);
				}
			}
		}
		return(false);
	}
}


BOOLEAN Modifies::modifies_01(STMT_LIST *sts_p, VAR_INDEX_LIST *vs_p)
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
		 /*
		  * VectorA (a1, a2, …, am) VectorB (b1, b2, …, bn)
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


BOOLEAN Modifies::modifies_10(STMT_LIST *sts_p, VAR_INDEX_LIST *vs_p)
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
		 /*
		  * VectorA (a1, a2, …, am) VectorB (b1, b2, …, bn)
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


BOOLEAN Modifies::modifies_11(STMT_LIST *sts_p, VAR_INDEX_LIST *vs_p)
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

/**
 * \fn	STMT_LIST Modifies::getAllModifiesFirst()
 *
 * \brief	Return all the statement numbers stored in ModifiesTable as a list.
 * 
 *
 * 
 * 
 *
 * \return	STMT_LIST.
 */
STMT_LIST Modifies::getAllModifiesFirst()
{
	return(stmts);
}

/**
 * \fn	VAR_INDEX_LIST Modifies::getAllModifiesSecond()
 *
 * \brief	Return all the variable indices stored in ModifiesTable as a list.
 * 
 *
 * 
 * 
 *
 * \return	VAR_INDEX_LIST.
 */
VAR_INDEX_LIST Modifies::getAllModifiesSecond()
{
	return(vars);
}


/*
 * BOOLEAN Modifies::modifies(STMT_LIST* sts_ptr, VAR_INDEX_LIST* vs_ptr) {
 *      int size1 = sts_ptr->size();
 *      int size2 = vs_ptr->size();
 *      if(size1==0 && size2==0) { //case1
 *              if(stmts.size()==0)
 *                      return false;
 *              else
 *                      return true;
 *      } else if(size1==0) { //size1==0 && size2!=0, case 2&3
 *              if(size2==1) { //case2
 *                      int firstEleVs = vs_ptr->at(0);
 *                      vector<int> temp = getModifiesFirst(firstEleVs);
 *                      int tempSize = temp.size();
 *                      if(tempSize==0) {
 *                              sts_ptr->push_back(-1);
 *                              return false;
 *                      } else {
 *                              sts_ptr->push_back(temp.at(0));
 *                              for(int i=1; i<tempSize; i++) {
 *                                      sts_ptr->push_back(temp.at(i));
 *                                      vs_ptr->push_back(firstEleVs);
 *                              }
 *                              return true;
 *                      }
 *              } else { //case3
 *                      bool flag = false; //no relation found yet
 *                      vector<int> vs_copy;
 *                      for(int i=0; i<size2; i++)
 *                              vs_copy.push_back(vs_ptr->at(i));
 *                      vs_ptr->clear();
 *
 *                      for(int i=0; i<size2; i++) {
 *                              int currentEleVs = vs_copy.at(i);
 *                              vector<int> temp = getModifiesFirst(currentEleVs);
 *                              int tempSize = temp.size();
 *                              if(tempSize==0) {
 *                                      sts_ptr->push_back(-1);
 *                                      vs_ptr->push_back(currentEleVs);
 *                              } else {
 *                                      flag=true;
 *                                      for(int j=0; j<tempSize; j++) {
 *                                              sts_ptr->push_back(temp.at(j));
 *                                              vs_ptr->push_back(currentEleVs);
 *                                      }
 *                              }
 *                      }
 *                      return flag;
 *              } //end of case3
 *
 *      } else if(size2==0) { //size2==0 && size1 !=0, case 4&5
 *              if(size1==1) { //case4
 *                      int firstEleSts = sts_ptr->at(0);
 *                      vector<int> temp = getModifiesSecond(firstEleSts);
 *                      int tempSize = temp.size();
 *                      if(tempSize==0) {
 *                              vs_ptr->push_back(-1);
 *                              return false;
 *                      } else {
 *                              vs_ptr->push_back(temp.at(0));
 *                              for(int i=1; i<tempSize; i++) {
 *                                      sts_ptr->push_back(firstEleSts);
 *                                      vs_ptr->push_back(temp.at(i));
 *                              }
 *                              return true;
 *                      }
 *              } else { //case5
 *                      bool flag = false; //no relation found yet
 *                      vector<int> sts_copy;
 *                      for(int i=0; i<size1; i++)
 *                              sts_copy.push_back(sts_ptr->at(i));
 *                      sts_ptr->clear();
 *
 *                      for(int i=0; i<size1; i++) {
 *                              int currentEleSts = sts_copy.at(i);
 *                              vector<int> temp = getModifiesSecond(currentEleSts);
 *                              int tempSize = temp.size();
 *                              if(tempSize==0) {
 *                                      sts_ptr->push_back(currentEleSts);
 *                                      vs_ptr->push_back(-1);
 *                              } else {
 *                                      flag=true;
 *                                      for(int j=0; j<tempSize; j++) {
 *                                              sts_ptr->push_back(currentEleSts);
 *                                              vs_ptr->push_back(temp.at(j));
 *                                      }
 *                              }
 *                      }
 *                      return flag;
 *              } //end of case5
 *      } else { //size1!=0 && size2!=0, case 6,7,8
 *              if(size1==1 && size2==1) { //case 6
 *                      if(isModifies(sts_ptr->at(0), vs_ptr->at(0)))
 *                              return true;
 *                      else
 *                              return false;
 *              } else { //case 7&8
 *                      throw exception("At least one vector has more than one element!");
 *              }
 *      }
 * }
 */
