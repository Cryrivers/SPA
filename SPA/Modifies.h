/*
 * Modifies.h
 *
 *  Created on: 21 Oct 2012
 *      Author: ray
 */

#ifndef MODIFIES_H_
#define MODIFIES_H_

#include "SPAType.h"
#include "Utility.h"
using namespace std;

class Modifies
{
vector<int> stmts;
vector<int> vars;
//int size;

STMT_LIST getModifiesFirst(VAR_INDEX var);
VAR_INDEX_LIST getModifiesSecond(STMT stmt);
BOOLEAN modifies_00(STMT_LIST* sts_p, VAR_INDEX_LIST* vs_p);
BOOLEAN modifies_01(STMT_LIST* sts_p, VAR_INDEX_LIST* vs_p);
BOOLEAN modifies_10(STMT_LIST* sts_p, VAR_INDEX_LIST* vs_p);
BOOLEAN modifies_11(STMT_LIST* sts_p, VAR_INDEX_LIST* vs_p);

public:
void addModifies(STMT stmt, VAR_INDEX var);
BOOLEAN isModifies(STMT stmt, VAR_INDEX var);
BOOLEAN modifies(STMT_LIST* sts_ptr, VAR_INDEX_LIST* vs_ptr, int arg);
STMT_LIST getAllModifiesFirst();
VAR_INDEX_LIST getAllModifiesSecond();
int size() {
	return stmts.size();
}
};


#endif /* MODIFIES_H_ */
