/*
 * Parent.h
 *
 *  Created on: 21 Oct 2012
 *      Author: ray
 */

#ifndef PARENT_H_
#define PARENT_H_

#include "SPAType.h"
#include "Utility.h"
using namespace std;

class Parent
{
vector<int> stmt1s;
vector<int> stmt2s;
//int size;
int getParentFirst(int stmt);
vector<int> getParentSecond(int stmt);
STMT_LIST getParentStarFirst(STMT stmt);
STMT_LIST getParentStarSecond(STMT stmt);
BOOLEAN isParent(STMT stmt1, STMT stmt2);
BOOLEAN isParentStar(STMT stmt1, STMT stmt2);
BOOLEAN parent_00(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN parent_01(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN parent_10(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN parent_11(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN parentStar_00(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN parentStar_01(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN parentStar_10(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN parentStar_11(STMT_LIST* st1s_p, STMT_LIST* st2s_p);

public:
void addParent(STMT stmt1, STMT stmt2);
BOOLEAN parent(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg);
BOOLEAN parentStar(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg);
//void parent(STMT* stmt1_ptr, STMT* stmt2_ptr);
//BOOLEAN parentStar(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr);


};


#endif /* PARENT_H_ */
