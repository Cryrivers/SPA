#ifndef CALLS_H_
#define CALLS_H_

#include <string>
#include <vector>
#include "SPAType.h"

using namespace std;

class Calls
{
vector<STMT> stmts;
vector<PROC_INDEX> procs;
BOOLEAN isCalls(STMT stmt, PROC_INDEX proc);
STMT_LIST getCallsFirst(PROC_INDEX proc);
PROC_INDEX getCallsSecond(STMT stmt);
BOOLEAN calls_00(STMT_LIST* sts_p, PROC_INDEX_LIST* ps_p);
//BOOLEAN calls_01(STMT_LIST* sts_p, PROC_INDEX_LIST* ps_p);
//BOOLEAN calls_10(STMT_LIST* sts_p, PROC_INDEX_LIST* ps_p);
//BOOLEAN calls_11(STMT_LIST* sts_p, PROC_INDEX_LIST* ps_p);


public:
void addCalls(STMT stmt, PROC_INDEX proc);
BOOLEAN calls(STMT_LIST* sts_ptr, PROC_INDEX_LIST* ps_ptr, int arg);
STMT_LIST getAllCallsFirst();
PROC_INDEX_LIST getAllCallsSecond();
};


#endif /* CALLS_H_ */
