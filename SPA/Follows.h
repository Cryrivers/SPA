/*
 * Follows.h
 *
 *  Created on: 21 Oct 2012
 *      Author: ray
 */

#ifndef FOLLOWS_H_
#define FOLLOWS_H_

#include "SPAType.h"
#include "Utility.h"
using namespace std;

class Follows
{
vector<int> stmt1s;
vector<int> stmt2s;
//int size;
int getFollowsFirst(int stmt);
int getFollowsSecond(int stmt);
STMT_LIST getFollowsStarFirst(STMT stmt);
STMT_LIST getFollowsStarSecond(STMT stmt);
BOOLEAN isFollows(STMT stmt1, STMT stmt2);
BOOLEAN isFollowsStar(STMT stmt1, STMT stmt2);
BOOLEAN follows_00(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN follows_01(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN follows_10(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN follows_11(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN followsStar_00(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN followsStar_01(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN followsStar_10(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN followsStar_11(STMT_LIST* st1s_p, STMT_LIST* st2s_p);

public:
void addFollows(STMT stmt1, STMT stmt2);
BOOLEAN follows(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg);
BOOLEAN followsStar(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg);
//void follows(STMT* stmt1_ptr, STMT* stmt2_ptr);
//BOOLEAN followsStar(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr);


};


#endif /* FOLLOWS_H_ */
