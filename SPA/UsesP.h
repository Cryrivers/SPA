/*
 * UsesPP.h
 *
 *  Created on: 21 Oct 2012
 *      Author: ray
 */

#ifndef USESP_H_
#define USESP_H_

#include "SPAType.h"
#include "Utility.h"
using namespace std;

class UsesP
{
PROC_INDEX_LIST procs;
VAR_INDEX_LIST vars;


PROC_INDEX_LIST getUsesPFirst(VAR_INDEX var);
VAR_INDEX_LIST getUsesPSecond(PROC_INDEX proc);
BOOLEAN usesP_00(PROC_INDEX_LIST* ps_p, VAR_INDEX_LIST* vs_p);
BOOLEAN usesP_01(PROC_INDEX_LIST* ps_p, VAR_INDEX_LIST* vs_p);
BOOLEAN usesP_10(PROC_INDEX_LIST* ps_p, VAR_INDEX_LIST* vs_p);
BOOLEAN usesP_11(PROC_INDEX_LIST* ps_p, VAR_INDEX_LIST* vs_p);

public:
void addUsesP(PROC_INDEX proc, VAR_INDEX var);
BOOLEAN isUsesP(PROC_INDEX proc, VAR_INDEX var);
BOOLEAN usesP(PROC_INDEX_LIST* ps_ptr, VAR_INDEX_LIST* vs_ptr, int arg);
PROC_INDEX_LIST getAllUsesPFirst();
PROC_INDEX_LIST getAllUsesPSecond();
};


#endif /* USESP_H_ */
