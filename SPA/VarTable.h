/*
 * VarTable.h
 *
 *  Created on: 21 Oct 2012
 *      Author: ray
 */

#ifndef VARTABLE_H_
#define VARTABLE_H_

#include <string>
#include <vector>
#include "SPAType.h"
using namespace std;

class VarTable
{
//int size = 0;
vector<string> variables;

public:
VAR_INDEX addVar(STRING varName);
STRING getVarName(VAR_INDEX ind);
VAR_INDEX getVarIndex(STRING varName);
STRING_LIST getAllVarName(VAR_INDEX_LIST inds);
STRING_LIST getAllVarName();
};


#endif /* VARTABLE_H_ */
