#ifndef NEXTBIP_H_
#define NEXTBIP_H_

#include "SPAType.h"
#include "Utility.h"
#include "PKBController.h"
#include <stack>
#include <queue>
using namespace std;

class NextBip
{
//STMT_LIST stmt1s;
//STMT_LIST stmt2s;
//vector<CFGBipType> stmtBipType;
PKBController* _pkb;

BOOLEAN isNextBip(STMT stmt1, STMT stmt2);
BOOLEAN isNextBipStar(STMT stmt1, STMT stmt2);
void isNextBipStarHelper(CFGNode*, STMT, vector<CFGNode*>*, stack<STMT>, BOOLEAN*);
STMT_LIST getNextBipFirst(STMT stmt);
STMT_LIST getNextBipSecond(STMT stmt);
STMT_LIST getNextBipStarFirst(STMT stmt2);
void getNextBipStarFirstHelper(CFGNode*,vector<CFGNode*>*, stack<STMT>, STMT_LIST*);
STMT_LIST getNextBipStarSecond(STMT stmt1);
void getNextBipStarSecondHelper(CFGNode*,vector<CFGNode*>*, stack<STMT>, STMT_LIST*);
BOOLEAN nextBip_00(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN nextBip_01(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN nextBip_10(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN nextBip_11(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN nextBipStar_00(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN nextBipStar_01(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN nextBipStar_10(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN nextBipStar_11(STMT_LIST* st1s_p, STMT_LIST* st2s_p);

public:
NextBip(void);
//void addNext(STMT stmt1, STMT stmt2, CFGBipType bipType);
BOOLEAN nextBip(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg);
BOOLEAN nextBipStar(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg);
//STMT_LIST getAllNextFirst();
//STMT_LIST getAllNextSecond();
};


#endif /* NEXTBIP_H_ */
