#include "stdafx.h"
#include "QueryProcessor.h"

QueryProcessor::QueryProcessor(void){ 
}

QueryProcessor::~QueryProcessor(void){	
}

void QueryProcessor::evaluate(string query, list<string>& results){
	if (qPre.parse(query) == 0) {
		qEval.evaluate(qPre.getQueryClauseTable(), qPre.getQueryVariableTable(), qPre.getQueryTargetTable(), results);
	}
}
