#include "stdafx.h"
#include "UsesP.h"

BOOLEAN UsesP::isUsesP(PROC_INDEX proc, VAR_INDEX var)
{
	int size = procs.size();

	for (int i = 0; i < size; i++) {
		if ((procs.at(i) == proc) && (vars.at(i) == var)) {
			return(true);
		}
	}
	return(false);
}


PROC_INDEX_LIST UsesP::getUsesPFirst(VAR_INDEX var)
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


VAR_INDEX_LIST UsesP::getUsesPSecond(PROC_INDEX proc)
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


void UsesP::addUsesP(PROC_INDEX proc, VAR_INDEX var)
{
	if (isUsesP(proc, var)) {
		// UsesP(proc1, proc2) already in the table, do nothing
	}else {
		// UsesP(proc1, proc2) not in the table, add in
		procs.push_back(proc);
		vars.push_back(var);
	}
}


BOOLEAN UsesP::usesP(PROC_INDEX_LIST *ps_ptr, VAR_INDEX_LIST *vs_ptr, int arg)
{
	switch (arg) {
	case 0:
		return(usesP_00(ps_ptr, vs_ptr));

	case 1:
		return(usesP_01(ps_ptr, vs_ptr));

	case 2:
		return(usesP_10(ps_ptr, vs_ptr));

	case 3:
		return(usesP_11(ps_ptr, vs_ptr));

	default:
		throw "arg value invalid";
	}
}


BOOLEAN UsesP::usesP_00(PROC_INDEX_LIST *ps_p, VAR_INDEX_LIST *vs_p)
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
				if (isUsesP(ps_p->at(i), vs_p->at(j))) {
					return(true);
				}
			}
		}
		return(false);
	}
}


BOOLEAN UsesP::usesP_01(PROC_INDEX_LIST *ps_p, VAR_INDEX_LIST *vs_p)
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
			vector<int> val = getUsesPSecond(ps_p->at(i));
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
				if (isUsesP(ps_p->at(j), vs_p->at(index))) {
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


BOOLEAN UsesP::usesP_10(PROC_INDEX_LIST *ps_p, VAR_INDEX_LIST *vs_p)
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
			vector<int> val = getUsesPFirst(vs_p->at(i));
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
				if (isUsesP(ps_p->at(index), vs_p->at(j))) {
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


BOOLEAN UsesP::usesP_11(PROC_INDEX_LIST *ps_p, VAR_INDEX_LIST *vs_p)
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
			vector<int> temp = getUsesPFirst(currentEleVs);
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
		vector<int> ps_copy;
		for (int i = 0; i < size1; i++) {
			ps_copy.push_back(ps_p->at(i));
		}
		ps_p->clear();

		for (int i = 0; i < size1; i++) {
			int currentEleSts = ps_copy.at(i);
			vector<int> temp = getUsesPSecond(currentEleSts);
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
				if (isUsesP(ps_p->at(index), vs_p->at(index))) {
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


PROC_INDEX_LIST UsesP::getAllUsesPFirst()
{
	return(procs);
}


PROC_INDEX_LIST UsesP::getAllUsesPSecond()
{
	return(vars);
}
