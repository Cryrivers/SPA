#ifndef NEXT_H_
#define NEXT_H_

#include "SPAType.h"
#include "Utility.h"
using namespace std;

class Next
{
STMT_LIST stmt1s;
STMT_LIST stmt2s;
BOOLEAN isNext(STMT stmt1, STMT stmt2);
BOOLEAN isNextStar(STMT stmt1, STMT stmt2);
BOOLEAN isNextStarHelper(STMT_LIST links, STMT stmt2, STMT_LIST* visitedNodes);
STMT_LIST getNextFirst(STMT stmt);
STMT_LIST getNextSecond(STMT stmt);
STMT_LIST getNextStarFirst(STMT stmt2);
//STMT_LIST getNextStarFirstHelper(STMT stmt2, STMT_LIST* visitedNodes);
void getNextStarFirstHelper(STMT stmt2, STMT_LIST* resultLst);
STMT_LIST getNextStarSecond(STMT stmt1);
//STMT_LIST getNextStarSecondHelper(STMT stmt1, STMT_LIST* visitedNodes);
void getNextStarSecondHelper(STMT stmt1, STMT_LIST* resultLst);
BOOLEAN next_00(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN next_01(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN next_10(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN next_11(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN nextStar_00(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN nextStar_01(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN nextStar_10(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN nextStar_11(STMT_LIST* st1s_p, STMT_LIST* st2s_p);

public:
void addNext(STMT stmt1, STMT stmt2);
BOOLEAN next(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg);
BOOLEAN nextStar(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg);
STMT_LIST getAllNextFirst();
STMT_LIST getAllNextSecond();
};


#endif /* NEXT_H_ */
