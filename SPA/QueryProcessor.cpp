#include "stdafx.h"
#include "QueryProcessor.h"

QueryProcessor::QueryProcessor(void){ 
}

QueryProcessor::~QueryProcessor(void){	
}

void QueryProcessor::evaluate(string query, list<string>& results){
	if (qPre.parse(query) == 0) {
		if (!qEval.evaluate(qPre.getClauseTable(), qPre.getQueryVariableTable(), qPre.getQueryTargetTable(), results))
			if (qPre.getQueryVariableTable().at(qPre.getQueryTargetTable().at(0).varIndex).variableType == DT_BOOLEAN)
				results.push_back("false");
	}else{
		if (qPre.getQueryVariableTable().at(qPre.getQueryTargetTable().at(0).varIndex).variableType == DT_BOOLEAN)
			results.push_back("false");
	}
}
