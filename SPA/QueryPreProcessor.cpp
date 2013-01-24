#include "stdafx.h"
#include "QueryPreprocessor.h"

//Local variables
PKBController * pkb;
int target_variable;
vector<Query_variable> query_var_table;
vector<Query_clause>   query_cla_table;

//Dictionaries
enum return_type{
	RT_SYNONYM,		//0
	RT_INTEGER,		//1
	RT_UNDERSCORE,	//2
	RT_CONSTANTVAR,	//3
	RT_EXPRESSION	//4
};	
int dic_clauseRef_size = 3;
string dic_clauseRef[] = {"such that", "and", "pattern"};										//3
int dic_designEntity_size = 6;
string dic_designEntity[] = {"stmt", "assign", "while", "variable", "constant", "prog_line"};	//6
int dic_relationRef_size = 6;
string dic_relationRef[] = {"Parent*", "Parent", "Follows*", "Follows", "Modifies", "Uses"};	//6

//Constructors
QueryPreprocessor::QueryPreprocessor(void){
	pkb = PKBController::createInstance();
	//cout<<"QueryPreprocessor constructed!"<<endl;
}
QueryPreprocessor::~QueryPreprocessor(void){	
	//cout<<"QueryPreprocessor deconstructed!"<<endl;
}

//Helper functions
bool isLetter(char a){
	if((a>='A'&&a<='Z')||(a>='a'&&a<='z')){
		return true;
	}
	else{
		return false;
	}
}

bool isDigit(char a){
	int tmp = (int) a;
	if(tmp>47&&tmp<58){
		return true;
	}
	else{
		return false;
	}
}

bool isInteger(string str){
	for(string::size_type i = 0; i<str.size(); i++){
		if(isDigit(str[i])==true)
			continue;
		else
			return false;
	}
	return true;
}

bool isSynonym(string str){ 
	if(str.size()<1&&!isLetter(str[0])){
		return false;
	}
	for(string::size_type i = 1; i<str.size(); i++){
		if(isLetter(str[i])==true||isDigit(str[i])==true||str[i]=='#')
			continue;
		else
			return false;
	}
	return true;
}

void trim(string& str, char del){
	string::size_type pos = str.find_last_not_of(del);
	if(pos != string::npos) {
		str.erase(pos + 1);
		pos = str.find_first_not_of(del);
		if(pos != string::npos) 
			str.erase(0, pos);
	}
	else 
		str.erase(str.begin(), str.end());
}

void trim_all(string& str){
	for(;;){ 
		if((str.find(" ")==0||(str.rfind(" ")==str.size()-1))&&str.size()>1){
			trim(str,' ');
		}else if((str.find(";")==0||str.rfind(";")==str.size()-1)&&str.size()>1){
			trim(str,';');
		}else if((str.find("\n")==0||(str.rfind("\n")==str.size()-1))&&str.size()>1){
			trim(str,'\n');
		}else{
			break;
		}
	}
} 

int verifyInFirst(string b, string a[], int size){
	for(int i=0;i<size;i++){  
		if(b.find(a[i])==0)
			return i;
	}
	return -1;
}

int verifyStmtRef(string str){
	if (isInteger(str)) {
		return RT_INTEGER;
	}else if(str=="_"){
		return RT_UNDERSCORE;
	}else if(isSynonym(str)){
		return RT_SYNONYM;
	}
	return -1;
}

int verifyEntRef(string str){
	if(isSynonym(str)){
		return RT_SYNONYM;
	}else if(str=="_"){
		return RT_UNDERSCORE;
	}else if(str.find("\"")<=str.size()){
		str = str.substr(str.find("\"")+1);
		if(str.find("\"")<=str.size()){
			str = str.substr(0,str.find("\""));
			if(isSynonym(str)){
				return RT_CONSTANTVAR;
			}
		}
	}
	return -1;
}

int verifyExprSpec(string str){
	if(str=="_"){
		return RT_UNDERSCORE;
	}else if(str[0]=='_'&&str[str.size()-1]=='_'){
		if(str[1]=='\"'&&str[str.size()-2]=='\"'){
			return RT_EXPRESSION;
		}
	}
	return -1;
}

vector<string> spilt(string& str, string require){
	vector<string> result;
	while(str.find(require)<str.size()){
		string tmp = str.substr(0, str.find(require));
		trim_all(tmp);
		if(tmp.size()>0){
			result.push_back(tmp);
		}
		str = str.substr(str.find(require)+require.size());
	}
	trim_all(str); 
	return result;
}

//Logic functions
int getIndexFromVarTable(string str){
	int size = (int) query_var_table.size();
	for(int i = 0; i< size; i++){
		if(query_var_table[i].content==str){
			return i;
		}else if(str=="constant"||str=="_"){ 
			Query_variable var; 
			if(str=="constant"){
				var.type=var.QV_CONSTANT;
			}else if(str=="_"){
				var.type=var.QV_UNDERSCORE;
			}
			var.dependency=-1;
			var.content=str;
			var.countAppear=0;
			query_var_table.push_back(var);
			return size;
		}else if(isInteger(str)){
			Query_variable var; 
			var.type=var.QV_STMT_NUMBER; 
			var.dependency=-1;
			var.content=str;
			var.countAppear=0;
			query_var_table.push_back(var);
			return size;
		}else if(str.find("\"")<=str.size()){
			str = str.substr(str.find("\"")+1);
			if(str.find("\"")<=str.size()){
				str = str.substr(0,str.find("\""));
				int in = pkb->getVarIndex(str);
				if(in<0){
					cout<<"Error @getIndexFromVarTable: Constant variable not in pkb."<<endl;
				} 
				Query_variable var; 
				var.type=var.QV_CONSTANTVAR; 
				var.dependency=-1; 
				var.content= to_string(static_cast<long long>(in));
				var.countAppear=0;
				query_var_table.push_back(var);
				return size;
			}else{
				return -1;
			}
		}
	}
	return -1;
}

bool setup_varTable(vector<string> declares){
	//cout<<"\nvarTable:"<<endl; 
	//for(int i=0; i<declares.size(); i++){
	//	cout<<i<<". "<<declares[i]<<endl;	
	//}
	for(string::size_type i=0; i<declares.size(); i++){
		int p = verifyInFirst(declares[i], dic_designEntity, dic_designEntity_size); 
		if(p==-1){
			cout<<"Error @setup_varTable(): Design entity type not found, recieve:["<<declares[i]<<"]"<<endl;
			return false;
		}else{ 
			Query_variable var; 
			if(dic_designEntity[p]=="variable"){
				var.type=var.QV_VARIABLE;
			}else if(dic_designEntity[p]=="stmt"){
				var.type=var.QV_STMT;
			}else if(dic_designEntity[p]=="assign"){
				var.type=var.QV_ASSIGN;
			}else if(dic_designEntity[p]=="while"){
				var.type=var.QV_WHILE;
			}else if(dic_designEntity[p]=="constant"){
				var.type=var.QV_CONSTANT;
			}else if(dic_designEntity[p]=="prog_line"){
				var.type=var.QV_STMT;
			}
			string sub=declares[i].substr(declares[i].find(" "));
			trim_all(sub);
			if(sub.find(",")>sub.size()){ 
				if(!isSynonym(sub)){
					cout<<"Error @setup_varTable: Declared variable is not a synonym, recieve:["<<sub<<"]"<<endl;
					return false;
				}
				int index = getIndexFromVarTable(sub);
				if(index!=-1){
					cout<<"Error @setup_varTable: Dupulicate variable at index"<<index<<", recieve:["<<sub<<"]"<<endl;
					return false;
				}  
				Query_variable newvar;
				newvar.type=var.type;
				newvar.dependency=-1;
				newvar.content=sub; 
				newvar.countAppear=0;
				query_var_table.push_back(newvar);
			}else{
				while(sub.find(",")<sub.size()){  
					string front = sub.substr(0, sub.find(","));
					trim_all(front); 
					if(!isSynonym(front)){
						cout<<"Error @setup_varTable: Declared variable is not a synonym, recieve:["<<front<<"]"<<endl;
						return false;
					} 
					int index = getIndexFromVarTable(front);
					if(index!=-1){
						cout<<"Error @setup_varTable: Dupulicate variable at index"<<index<<", recieve:["<<front<<"]"<<endl;
						return false;
					}
					Query_variable newvar;
					newvar.type=var.type;
					newvar.dependency=-1;
					newvar.content=front;
					newvar.countAppear=0;
					query_var_table.push_back(newvar);
					sub = sub.substr(sub.find(",")+1);
					trim_all(sub);
				} 
				if(!isSynonym(sub)){
					cout<<"Error @setup_varTable: Declared variable is not a synonym, recieve:["<<sub<<"]"<<endl;
					return false;
				}
				int index = getIndexFromVarTable(sub);
				if(index!=-1){
					cout<<"Error @setup_varTable: Dupulicate variable at index"<<index<<", recieve:["<<sub<<"]"<<endl;
					return false;
				}
				Query_variable newvar;
				newvar.type=var.type;
				newvar.dependency=-1;
				newvar.content=sub;
				newvar.countAppear=0;
				query_var_table.push_back(newvar);
			}

		}
	}
	return true; 
}

vector<string> spilt_clause(string& str){
	vector<string> tmp1;
	vector<string> result; 
	result.push_back(str);
	for(int i=0; i<dic_clauseRef_size; i++){ 
		tmp1=result;
		result.clear();
		for(string::size_type j=0; j<tmp1.size(); j++){ 
			string tmp2 = tmp1[j];
			trim_all(tmp2);
			vector<string> tmp3=spilt(tmp2, dic_clauseRef[i]); 
			for(string::size_type k=0; k<tmp3.size();k++){
				result.push_back(tmp3[k]); 
			}
			if(tmp2.size()>0){
				result.push_back(tmp2);
			} 
		}
	}
	return result;
}

vector<string> extractRelation(string str){ 
	string front;
	vector<string> result; 
	front = str.substr(0, str.find("("));
	str = str.substr(str.find("(")+1);
	trim_all(front);
	if(front.size()>0)
		result.push_back(front);
	front = str.substr(0, str.find(","));
	str = str.substr(str.find(",")+1);
	trim_all(front);
	if(front.size()>0)
		result.push_back(front);
	front = str.substr(0, str.find_last_of(")"));
	trim_all(front);
	if(front.size()>0)
		result.push_back(front);	
	return result;
}

bool setup_claTable(vector<string> claTable){
	//cout<<"\nclaTable:"<<endl; 
	//for(int i=0; i<claTable.size(); i++){
	//	cout<<i<<". "<<claTable[i]<<endl;	
	//}

	for(string::size_type i=0; i<claTable.size(); i++){
		vector<string> element = extractRelation(claTable[i]);
		if(element.size()!=3){
			cout<<"Error @setup_claTable: the format of relation is wrong, recieve: ["<<claTable[i]<<"]"<<endl;
			return false;
		}
		int m = verifyInFirst(element[0], dic_relationRef, 6); 
		Query_clause qc;
		qc.index=query_cla_table.size();
		if(m==1){
			qc.relation=qc.QC_PARENT;
		}else if(m==0){
			qc.relation=qc.QC_PARENTT;
		}else if(m==3){
			qc.relation=qc.QC_FOLLOWS;
		}else if(m==2){
			qc.relation=qc.QC_FOLLOWST;
		}else if(m==4){
			qc.relation=qc.QC_MODIFIES;
		}else if(m==5){
			qc.relation=qc.QC_USES;
		}else{
			qc.relation=qc.QC_PATTERN;
		}
		if(m==4||m==5){
			if(verifyStmtRef(element[1])==-1||verifyEntRef(element[2])==-1){
				cout<<"Error @setup_claTable(): Variable1 not StmtRef or Variable2 not EntRef. a:["<<element[1]<<"] b:["<<element[2]<<"]"<<endl;
				return false;
			}
			int ia = getIndexFromVarTable(element[1]);
			int ib = getIndexFromVarTable(element[2]);
			if(ia==-1||ib==-1){
				cout<<"Error @setup_claTable(): Variable1 or Variable2 not declared. a:["<<ia<<"] b:["<<ib<<"]"<<endl;
				return false;
			} 
			qc.variable1 = ia;
			qc.variable2 = ib;
		}else if(m>=0){  
			if(verifyStmtRef(element[1])==-1||verifyStmtRef(element[2])==-1){
				cout<<"Error @setup_claTable(): Variable1 or Variable2 not StmtRef. a:["<<element[1]<<"] b:["<<element[2]<<"]"<<endl;
				return false;
			}
			int ia = getIndexFromVarTable(element[1]);
			int ib = getIndexFromVarTable(element[2]);
			if(ia==-1||ib==-1){
				cout<<"Error @setup_claTable(): Variable1 or Variable2 not declared. a:["<<ia<<"] b:["<<ib<<"]"<<endl;
				return false;
			}
			if(ia==ib){
				cout<<"Error @setup_claTable(): Variable1 and Variable2 cannot be the same. a:["<<ia<<"] b:["<<ib<<"]"<<endl;
				return false;
			} 
			qc.variable1 = ia;
			qc.variable2 = ib;
		}else{
			int ia = getIndexFromVarTable(element[0]);
			if(query_var_table[ia].type!=query_var_table[0].QV_ASSIGN){
				cout<<"Error @setup_claTable(): Pattern has to work on assignment, recieve:"<<query_var_table[ia].type<<endl;
				return false;
			}
			if(verifyEntRef(element[1])==-1){
				cout<<"Error @setup_claTable(): Variable1 not StmtRef. a:["<<element[1]<<"] b:["<<element[2]<<"]"<<endl;
				return false;
			}
			int ib = getIndexFromVarTable(element[1]);
			if(ib==-1){
				cout<<"Error @setup_claTable(): Variable1 not declared. a:["<<ia<<"] b:["<<ib<<"]"<<endl;
				return false;
			} 
			qc.variable1 = ia;
			qc.variable2 = ib;
			qc.variable3 = (element[2]).substr(1,element[2].size()-2);
		}  
		query_cla_table.push_back(qc);
	}
	return true;
} 

bool setDependency(){
	if(query_var_table.size()==0||target_variable<0){
		return false;
	}
	//set appearer count
	for(int i = 0; i<(int)query_var_table.size(); i++){ 
		for(int j = 0; j<(int)query_cla_table.size(); j++){
			if(query_cla_table[j].variable1==i||query_cla_table[j].variable2==i){
				query_var_table[i].countAppear++;
			}
		}
	}
	for(int i = 0; i<(int)query_var_table.size(); i++){ 
		if((query_var_table[i].type==query_var_table[i].QV_STMT_NUMBER)||(query_var_table[i].type==query_var_table[i].QV_UNDERSCORE||(query_var_table[i].type==query_var_table[i].QV_CONSTANTVAR)||(query_var_table[i].type==query_var_table[i].QV_STMT_NUMBER))){
			continue;
		}else if(query_var_table[i].countAppear>1&&query_var_table[i].dependency==-1){
			query_var_table[i].dependency=i;
		}
	}
	//set variables that has dependency
	for(int i = 0; i<(int)query_var_table.size(); i++){ 
		for(int j = 0; j<(int)query_cla_table.size(); j++){
			int a = query_cla_table[j].variable1;
			int b = query_cla_table[j].variable2;
			if(i==a||i==b){
				if(query_var_table[a].dependency!=-1&&query_var_table[b].dependency!=-1){
					query_var_table[a].dependency=a;
					query_var_table[b].dependency=a;
				}
			}
		} 
	}
	return true;
}

int QueryPreprocessor::parse(string query){
	//cout<<">>"<<query<<"<<"<<endl;
	target_variable=-1;
	query_var_table.clear();
	query_cla_table.clear();
	trim_all(query);
	//1. get declares
	vector<string> declares = spilt(query, ";");
	if(declares.size()<0){
		return -1;
	}
	//2. setup vartable
	if(!setup_varTable(declares)){
		return -1;
	}

	//3. get query target
	string target;
	if(query.find("Select")==0){
		query = query.substr(query.find("Select")+6);
		trim_all(query);
		target = query.substr(0, query.find(" "));
		query = query.substr(query.find(" "));
	}else{
		return -1;
	}
	//4. setup query target
	//cout<<"\ntarget:"<<target<<endl; 
	int index = getIndexFromVarTable(target);
	if(index==-1){
		cout<<"Error @parse: Query target ["<<target<<"] is not declared!"<<endl;
		return false;
	}
	target_variable=index;
	query_var_table[target_variable].countAppear++;

	//5. get clauses
	vector<string> clauses = spilt_clause(query);
	if(clauses.size()<0){ 
		return -1;
	}
	//6. setup clatable
	if(!setup_claTable(clauses)){ 
		return -1;
	} 

	//7. set dependency
	if(!setDependency()){ 
		return -1;
	}

	cout<<"@parse:succeed!"<<endl;
	return 0;
} 

int QueryPreprocessor::getTargetVariable(){
	return target_variable;
}

vector<Query_variable> QueryPreprocessor::getQueryVariableTable(){
	return query_var_table;
}

vector<Query_clause> QueryPreprocessor::getQueryClauseTable(){
	return query_cla_table;
}