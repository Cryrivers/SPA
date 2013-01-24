#include "stdafx.h"
#include "Uses.h"

BOOLEAN Uses::isUses(STMT stmt, VAR_INDEX var)
{
	int size = stmts.size();

	for (int i = 0; i < size; i++) {
		if ((stmts.at(i) == stmt) && (vars.at(i) == var)) {
			return(true);
		}
	}
	return(false);
}


STMT_LIST Uses::getUsesFirst(VAR_INDEX var)
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


VAR_INDEX_LIST Uses::getUsesSecond(STMT stmt)
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


void Uses::addUses(STMT stmt, VAR_INDEX var)
{
	if (isUses(stmt, var)) {
		// Uses(stmt1, stmt2) already in the table, do nothing
	}else {
		// Uses(stmt1, stmt2) not in the table, add in
		stmts.push_back(stmt);
		vars.push_back(var);
	}
}


BOOLEAN Uses::uses(STMT_LIST *sts_ptr, VAR_INDEX_LIST *vs_ptr, int arg)
{
	switch (arg) {
	case 0:
		return(uses_00(sts_ptr, vs_ptr));

	case 1:
		return(uses_01(sts_ptr, vs_ptr));

	case 2:
		return(uses_10(sts_ptr, vs_ptr));

	case 3:
		return(uses_11(sts_ptr, vs_ptr));

	default:
		throw "arg value invalid";
	}
}


BOOLEAN Uses::uses_00(STMT_LIST *sts_p, VAR_INDEX_LIST *vs_p)
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
				if (isUses(sts_p->at(i), vs_p->at(j))) {
					return(true);
				}
			}
		}
		return(false);
	}
}


BOOLEAN Uses::uses_01(STMT_LIST *sts_p, VAR_INDEX_LIST *vs_p)
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
			vector<int> val = getUsesSecond(sts_p->at(i));
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
				if (isUses(sts_p->at(j), vs_p->at(index))) {
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


BOOLEAN Uses::uses_10(STMT_LIST *sts_p, VAR_INDEX_LIST *vs_p)
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
			vector<int> val = getUsesFirst(vs_p->at(i));
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
				if (isUses(sts_p->at(index), vs_p->at(j))) {
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


BOOLEAN Uses::uses_11(STMT_LIST *sts_p, VAR_INDEX_LIST *vs_p)
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
			vector<int> temp = getUsesFirst(currentEleVs);
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
			vector<int> temp = getUsesSecond(currentEleSts);
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
				if (isUses(sts_p->at(index), vs_p->at(index))) {
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


STMT_LIST Uses::getAllUsesFirst()
{
	return(stmts);
}


STMT_LIST Uses::getAllUsesSecond()
{
	return(vars);
}


/*
 * BOOLEAN Uses::uses(STMT_LIST* sts_ptr, VAR_INDEX_LIST* vs_ptr) {
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
 *                      vector<int> temp = getUsesFirst(firstEleVs);
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
 *                              vector<int> temp = getUsesFirst(currentEleVs);
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
 *                      vector<int> temp = getUsesSecond(firstEleSts);
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
 *                              vector<int> temp = getUsesSecond(currentEleSts);
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
 *                      if(isUses(sts_ptr->at(0), vs_ptr->at(0)))
 *                              return true;
 *                      else
 *                              return false;
 *              } else { //case 7&8
 *                      throw exception("At least one vector has more than one element!");
 *              }
 *      }
 * }
 */
