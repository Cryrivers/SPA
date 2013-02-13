#ifndef CALLS_H_
#define CALLS_H_

#include <string>
#include <vector>
#include "SPAType.h"

using namespace std;

class Calls
{
vector<STMT> stmts;
vector<PROC_INDEX> callers; //first
vector<PROC_INDEX> callees; //second
BOOLEAN isCalls(PROC_INDEX caller, PROC_INDEX callee);
BOOLEAN isCallsStar(PROC_INDEX caller, PROC_INDEX callee);
BOOLEAN isCallsStarHelper(PROC_INDEX_LIST links, PROC_INDEX callee);
PROC_INDEX_LIST getCallsFirst(PROC_INDEX callee);
PROC_INDEX_LIST getCallsSecond(PROC_INDEX caller);
PROC_INDEX_LIST getCallsStarFirst(PROC_INDEX callee);
PROC_INDEX_LIST getCallsStarSecond(PROC_INDEX caller);
BOOLEAN calls_00(PROC_INDEX_LIST* callers_p, PROC_INDEX_LIST* callees_p);
BOOLEAN calls_01(PROC_INDEX_LIST* callers_p, PROC_INDEX_LIST* callees_p);
BOOLEAN calls_10(PROC_INDEX_LIST* callers_p, PROC_INDEX_LIST* callees_p);
BOOLEAN calls_11(PROC_INDEX_LIST* callers_p, PROC_INDEX_LIST* callees_p);
BOOLEAN callsStar_00(PROC_INDEX_LIST* callers_p, PROC_INDEX_LIST* callees_p);
BOOLEAN callsStar_01(PROC_INDEX_LIST* callers_p, PROC_INDEX_LIST* callees_p);
BOOLEAN callsStar_10(PROC_INDEX_LIST* callers_p, PROC_INDEX_LIST* callees_p);
BOOLEAN callsStar_11(PROC_INDEX_LIST* callers_p, PROC_INDEX_LIST* callees_p);


public:
void addCalls(STMT stmt, PROC_INDEX caller, PROC_INDEX callee);
BOOLEAN calls(PROC_INDEX_LIST* callers_ptr, PROC_INDEX_LIST* callees_ptr, int arg);
BOOLEAN callsStar(PROC_INDEX_LIST* callers_ptr, PROC_INDEX_LIST* callees_ptr, int arg);
PROC_INDEX getCallee(STMT stmt);
PROC_INDEX_LIST getAllCallers();
PROC_INDEX_LIST getAllCallees();
int size();
};


#endif /* CALLS_H_ */
