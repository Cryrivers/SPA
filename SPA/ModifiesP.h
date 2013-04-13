/*
 * ModifiesPP.h
 *
 *  Created on: 21 Oct 2012
 *      Author: ray
 */

#ifndef MODIFIESP_H_
#define MODIFIESP_H_

#include "SPAType.h"
#include "Utility.h"
using namespace std;

class ModifiesP
{
PROC_INDEX_LIST procs;
VAR_INDEX_LIST vars;


PROC_INDEX_LIST getModifiesPFirst(VAR_INDEX var);
VAR_INDEX_LIST getModifiesPSecond(PROC_INDEX proc);
BOOLEAN modifiesP_00(PROC_INDEX_LIST* ps_p, VAR_INDEX_LIST* vs_p);
BOOLEAN modifiesP_01(PROC_INDEX_LIST* ps_p, VAR_INDEX_LIST* vs_p);
BOOLEAN modifiesP_10(PROC_INDEX_LIST* ps_p, VAR_INDEX_LIST* vs_p);
BOOLEAN modifiesP_11(PROC_INDEX_LIST* ps_p, VAR_INDEX_LIST* vs_p);

public:
void addModifiesP(PROC_INDEX proc, VAR_INDEX var);
BOOLEAN isModifiesP(PROC_INDEX proc, VAR_INDEX var);
BOOLEAN modifiesP(PROC_INDEX_LIST* ps_ptr, VAR_INDEX_LIST* vs_ptr, int arg);
PROC_INDEX_LIST getAllModifiesPFirst();
VAR_INDEX_LIST getAllModifiesPSecond();
};


#endif /* MODIFIESP_H_ */
