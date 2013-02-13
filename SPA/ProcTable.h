#pragma once
#include <string>
#include <vector>
#include "SPAType.h"
using namespace std;

class ProcTable
{
//int size = 0;
vector<string> procedureNames;
STMT_LIST startStmts;
STMT_LIST endStmts;

public:
PROC_INDEX addProc(STRING procName, STMT start, STMT end);
STRING getProcName(PROC_INDEX ind);
PROC_INDEX getProcIndex(STRING procName);
STRING_LIST getAllProcName(PROC_INDEX_LIST inds);
STRING_LIST getAllProcName();
STMT getProcStart(PROC_INDEX ind);
STMT getProcEnd(PROC_INDEX ind);
int size();
};