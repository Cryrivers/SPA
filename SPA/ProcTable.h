#pragma once
#include <string>
#include <vector>
#include "SPAType.h"
using namespace std;

class ProcTable
{
//int size = 0;
vector<string> procedures;

public:
VAR_INDEX addProc(STRING procName);
STRING getProcName(VAR_INDEX ind);
VAR_INDEX getProcIndex(STRING procName);
STRING_LIST getAllProcName(VAR_INDEX_LIST inds);
STRING_LIST getAllProcName();
};