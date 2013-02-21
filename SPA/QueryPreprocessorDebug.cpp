#include "stdafx.h"
#include "QueryPreprocessor.h"

QueryPreprocessorDebug::QueryPreprocessorDebug(){
}

char* QueryPreprocessorDebug::typeToString(Type t){
	switch (t){
		case DT_PROCEDURE:
			return "procedure";
		case DT_STMTLST:
			return "stmtlst";
		case DT_STMT:
			return "stmt";
		case DT_ASSIGN:
			return "assign";
		case DT_CALL:
			return "call";
		case DT_WHILE:
			return "while";
		case DT_IF:
			return "if";
		case DT_VARIABLE:
			return "variable";
		case DT_CONSTANT:
			return "constant";
		case DT_BOOLEAN:
			return "boolean";
		case DT_UNDERSCORE:
			return "underscore";
		case KT_STMT_NUM:
			return "stmt_num";
		case KT_KNOWN_VARIABLE:
			return "known_var";
		case KT_KNOWN_PROCEDURE:
			return "known_proc";
		case KT_CONSTANT_INTEGER:
			return "const_int";
		case KT_CONSTANT_STRING:
			return "const_str";
		case AT_STMT_NUM:
			return "stmt_num";
		case AT_VAR_NAME:
			return "var_name";
		case AT_PROC_NAME:
			return "proc_name";
		case AT_CALL_PROC_NAME:
			return "callprocnm";
		case AT_VALUE:
			return "value";
		case AT_PROCTABLEINDEX:
			return "protbindex";
		case RT_MODIFIESP:
			return "ModifiesP";
		case RT_MODIFIESS:
			return "ModifiesS";
		case RT_USESP:
			return "UsesP";
		case RT_USESS:
			return "UsesS";
		case RT_CALLS:
			return "Calls";
		case RT_CALLST:
			return "Calls*";
		case RT_PARENT:
			return "Parent";
		case RT_PARENTT:
			return "Parent*";
		case RT_FOLLOWS:
			return "Follows";
		case RT_FOLLOWST:
			return "Follows*";
		case RT_NEXT:
			return "Next";
		case RT_NEXTT:
			return "Next*";
		case RT_AFFECTS:
			return "Affects";
		case RT_AFFECTST:
			return "Affects*";
		case CT_PATTERN:
			return "pattern";
		case CT_WITH:
			return "with";
		case KT_KNOWN_CONSTANT:
			return "known_const";
		case DT_PROGLINE:
			return "prog_line";
	}	  
	return "";
}

void QueryPreprocessorDebug::printQueryVariableTable(vector<QueryVariable> v){
	cout<<"\nQueryVariableTable:"<<endl;
	for(int i = 0; i< (int) v.size(); i++){	
		if(v[i].mapTo!=i){
			printf("%3d. MapTo[%3d] Origin[%9s] Type[%12s]\n", i,  v[i].mapTo, v[i].origin.c_str(), typeToString(v[i].variableType));
		}else{
			printf("%3d. MapTo[%3d] Origin[%9s] Type[%12s] ", i,  v[i].mapTo, v[i].origin.c_str(), typeToString(v[i].variableType));
			printf("Content[%3d] ",v[i].content);
			printf("Depd[%2d] Count[%2d]\n", v[i].dependency, v[i].countAppear);
		}
	}
}

void QueryPreprocessorDebug::printTargetVariableTable(vector<QueryTarget> v){
	cout<<"\nTargetVariable: "<<endl;
	for(int i = 0; i< (int) v.size(); i++){
		printf("%2d. Var[%3d] HasA[%d] Attr[%12s]\n",
			i,v[i].varIndex,v[i].hasAttribute,typeToString(v[i].attributeType));
	}
}

void QueryPreprocessorDebug::printQueryClauseTable(vector<QueryClause> v){
	cout<<"\nQueryClauseTable:"<<endl;
	for(int i = 0; i< (int) v.size(); i++){
		if(v[i].index==-1){
			printf("     Rel[%-10s] Var1[%2d] Var2[%2d]\n",typeToString(v[i].relationType), v[i].variable1, v[i].variable2);
		}else{
			printf("%3d. Rel[%-10s] Var1[%2d] Var2[%2d] ",
				v[i].index, typeToString(v[i].relationType), v[i].variable1, v[i].variable2); 
			if(typeToString(v[i].attribute1)!=""){
				printf("Attr1[%15s] Attr2[%15s] ",typeToString(v[i].attribute1),typeToString(v[i].attribute2));
			}
			if(v[i].patternType>=0){
				printf("Typ3[%d] Var3[%s] ", v[i].patternType, v[i].variable3.c_str());
			}					  
			cout<<"\n";
		}
	}
}
