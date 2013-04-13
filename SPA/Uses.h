/*
 * Uses.h
 *
 *  Created on: 21 Oct 2012
 *      Author: ray
 */

#ifndef USES_H_
#define USES_H_

#include "SPAType.h"
#include "Utility.h"
using namespace std;

class Uses
{
vector<int> stmts;
vector<int> vars;
//int size;

STMT_LIST getUsesFirst(VAR_INDEX var);
VAR_INDEX_LIST getUsesSecond(STMT stmt);
BOOLEAN uses_00(STMT_LIST* sts_p, VAR_INDEX_LIST* vs_p);
BOOLEAN uses_01(STMT_LIST* sts_p, VAR_INDEX_LIST* vs_p);
BOOLEAN uses_10(STMT_LIST* sts_p, VAR_INDEX_LIST* vs_p);
BOOLEAN uses_11(STMT_LIST* sts_p, VAR_INDEX_LIST* vs_p);

public:
void addUses(STMT stmt, VAR_INDEX var);
BOOLEAN isUses(STMT stmt, VAR_INDEX var);
BOOLEAN uses(STMT_LIST* sts_ptr, VAR_INDEX_LIST* vs_ptr, int arg);
STMT_LIST getAllUsesFirst();
STMT_LIST getAllUsesSecond();
};


#endif /* USES_H_ */
