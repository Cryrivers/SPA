#ifndef AFFECTSBIP_H_
#define AFFECTSBIP_H_

#include "SPAType.h"
#include "Utility.h"
#include "PKBController.h"
#include <stack>
#include <queue>
using namespace std;


class AffectsBip
{
private:
PKBController* _pkb2;

BOOLEAN isAffectsBip(STMT stmt1, STMT stmt2);
void isAffectsBipHelper(CFGNode*, STMT, vector<CFGNode*>*, VAR_INDEX_LIST, int*, stack<STMT>);
BOOLEAN isAffectsBipStar(STMT stmt1, STMT stmt2);
void isAffectsBipStarHelper(STMT, STMT, STMT_LIST*, BOOLEAN*);
STMT_LIST getAffectsBipFirst(STMT, BOOLEAN);
void AffectsBip::getAffectsBipFirstHelper(CFGNode*, vector<CFGNode*>*, VAR_INDEX_LIST, BOOLEAN, STMT_LIST*, stack<STMT>);
STMT_LIST getAffectsBipSecond(STMT, BOOLEAN);
void getAffectsBipSecondHelper(CFGNode*, vector<CFGNode*>*, VAR_INDEX_LIST, BOOLEAN, STMT_LIST*, stack<STMT>);
STMT_LIST getAffectsBipStarFirst(STMT, BOOLEAN);
void getAffectsBipStarFirstHelper(STMT, STMT_LIST*);
STMT_LIST getAffectsBipStarSecond(STMT, BOOLEAN);
void getAffectsBipStarSecondHelper(STMT, STMT_LIST*);
BOOLEAN affectsBip_00(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN affectsBip_01(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN affectsBip_10(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN affectsBip_11(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN affectsBipStar_00(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN affectsBipStar_01(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN affectsBipStar_10(STMT_LIST* st1s_p, STMT_LIST* st2s_p);
BOOLEAN affectsBipStar_11(STMT_LIST* st1s_p, STMT_LIST* st2s_p);

public:
AffectsBip(void);
BOOLEAN affectsBip(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg);
BOOLEAN affectsBipStar(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg);
};


#endif /* AFFECTSBIP_H_ */
