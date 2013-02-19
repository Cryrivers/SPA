/********************************************//**
 * @file QueryPreprocessor.cpp
 * @brief Query Preprocessor
 * @see QueryPreprocessor
 * @author Zhang Haoqiang
 * @date 30.01.2012
 * @version 1.01
 ***********************************************/
#include "stdafx.h"
#include "QueryPreprocessor.h"
/********************************************//**
 * @brief  Constructor
 * @see QueryPreprocessor.h
 * @details Set up dictionary
 ***********************************************/
QueryPreprocessor::QueryPreprocessor(void){
	pkb = PKBController::createInstance();
	dicClause[0] = "such that ";
	dicClause[1] = "and ";
	dicClause[2] = "pattern ";
	dicClause[3] = "with ";
	dicDesignEntity[0] = "stmtLst ";
	dicDesignEntity[1] ="stmt ";
	dicDesignEntity[2] ="assign ";
	dicDesignEntity[3] ="while ";
	dicDesignEntity[4] ="variable ";
	dicDesignEntity[5] ="constant ";
	dicDesignEntity[6] ="prog_line ";
	dicDesignEntity[7] ="call ";
	dicDesignEntity[8] ="if ";
	dicDesignEntity[9] ="procedure ";
	dicRelationRef[0] = "Parent*";
	dicRelationRef[1] = "Parent";
	dicRelationRef[2] = "Follows*";
	dicRelationRef[3] = "Follows";
	dicRelationRef[4] = "Modifies";
	dicRelationRef[5] = "Uses";
	dicRelationRef[6] = "Affects*";
	dicRelationRef[7] = "Affects";
	dicRelationRef[8] = "Next*";
	dicRelationRef[9] = "Next";
	dicRelationRef[10] = "Calls*";
	dicRelationRef[11] = "Calls";
	dicAttribute[0] = ".procName";
	dicAttribute[1] =".varName";
	dicAttribute[2] =".value";
	dicAttribute[3] =".stmt#";
	dicAttribute[4] =".prog_line#";
}
/********************************************//**
 * @brief  Destructor
 ***********************************************/
QueryPreprocessor::~QueryPreprocessor(void){	
	//delete pkb;
} 

/********************************************//**
 * @param b string that is going to match
 * @param a string[] input dictionary
 * @param size integer size of the input dictionary
 * @brief	Used for matching the first word of the input string
 *	to the given dictionary
 * @return the index of the matched word in dictionary\n
 *    else return -1
 ***********************************************/
int QueryPreprocessor::getIndexOfFirstFrom(string b, string a[], int size){
	for(int i=0;i<size;i++){  
		if(b.find(a[i])==0)
			return i;
	}
	return -1;
}
/********************************************//**
 * @param b string that is going to match
 * @param a string[] input dictionary
 * @param size integer size of the input dictionary
 * @brief	Used for matching the next matching word of the input string
 *	from the given dictionary
 * @return the index of the matched word in dictionary\n
 *    else return -1 
 ***********************************************/
int QueryPreprocessor::getIndexOfNextFrom(string b, string a[], int size){
	int temp1, temp2=b.size(), temp3=size;
	for(int i=0;i<size;i++){  
		temp1 = b.find(a[i]); 
		if(temp1<temp2&&temp1>=0){
			temp2=temp1;
			temp3=i;
		}
	} 
	if(temp2==b.size()){
		return -1;
	}else{
		return temp3;
	}
}
/********************************************//**
 * @param b string that is going to match
 * @param a string[] input dictionary
 * @param size integer size of the input dictionary
 * @brief	Used for find the next word in the input string
 *	that match any given word in the given dictionary
 * @return the index of the matched word in dictionary\n
 *    else return -1
 ***********************************************/
int QueryPreprocessor::findNextIndexFrom(string b, string a[], int size){
	int temp1, temp2=b.size();
	for(int i=0;i<size;i++){  
		temp1 = b.find(a[i]); 
		if(temp1<temp2&&temp1>=0){
			temp2=temp1;
		}
	} 
	if(temp2==b.size()){
		return -1;
	}else{
		return temp2;
	}
}
/********************************************//**
 * @param str string& address of string
 * @param del char you want to delete
 * @brief  Trim the char from string
 ***********************************************/
void QueryPreprocessor::trim(string& str, char del){
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
/********************************************//**
 * @brief  Trim space, ';', '\\n'
 * @param str string& address of string
 * @details  Operation done from both side 
 ***********************************************/ 
void QueryPreprocessor::trim_all(string& str){
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
/********************************************//**
 * @brief  split the declartion part from query 
 * @param str string& address of string
 * @details  according to the ';' and further spilt into separate declare
 * @return a vector of choped items
 ***********************************************/
vector<string> QueryPreprocessor::getDeclares(string& str){
	vector<string> result;
	while(str.find(";")<str.size()){
		string tmp = str.substr(0, str.find(";"));
		trim_all(tmp);
		if(tmp.size()>0){
			result.push_back(tmp);
		}
		str = str.substr(str.find(";")+1);
	}
	trim_all(str); 
	return result;
}
/********************************************//**
 * @brief  split the result clause part from query
 * @param str string& address of string
 * @details  according to the next clause and further spilt into tuples
 * @warning 'Select' should be capitalized
 * @return a vector of choped items
 ***********************************************/
vector<string> QueryPreprocessor::getTargets(string& str){
	vector<string> result;
	trim_all(str);
	if(str.find("Select ")!=0){
		cout<<"@getTargets: Cannot find select clause. Got["<<str<<"]. "<<endl;
		return result;
	}
	str = str.substr(7);
	//split select
	int next = findNextIndexFrom(str, dicClause, 4);
	if(next == -1){
		cout<<"@getTargets: No clause. Got["<<str<<"]. "<<endl;
		return result;
	}
	string target = str.substr(0, next);
	trim_all(target);
	str = str.substr(next); 
	//split tuple
	if(target.find("<")<target.size()){
		if(target.find(">")<target.size()){ 
			target=target.substr(1); 
			target=target.substr(0,target.size()-1);  
			while(target.find(",")<target.size()){
				string tmp = target.substr(0, target.find(","));
				trim_all(tmp);
				if(tmp.size()>0){
					result.push_back(tmp);
				}
				target = target.substr(target.find(",")+1);
			}
			trim_all(target);
			result.push_back(target);
		}
	}else{
		result.push_back(target);
	}
	return result;
}
/********************************************//**
 * @brief  split the declartion part from query
 * @param str string& address of string
 * @details according to the clause dictionary\n
 *			The and clause will be replaced with the previous clause as defined
 * @return a vector of choped items
 ***********************************************/ 
vector<string> QueryPreprocessor::getClauses(string& str){
	vector<string> result;   
	string local = str;
	int currentClause = -1;
	int index, next = 0;
	trim_all(str);
	while(str.size()>0){
		index = getIndexOfFirstFrom(str, dicClause, DICCLAUSESIZE); 
		if(dicClause[index]=="such that "||dicClause[index]=="with "||dicClause[index]=="pattern "){
			currentClause=index;
			next = findNextIndexFrom(str.substr(dicClause[index].size()), dicClause, DICCLAUSESIZE); 
			if(next==-1){  
				result.push_back(str); 
				str = str.substr(str.size());
				break;
			}else{
				string newStr = str.substr(0, next+dicClause[index].size());
				result.push_back(newStr);  
				str = str.substr(next+dicClause[index].size());
			}
		}else if(dicClause[index]=="and "){
			if(currentClause==-1){
				cout<<"@getClauses: No previous deaclearation for and, got ["<<local<<"].";
				result.clear();
				return result;
			}else{ 
				next = findNextIndexFrom(str.substr(dicClause[index].size()), dicClause, DICCLAUSESIZE);
				if(next==-1){ 
					string newStr = dicClause[currentClause];
					newStr.append(str.substr(dicClause[index].size()));
					result.push_back(newStr); 
					str = str.substr(str.size());
					break;
				}else{
					string newStr1 = str.substr(0, next+dicClause[index].size());
					newStr1 = newStr1.substr(dicClause[index].size());
					string newStr2 = dicClause[currentClause];
					newStr2.append(newStr1);
					result.push_back(newStr2);  
					str = str.substr(next+dicClause[index].size());
				}
			}
		}else{
			break;
		}
	}
	return result;
}
/********************************************//**
 * @brief  Used to cut the relation
 * @param str string
 * @details cut the relation into top, front and back variables \n
 *  example:  Modifies(a, b) -> r, a, b
 * @return vector<string>
 ***********************************************/ 
vector<string> QueryPreprocessor::extractRelation(string str){ 
	trim_all(str);  
	vector<string> result; 
	string front;
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
/********************************************//**
 * @brief  Used to get attribute of type b.a 
 * @param a integer the index of attribute dictionary
 * @param b integer the index of var in queryVarTable
 * @details A type check is performed to prevent attribute not 
 *  matching variable type, like var.procName
 * @return attribut type
 * @see Type
 ***********************************************/
Type QueryPreprocessor::getAttributeOfVariable(int b, int a){ 
		if(dicAttribute[a]==".procName"&&queryVarTable[b].variableType==DT_PROCEDURE){
			return AT_PROC_NAME;
		}else if(dicAttribute[a]==".procName"&&queryVarTable[b].variableType==DT_CALL){
			return AT_CALL_PROC_NAME;
		}else if(dicAttribute[a]==".varName"&&queryVarTable[b].variableType==DT_VARIABLE){
			return AT_VAR_NAME;
		}else if(dicAttribute[a]==".value"&&queryVarTable[b].variableType==DT_CONSTANT){
			return AT_VALUE;
		}else if(dicAttribute[a]==".stmt#"&&(queryVarTable[b].variableType==DT_STMT||queryVarTable[b].variableType==DT_ASSIGN||queryVarTable[b].variableType==DT_CALL||queryVarTable[b].variableType==DT_WHILE||queryVarTable[b].variableType==DT_IF)){
			return AT_STMT_NUM;
		}else{  
			cout<<"@getAttributeOfVariable: Not match. Got["<<b<<"] ["<<dicAttribute[a]<<"]"<<endl;
			return ERROR;
		}		
}

/********************************************//**
 * @brief Find the index of given string in queryVarTable
 * @param str string
 * @param flag integer
 * Use of flag \n
 *	Reduce the loads of traverse and do validation \n 
 *		0 The variable don't need to exist
 *		1 The variable must exist
 *		2 Type boolean
 *		3 The variable will be a variable
 *		4 The variable will be a procedure
 *		5 '_'
 *		6 stmt_num
 * @details
 * Process: \n
 *		1. get the index of variables found inside the query body \n
 *		2. if new special variables will be added \n
 *		3. if it's already there then return the index \n
 *		   else return -1  \n\n
 * Cases: \n
 *		1. '_'		->underscore \n
 *		2. boolean	->boolean \n
 *		3. integer	->stmt num \n
 *		4. "NAME"	->known \n\n
 * Validates: \n
 *		1. If known variables exist in pkb \n
 *		2. If the required QueryVariable type is stmt \n
 * @note The flag 1 is a trick of playing with avaliability, thus variables 
 *		variable and assign cannot be tested in this function
 * @return int the index in queryVarTable or -1
 ***********************************************/
int QueryPreprocessor::getIndexFromVarTable(string str, int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7){
	int size = (int) queryVarTable.size();
	//================================
	//Use of flag
	//Reduce the loads of traverse and do validation
	//a0 The variable don't need to exist
	//a1 The variable must exist
	//a2 Type boolean
	//a3 The variable will be a variable
	//a4 The variable will be a procedure
	//a5 '_'
	//a6 stmt_num
	//a7 1 must be stmt
	//a7 2 must be var 
	//a7 2 must be proc
	if((str=="BOOLEAN"||str=="boolean")&&a2!=0){ 
		QueryVariable var; 
		var.variableType=DT_BOOLEAN;
		var.dependency=-1;
		var.content=-1;
		var.origin=str;
		var.countAppear=0;
		queryVarTable.push_back(var);
		return size;
	}
	if(str.find("\"")<=str.size()&&(a3!=0||a4!=0)){
		//================================
		//Handle cases of known variables 
		//================================
		str = str.substr(str.find("\"")+1);
		if(str.find("\"")<=str.size()){
			QueryVariable var; 
			str = str.substr(0,str.find("\""));
			int inV = pkb->getVarIndex(str);
			int inP = pkb->getProcIndex(str);
			if(a3!=0&&inV>=0){  
				var.variableType=KT_KNOWN_VARIABLE; 	
				var.content=inV;
				var.origin=str;	
				var.dependency=-1; 
				var.countAppear=0;
				queryVarTable.push_back(var); 
				return size;
			}else if(a4!=0&&inP>=0){ 
				var.variableType=KT_KNOWN_PROCEDURE; 
				var.content=inP;
				var.origin=str;
				var.dependency=-1; 
				var.countAppear=0;
				queryVarTable.push_back(var); 
				return size;
			}else{
				cout<<"@getIndexFromVarTable: Query variable ["<<str<<"] not in pkb."<<a3<<";"<<inV<<";"<<a4<<";"<<inP<<endl;
			}
		} 
	}
	if(str=="_"&&a5!=0){ 
		//================================
		//Handle cases of constant in declare or _ in query
		//example: constant c Select x such that Modifies(_,c)
		//================================
		QueryVariable var; 
		var.variableType=DT_UNDERSCORE;
		var.dependency=-1;
		var.content=-1;
		var.origin=str;
		var.countAppear=0;
		queryVarTable.push_back(var);
		return size;
	}
	if(qv.isInteger(str)&&a6!=0){
		//================================
		//Handle cases of constant stmt numbers
		//example: Parent(1,2)
		//================================
		QueryVariable var; 
		var.variableType=KT_STMT_NUM; 
		var.dependency=-1;
		var.content=atoi(str.c_str());
		var.origin=str;
		var.countAppear=0;
		queryVarTable.push_back(var);
		return size;
	}
	if(a1!=0||a0!=0){
		//================================
		//return the index in the var table
		//================================
		for(int i = 0; i< size; i++){
			if(queryVarTable[i].origin==str){
				if(a7==1){
					if(queryVarTable[i].variableType==DT_ASSIGN){
						return i;
					}else if(queryVarTable[i].variableType==DT_CALL){
						return i;
					}else if(queryVarTable[i].variableType==DT_IF){
						return i;
					}else if(queryVarTable[i].variableType==DT_WHILE){
						return i;
					}else if(queryVarTable[i].variableType==DT_STMT){
						return i;
					}else{
						cout<<"@getIndexFromVarTable: Query variable ["<<str<<"] not stmt."<<endl;
						return -1;
					}
				}else if(a7==2){
					if(queryVarTable[i].variableType==DT_VARIABLE){
						return i;
					}else{
						cout<<"@getIndexFromVarTable: Query variable ["<<str<<"] not var."<<endl;
						return -1;
					}
				}else if(a7==3){
					if(queryVarTable[i].variableType==DT_PROCEDURE){
						return i;
					}else{
						cout<<"@getIndexFromVarTable: Query variable ["<<str<<"] not var."<<endl;
						return -1;
					}
				}else{
					return i;
				}
			}
		}
	}
	if(a0==0){
		cout<<"@getIndexFromVarTable: Variable ["<<str<<"] not declared."<<endl;
	}
	return -1;
}
/********************************************//**
 * @brief insert the declared variables into the var table
 * @param declares vector<string> 
 *    elements in this vector is alraedy well formed
 * @details 
 * example:  if a, b stmt c, d \n\n
 * Process: \n
 *		1. Loop through the declare table and insert value to QueryVarTable \n
 *		2. Validate each variable \n\n
 * Cases: \n
 *		1. Only one variable is declared \n
 *		2. More than one variable is declared \n\n
 * Validates: \n
 *		1. Name should be synonms \n
 *		2. Variable cannot be duplicate \n
 * @note In order to check the duplicate, all flags for getIndexFromVarTable 
 *    should be set to 0, which indicate no adding is allowed.
 * @see Type
 * @see QueryVariable
 * @return The complitance of this function, true for done, false for error
 ***********************************************/ 
bool QueryPreprocessor::setupVarTable(vector<string> declares){ 
	for(unsigned int i=0; i<declares.size(); i++){
		int p = getIndexOfFirstFrom(declares[i], dicDesignEntity, DICDESIGNENTITYSIZE); 
		if(p==-1){
			//================================
			//The declares is not in the design entity dictionar
			//================================
			cout<<"@setupVarTable(): Design entity type not found, recieve:["<<declares[i]<<"]"<<endl;
			return false;
		}else{ 
			//================================
			//Subjected to Types for design entities
			//================================
			Type var; 
			if(dicDesignEntity[p]=="variable "){
				var=DT_VARIABLE;
			}else if(dicDesignEntity[p]=="stmt "){
				var=DT_STMT;
			}else if(dicDesignEntity[p]=="assign "){
				var=DT_ASSIGN;
			}else if(dicDesignEntity[p]=="call "){
				var=DT_CALL;
			}else if(dicDesignEntity[p]=="if "){
				var=DT_IF;
			}else if(dicDesignEntity[p]=="while "){
				var=DT_WHILE;
			}else if(dicDesignEntity[p]=="constant "){
				var=DT_CONSTANT;
			}else if(dicDesignEntity[p]=="prog_line "){
				var=DT_STMT;
			}else if(dicDesignEntity[p]=="stmtLst "){
				var=DT_STMTLST;
			}else if(dicDesignEntity[p]=="procedure "){
				var=DT_PROCEDURE;
			}else{
				cout<<"@setupVarTable: Design entity not exist. Got:["<<declares[i]<<"]"<<endl;
				return false;
			}
			string sub=declares[i].substr(declares[i].find(" "));
			trim_all(sub);
			if(sub.find(",")>sub.size()){ 
				//================================
				//Only one variable is declared
				//================================
				if(!qv.isSynonym(sub)){
					cout<<"@setupVarTable: Declared variable is not a synonym, recieve:["<<sub<<"]"<<endl;
					return false;
				}
				int index = getIndexFromVarTable(sub, 1,0,0,0,0,0,0,0);
				if(index!=-1){
					cout<<"@setupVarTable: Duplicate variable at index"<<index<<", recieve:["<<sub<<"]"<<endl;
					return false;
				}  
				QueryVariable newvar;
				newvar.variableType=var;
				newvar.dependency=-1;
				newvar.content=-1;
				newvar.origin=sub; 
				newvar.countAppear=0;
				queryVarTable.push_back(newvar);
			}else{
				//================================
				//More than one variable is declared
				//================================
				while(sub.find(",")<sub.size()){  
					string front = sub.substr(0, sub.find(","));
					trim_all(front); 
					if(!qv.isSynonym(front)){
						cout<<"@setupVarTable: Declared variable is not a synonym, recieve:["<<front<<"]"<<endl;
						return false;
					} 
					int index = getIndexFromVarTable(front, 1,0,0,0,0,0,0,0);
					if(index!=-1){
						cout<<"@setupVarTable: Dupulicate variable at index"<<index<<", recieve:["<<front<<"]"<<endl;
						return false;
					}
					QueryVariable newvar;
					newvar.variableType=var;
					newvar.dependency=-1;
					newvar.content=-1;
					newvar.origin=front;
					newvar.countAppear=0;
					queryVarTable.push_back(newvar);
					sub = sub.substr(sub.find(",")+1);
					trim_all(sub);
				} 
				if(!qv.isSynonym(sub)){
					cout<<"@setupVarTable: Declared variable is not a synonym, recieve:["<<sub<<"]"<<endl;
					return false;
				}
				int index = getIndexFromVarTable(sub, 1,0,0,0,0,0,0,0);
				if(index!=-1){
					cout<<"@setupVarTable: Dupulicate variable at index"<<index<<", recieve:["<<sub<<"]"<<endl;
					return false;
				}
				QueryVariable newvar;
				newvar.variableType=var;
				newvar.dependency=-1;
				newvar.origin=sub;
				newvar.content=-1;
				newvar.countAppear=0;
				queryVarTable.push_back(newvar);
			}
		}
	}
	return true; 
}
/********************************************//**
 * @brief get the select clause of the query
 * @param tarTable vector<string>
 *		elements in this vector is alraedy well formed
 * @note the target is considered to be a tuple all the time
 * @details 
 * Example:  s | s.stmt# \n\n
 * Process: \n
 *		1. Loop through the table and check if exists \n
 *		2. find if there is attribute \n
 *		3. Insert to QueryTarTable \n\n
 * Cases: \n
 *		1. Has attribute \n
 *		2. Single variable \n\n
 * Validates: \n
 *		each entry is a elem
 * @note 
 *		All variables needs to be predefined, thus flag is 0 \n
 *		Since variable in select clause already exist once, countAppear should be added
 * @see QueryTarget
 * @see Type
 * @return The complitance of this function, true for done, false for error
 ***********************************************/  
bool QueryPreprocessor::setupTarTable(vector<string> tarTable){
	for(unsigned int i = 0; i<tarTable.size(); i++){
		if(tarTable[i].find(".")<tarTable[i].size()){
			//================================
			//If the target contains attribute
			//================================
			string attrib = tarTable[i].substr(tarTable[i].find("."));
			tarTable[i] = tarTable[i].substr(0, tarTable[i].find("."));
			int index = getIndexFromVarTable(tarTable[i], 0,1,1,0,0,0,0,0);
			if(index==-1){
				cout<<"@setupTarTable: Variable not declared. Got["<<tarTable[i]<<"]"<<endl;
				return false;
			}
			QueryTarget newTarget;
			newTarget.varIndex=index;
			newTarget.hasAttribute=true;
			int index2 = getIndexOfNextFrom(attrib, dicAttribute, DICATTRIBUTESIZE);
			if(index2==-1){
				cout<<"@setupTarTable: Attribute not exist. Got["<<tarTable[i]<<"]"<<endl;
				return false;
			}  

			Type newType= getAttributeOfVariable(index, index2);
			if(newType==ERROR){
				return false;
			}
			newTarget.attributeType=newType;
			queryVarTable[index].countAppear++;
			queryTarTable.push_back(newTarget);
		}else{
			//================================
			//Single target
			//================================
			int index = getIndexFromVarTable(tarTable[i], 0,1,1,0,0,0,0,0);
			if(index==-1){
				cout<<"@setupTarTable: Variable not declared. Got["<<tarTable[i]<<"]"<<endl;
				return false;
			}
			QueryTarget newTarget;
			newTarget.varIndex=index;
			newTarget.hasAttribute=false;
			queryVarTable[index].countAppear++;
			queryTarTable.push_back(newTarget);
		} 
	}
	return true;
} 
/********************************************//**
 * @brief
 * @param claTable vector<string> 
 *    elements in this vector is alraedy well formed
 * @details 
 * example:  if a, b stmt c, d \n
 * Cases: \n\n
 *	1.with: \n\n
 *		Example: attrRef '=' ref | synonym '=' ref-pl \n\n
 *		Process: \n
 *			1. get attribute or constant on both side \n
 *			2. validate the exsitence of attri and variable \n
 *			3. optimise results \n\n
 *		Validates: \n
 *			1. Type on the left should be attrRef or synonym \n
 *			2. The content along the equal sign should be comparable \n\n
 *		Optimise: \n
 *			1. merge single variable like proc.procName="sth"  \n
 *				proc->kownn_proc, pkb index into origin \n
 *				var ->known_var, pkb index into origin \n
 *				stmt->stmt_num, integer into origin \n
 *				call.stmtnumber->kownn_proc, pkb index into origin \n 
 *			2. constant.value \n
 *			3. calls with string will create a new known proc variable \n
 *			4. if the case of p.procname=q.procname make a flag for this!! mergeFlag = 1 \n\n 
 *	2.pattern: \n\n
 *		Process: \n
 *			1. sort out three different cases \n
 *			2. validate the exsitence of attri and variable \n
 *			3. optimise results \n\n
 *		Validates: \n
 *			1. The first argument is either assign, while, if \n 
 *			2. Match the following case:\n 
 *				assign( varRef, expression-spec | '_' ) \n 
 *				while( varRef, '_' ) \n 
 *				if( varRef, '_' ',' '_' ) \n\n 
 *		Optimise: \n
 *			Currently none \n\n 
 *	3.such that: \n\n
 *		Process: \n
 *			Just validate and insert into claTable
 *		Validates: \n
 *			Relation is "Parent*", "Parent", "Follows*", "Follows" \n
 *				var1 must be stmtRef \n
 *				var2 must be stmtRef \n
 *				var1 must be stmt or '_' \n
 *				var2 must be stmt or '_' \n
 *				cannot be the same, except when '_' \n
 *				integer argument means statement number \n\n		
 *			Relation is "Modifies", "Uses" \n
 *				var1 must be entRef \n
 *				var2 must be varRef \n
 *				var1 must be stmt or procedure \n
 *				var2 must be variable or '_' \n
 *				integer argument means statement number \n\n
 *			Relation is "Affects*", "Affects" \n
 *				var1 must be stmtRef \n
 *				var2 must be stmtRef \n
 *				var1 must be assign or '_' \n
 *				var2 must be assign or '_' \n
 *				cannot be the same, except when '_' \n
 *				integer argument means statement number \n\n
 *			Relation is "Next*", "Next" \n
 *				var1 must be lineRef \n
 *				var2 must be lineRef \n
 *				var1 must be stmt or stmt number or '_' \n
 *				var2 must be stmt or stmt number or '_' \n
 *				integer arguments mean program line numbers \n\n
 *			Relation is "Call*", "Call" \n
 *				var1 must be entRef \n
 *				var2 must be entRef \n
 *				var1 must be procedure or '_' \n
 *				var2 must be procedure or '_' \n\n
 *				cannot be the same (no recusive), except when '_' \n
 *		Optimise: \n
 *			Currently none \n\n 
 * @note The and clause is already handled by getClauses()
 * @see Type
 * @see getClauses
 * @see QueryClause
 * @return The complitance of this function, true for done, false for error
 ***********************************************/ 
bool QueryPreprocessor::setupClaTable(vector<string> claTable){ 
	for(unsigned int i=0; i<claTable.size(); i++){
		//cout<<queryClaTable.size()<<"  "<<claTable[i]<<endl;
		if(claTable[i].find("with ")==0){
			claTable[i] = claTable[i].substr(5);
			if(claTable[i].find("=")>claTable[i].size()){
				cout<<"@setupClaTable: No equality found in \"with\". Got ["<<claTable[i]<<"]"<<endl;
				return false;
			}
			//split left and right
			string front = claTable[i].substr(0, claTable[i].find("="));
			string back = claTable[i].substr(claTable[i].find("=")+1);
			trim_all(front);
			trim_all(back);
			//split attribute   
			QueryClause qc;
			qc.index=queryClaTable.size();
			qc.relationType=CT_WITH;
			if(qv.isRef(front)&&qv.isRef(back)){
				//===========================
				//ref '=' ref
				//===========================
				int frontAttribute = getIndexOfNextFrom(front, dicAttribute, DICATTRIBUTESIZE); 
				int backAttribute = getIndexOfNextFrom(back, dicAttribute, DICATTRIBUTESIZE);  
				if(frontAttribute != -1 && backAttribute != -1){
					//===========================
					//both have attribute
					//===========================
					front = front.substr(0, front.find("."));
					back = back.substr(0, back.find("."));
					//===========================
					//both exist
					//===========================
					int ia = getIndexFromVarTable(front, 0,1,0,0,0,0,0,0);
					int ib = getIndexFromVarTable(back, 0,1,0,0,0,0,0,0);
					if(ia==-1||ib==-1){
						cout<<"@setupClaTable: Variable not exist! Got["<<front<<"], ["<<back<<"]"<<endl;
						return false;
					} 
					qc.variable1 = ia;
					//===========================
					//attribute type right
					//===========================
					Type ta = getAttributeOfVariable(frontAttribute, ia);
					if(ta==ERROR){
						cout<<"@setupClaTable: Variable does not own this attribute! Got["<<front<<"], ["<<frontAttribute<<"]"<<endl;
						return false;
					} 
					qc.attribute1 = ta;
					qc.variable2 = ib;
					Type tb = getAttributeOfVariable(backAttribute, ib);
					if(tb==ERROR){
						cout<<"@setupClaTable: Variable does not own this attribute! Got["<<back<<"], ["<<backAttribute<<"]"<<endl;
						return false;
					} 
					qc.attribute2 = tb;
					//===========================
					//Same attribute
					//check if type is the same
					//
					//note for the stmt=if
					//===========================
					if(ta==tb){
						if(queryVarTable[ia].variableType!=KT_STMT_NUM&&queryVarTable[ib].variableType!=KT_STMT_NUM){
							 if(queryVarTable[ia].variableType!=queryVarTable[ia].variableType){
								return false;
							 }
						}						 
						mergeFlag.push_back(ia);
						mergeFlag.push_back(ib);
						continue;
					}
					//===========================
					//Comparable
					//===========================
					if((ta==AT_PROC_NAME||ta==AT_CALL_PROC_NAME||ta==AT_VAR_NAME)&&(tb==AT_PROC_NAME||tb==AT_CALL_PROC_NAME||tb==AT_VAR_NAME)){
						queryClaTable.push_back(qc);
						continue;
					}else if((ta==AT_STMT_NUM)&&(tb==AT_STMT_NUM)){
						queryClaTable.push_back(qc);
						continue;
					}else{
						cout<<"@setupClaTable: Attribute not comparable! Got["<<ta<<"], ["<<tb<<"]"<<endl;
						return false;
					}					
				}else if((frontAttribute != -1 && backAttribute == -1)||(frontAttribute == -1 && backAttribute != -1)){
					if(frontAttribute == -1 && backAttribute != -1){
						int tmpt = backAttribute;
						frontAttribute = backAttribute;
						backAttribute = tmpt;
						string tmp = back;
						front = back;
						back = tmp;
					}
					//===========================
					//back has no attribute
					//===========================
					front = front.substr(0, front.find(".")); 
					int ia = getIndexFromVarTable(front, 0,1,0,0,0,0,0,0); 
					if(ia==-1){
						cout<<"@setupClaTable: Variable not exist! Got["<<front<<"], ["<<back<<"]"<<endl;
						return false;
					} 
					//===========================
					//front match back type
					//===========================
					if(qv.isInteger(back)){
						if(getAttributeOfVariable(frontAttribute, ia)==AT_STMT_NUM){
							//===========================
							//front will add in back as stmt number
							//
							//is match accordingly.
							//===========================
							queryVarTable[ia].content=atoi(back.c_str());
							queryVarTable[ia].variableType=KT_STMT_NUM;
						}else if(getAttributeOfVariable(frontAttribute, ia)==AT_VALUE){	 
							//===========================
							//front will add in back as known constant
							//===========================
							queryVarTable[ia].content=atoi(back.c_str());
							queryVarTable[ia].variableType=KT_KNOWN_CONSTANT;
						}else{
							cout<<"@setupClaTable: Variable ["<<queryVarTable[ia].origin<<"] attribute not of type integer."<<endl;
							return false;
						}
					}else if(back.find("\"")<back.size()){
						back = back.substr(back.find("\"")+1);
						if(back.find("\"")<=back.size()){
							back = back.substr(0,back.find("\""));
							if(qv.isSynonym(back)){
								//===========================
								//back is now confirmed to be a IDENT, therefore front will have
								//string as value which has to be a call or variable or proc
								//===========================
								if(getAttributeOfVariable(frontAttribute,ia)==AT_VAR_NAME){ 
									//=========================== 
									//change variable to known variable
									//=========================== 
									int inV = pkb->getVarIndex(back);
									if(inV<0){
										cout<<"@getIndexFromVarTable: Variable ["<<back<<"] not in pkb."<<endl;
										return false;
									}
									queryVarTable[ia].variableType=KT_KNOWN_VARIABLE; 	
									queryVarTable[ia].content=inV;
								}else if(getAttributeOfVariable(frontAttribute,ia)==AT_PROC_NAME&&queryVarTable[ia].variableType==DT_CALL){
									//=========================== 
									//create known proc for call
									//=========================== 
									int inP = pkb->getProcIndex(back);
									if(inP<0){
										cout<<"@getIndexFromVarTable: Procedure ["<<back<<"] not in pkb."<<endl;
										return false;
									} 
									QueryVariable var;
									var.variableType=KT_KNOWN_PROCEDURE;
									var.content=inP;
									var.origin=back;
									var.countAppear=0;
									var.dependency=-1;
									queryVarTable.push_back(var);

									qc.variable1=ia;
									qc.attribute1=AT_CALL_PROC_NAME;
									qc.variable2=queryVarTable.size()-1;
									qc.attribute2=AT_PROCTABLEINDEX;
									queryClaTable.push_back(qc);
									return true;
								}else if(getAttributeOfVariable(frontAttribute,ia)==AT_PROC_NAME){
									//=========================== 
									//change to known proc
									//=========================== 
									int inP = pkb->getProcIndex(back);
									if(inP<0){
										cout<<"@getIndexFromVarTable: Procedure ["<<back<<"] not in pkb."<<endl;
										return false;
									}
									queryVarTable[ia].variableType=KT_KNOWN_PROCEDURE; 
									queryVarTable[ia].content=inP; 
								}
							}
						}													
					}
				}else if(frontAttribute == -1 && backAttribute == -1){
					if(front!=back){
						cout<<"@setupClaTable: \"with\" return false! Got["<<front<<"="<<back<<"]"<<endl;
						return false;
					}
				}else{
					cout<<"@setupClaTable: Unkown ref! Got["<<front<<"], ["<<back<<"]"<<endl;
					return false;
				}
			}else{
				cout<<"@setupClaTable: with clause fromat wrong. Got[with "<<front<<"="<<back<<"]"<<endl;
				return false;
			}
		}else if(claTable[i].find("pattern ")==0){
			claTable[i] = claTable[i].substr(8);
			vector<string> element = extractRelation(claTable[i]);
			int ia = getIndexFromVarTable(element[0], 0,1,0,0,0,0,0,1);
			if(ia==-1){
				cout<<"@setupClaTable(): assign|if|while not exist. Got["<<element[0]<<"]"<<endl;
			}
			QueryClause qc;
			qc.index=queryClaTable.size();
			if(queryVarTable[ia].variableType==DT_ASSIGN){
				if(!qv.isVarRef(element[1])){
					cout<<"@setupClaTable(): Variable1 not VarRef. a:["<<element[1]<<"]"<<endl;
					return false;
				}
				int ib = getIndexFromVarTable(element[1], 0,1,0,1,0,1,0,2);
				if(ib==-1){
					return false;
				} 
				qc.relationType=CT_PATTERN;
				qc.variable1 = ia;
				qc.variable2 = ib;
				trim_all(element[2]); 
				if(element[2].find("_")==0){
					if(element[2].size()==1){
						element[2] = "";
						qc.variable3=element[2];
						qc.patternType=PATTERN_ASSIGN_UNDERSCORE_ONLY;
					}else if(element[2].find_last_of("_")== element[2].size()-1){
						element[2] = element[2].substr(2);
						element[2] = element[2].substr(0, element[2].size()-2);
						qc.variable3=element[2];
						qc.patternType=PATTERN_ASSIGN_WITH_UNDERSCORE;
					}else{
						 return false;
					}
				}else if(element[2].find("\"")==0){
					if(element[2].find_last_of("\"")!=0){ 
						element[2] = element[2].substr(1);
						element[2] = element[2].substr(0, element[2].size()-1);
						qc.variable3=element[2];
						qc.patternType=PATTERN_ASSIGN_NO_UNDERSCORE;
					}else{
						return false;
					}
				}else{
					return false;
				}
				queryClaTable.push_back(qc); 
				return true;
			}else if(queryVarTable[ia].variableType==DT_WHILE){
				if(!qv.isVarRef(element[1])){
					cout<<"@setupClaTable(): Variable1 not VarRef. a:["<<element[1]<<"] b:["<<element[2]<<"]"<<endl;
					return false;
				}
				int ib = getIndexFromVarTable(element[1], 0,1,0,1,0,1,0,2);
				if(ib==-1){
					return false;
				} 
				qc.relationType=CT_PATTERN;
				qc.variable1 = ia;
				qc.variable2 = ib;
				qc.patternType=PATTERN_WHILE;
				qc.variable3="";
				queryClaTable.push_back(qc); 
				return true;
			}else if(queryVarTable[ia].variableType==DT_IF){
				if(!qv.isVarRef(element[1])){
					cout<<"@setupClaTable(): Variable1 not VarRef. a:["<<element[1]<<"] b:["<<element[2]<<"]"<<endl;
					return false;
				}
				int ib = getIndexFromVarTable(element[1], 0,1,0,1,0,1,0,2);
				if(ib==-1){
					return false;
				} 
				qc.relationType=CT_PATTERN;
				qc.variable1 = ia;
				qc.variable2 = ib;
				qc.variable3 = ""; 
				qc.patternType=PATTERN_IF;
				queryClaTable.push_back(qc);
				return true;
			}else{
				cout<<"@setupClaTable(): Pattern has to work on assign,if,while Got:"<<queryVarTable[ia].variableType<<endl;
				return false;
			}
		}else{
			//================================
			//If the clauses is "such that "
			//================================
			claTable[i] = claTable[i].substr(10); 
			vector<string> element = extractRelation(claTable[i]);
			if(element.size()!=3){
				cout<<"@setupClaTable: the format of relation is wrong, recieve: ["<<claTable[i]<<"]"<<endl;
				return false;
			}
			int m = getIndexOfFirstFrom(element[0], dicRelationRef, DICRELATIONREFSIZE); 
			QueryClause qc;
			qc.index=queryClaTable.size();
			//================================
			//Following will validate and set 
			//   .variable1 .variable2 and 
			//   .relationType to clauseTable
			//================================
			if(m==0||m==1||m==2||m==3){
				//================================
				//Relation is "Parent*", "Parent", "Follows*", "Follows"
				//
				//
				//var1 must be stmtRef
				//var2 must be stmtRef
				//var1 must be stmt or '_' or stmt num
				//var2 must be stmt or '_' or stmt num
				//cannot be the same, except when '_'
				//integer argument means statement number
				//================================ 
				if(element[1]==element[2]&&(element[1]!="_"||element[2]!="_")){
					cout<<"@setupClaTable(): Variable cannot be the same for Parent and Follows a:["<<element[1]<<"] b:["<<element[2]<<"]"<<endl;
					return false;
				}
				if(!qv.isStmtRef(element[1])||!qv.isStmtRef(element[2])){
					cout<<"@setupClaTable(): Variable not right for Parent, Follows: a:["<<element[1]<<"] b:["<<element[2]<<"]"<<endl;
					return false;
				}
				int ia = getIndexFromVarTable(element[1], 0,1,0,0,0,1,1,1);
				int ib = getIndexFromVarTable(element[2], 0,1,0,0,0,1,1,1);
				if(ia==-1||ib==-1){
					return false;
				}  
				if(m==0){
					qc.relationType=RT_PARENTT;
				}else if(m==1){
					qc.relationType=RT_PARENT;
				}else if(m==2){
					qc.relationType=RT_FOLLOWST;
				}else if(m==3){
					qc.relationType=RT_FOLLOWS;
				}
				qc.variable1 = ia;
				qc.variable2 = ib;
			}else if(m==4||m==5){
				//================================
				//Relation is "Modifies", "Uses"
				//
				//
				//var1 must be entRef
				//var2 must be varRef
				//var1 must be stmt or procedure
				//var2 must be variable or '_'
				//integer argument means statement number
				//================================ 
				int ia,ib;
				ia = getIndexFromVarTable(element[1], 0,1,0,0,1,0,1,0);  
				ib = getIndexFromVarTable(element[2], 0,1,0,1,0,1,0,2);
				if(ia==-1||ib==-1){
					cout<<"@setupClaTable(): Variable not exist: a:["<<element[1]<<"] b:["<<element[2]<<"]"<<endl;
					return false;
				}  
				if(m==4){
					if(queryVarTable[ia].variableType==DT_PROCEDURE||queryVarTable[ia].variableType==KT_KNOWN_PROCEDURE){
						qc.relationType=RT_MODIFIESP;
					}else if(queryVarTable[ia].variableType!=DT_VARIABLE&&queryVarTable[ia].variableType!=KT_KNOWN_VARIABLE){
						qc.relationType=RT_MODIFIESS;
					}else{
						cout<<"@setupClaTable(): Var1 of Modifies cannot be variable"<<endl;
						return false;
					}
				}else if(m==5){
					if(queryVarTable[ia].variableType==DT_PROCEDURE||queryVarTable[ia].variableType==KT_KNOWN_PROCEDURE){
						qc.relationType=RT_USESP;
					}else if(queryVarTable[ia].variableType!=DT_VARIABLE&&queryVarTable[ia].variableType!=KT_KNOWN_VARIABLE){
						qc.relationType=RT_USESS;
					}else{
						cout<<"@setupClaTable(): Var1 of Modifies cannot be variable"<<endl;
						return false;
					}
				}
				qc.variable1 = ia;
				qc.variable2 = ib;
			}else if(m==6||m==7){
				//================================
				//Relation is "Affects*", "Affects"
				//
				// 
				//var1 must be stmtRef
				//var2 must be stmtRef
				//var1 must be assign or '_'
				//var2 must be assign or '_'
				//cannot be the same, except when '_'
				//integer argument means statement number
				//================================ 
				if(element[1]==element[2]&&(element[1]!="_"||element[2]!="_")){
					cout<<"@setupClaTable(): Variable cannot be the same for Affect. a:["<<element[1]<<"] b:["<<element[2]<<"]"<<endl;
					return false;
				}
				if(!qv.isStmtRef(element[1])||!qv.isStmtRef(element[2])){
					cout<<"@setupClaTable(): Variable right for Affect. a:["<<element[1]<<"] b:["<<element[2]<<"]"<<endl;
					return false;
				}
				int ia = getIndexFromVarTable(element[1], 0,1,0,0,0,1,1,1);
				int ib = getIndexFromVarTable(element[2], 0,1,0,0,0,1,1,1);
				if(ia==-1||ib==-1){
					return false;
				} 
				if(queryVarTable[ia].variableType!=DT_ASSIGN||queryVarTable[ia].variableType!=DT_UNDERSCORE){
					cout<<"@setupClaTable(): Variable 1 not assign: ["<<ia<<"]"<<endl;
					return false;
				}
				if(queryVarTable[ib].variableType!=DT_ASSIGN||queryVarTable[ib].variableType!=DT_UNDERSCORE){
					cout<<"@setupClaTable(): Variable 2 not assign: ["<<ib<<"]"<<endl;
					return false;
				}
				if(m==6){
					qc.relationType=RT_AFFECTST;
				}else if(m==7){
					qc.relationType=RT_AFFECTS;
				}
				qc.variable1 = ia;
				qc.variable2 = ib;
			}else if(m==8||m==9){
				//================================
				//Relation is "Next*", "Next"
				//
				//
				//var1 must be lineRef
				//var2 must be lineRef
				//var1 must be stmt or stmt number or '_'
				//var2 must be stmt or stmt number or '_'
				//integer arguments mean program line numbers
				//================================ 
				if(!qv.isLineRef(element[1])||!qv.isLineRef(element[2])){
					cout<<"@setupClaTable(): Variable not right for Next. a:["<<element[1]<<"] b:["<<element[2]<<"]"<<endl;
					return false;
				}
				int ia = getIndexFromVarTable(element[1], 0,1,0,0,0,1,1,0);
				int ib = getIndexFromVarTable(element[2], 0,1,0,0,0,1,1,0);
				if(ia==-1||ib==-1){
					return false;
				}  
				if(m==8){
					qc.relationType=RT_NEXTT;
				}else if(m==9){
					qc.relationType=RT_NEXT;
				}
				qc.variable1 = ia;
				qc.variable2 = ib;
			}else if(m==10||m==11){
				//================================
				//Relation is "Calls*", "Calls"
				//
				//
				//var1 must be entRef
				//var2 must be entRef
				//var1 must be procedure or '_'
				//var2 must be procedure or '_'
				//cannot be the same (no recusive), except when '_'
				//================================ 
				if(!qv.isEntRef(element[1])||!qv.isEntRef(element[2])){
					cout<<"@setupClaTable(): Variable not right for Calls. a:["<<element[1]<<"] b:["<<element[2]<<"]"<<endl;
					return false;
				}
				int ia = getIndexFromVarTable(element[1], 0,1,0,0,1,1,0,3);
				int ib = getIndexFromVarTable(element[2], 0,1,0,0,1,1,0,3);
				if(ia==-1||ib==-1){
					return false;
				}  
				if(m==10){
					qc.relationType=RT_CALLST;
				}else if(m==11){
					qc.relationType=RT_CALLS;
				}
				qc.variable1 = ia;
				qc.variable2 = ib;
			}else{
				cout<<"@setupClaTable(): Unknow relation. a:["<<element[0]<<endl;
			} 
			queryClaTable.push_back(qc);
		}
	}
	return true;
} 
/********************************************//**
 * @brief 
 * @details
 * Process: \n
 *		1. merge same variables with p1.procName=p2.procName \n
 *		2. count appearance \n
 *		3. set dependency \n
 * Philosophy: \n
 *		
 *		
 * @todo merge
 * @todo The judging of dependency is bot too simple, needs to improve
 * @return The complitance of this function, true for done, false for error
 ***********************************************/
bool QueryPreprocessor::setDependency(){
	if(queryVarTable.size()==0||queryTarTable.size()<0){
		return false;
	}


	//set appearer count
	for(int i = 0; i<(int)queryVarTable.size(); i++){ 
		for(int j = 0; j<(int)queryClaTable.size(); j++){
			if(queryClaTable[j].variable1==i||queryClaTable[j].variable2==i){
				queryVarTable[i].countAppear++;
			}
		}
	}
	for(int i = 0; i<(int)queryVarTable.size(); i++){ 
		if((queryVarTable[i].variableType==KT_STMT_NUM)||(queryVarTable[i].variableType==DT_UNDERSCORE)||(queryVarTable[i].variableType==KT_KNOWN_VARIABLE)||(queryVarTable[i].variableType==KT_KNOWN_PROCEDURE)){
			continue;
		}else if(queryVarTable[i].countAppear>1&&queryVarTable[i].dependency==-1){
			queryVarTable[i].dependency=i;
		}
	}
	//set variables that has dependency
	for(int i = 0; i<(int)queryVarTable.size(); i++){ 
		for(int j = 0; j<(int)queryClaTable.size(); j++){
			int a = queryClaTable[j].variable1;
			int b = queryClaTable[j].variable2;
			if(i==a||i==b){
				if(queryVarTable[a].dependency!=-1&&queryVarTable[b].dependency!=-1){
					queryVarTable[a].dependency=a;
					queryVarTable[b].dependency=a;
				}
			}
		} 
	}
	return true;
}
/********************************************//**
 * @param query string free format, can be anything 
 * @todo comfirm the return type of BOOLEAN
 * @return
 *		0	suceed	definitly success
 *		1	false	if the boolean needs to return false instead of none
 *		-1	error	just not pass the validation
 ***********************************************/
//    called by the query processor
//
//
int QueryPreprocessor::parse(string query){
	queryVarTable.clear();
	queryTarTable.clear();
	queryClaTable.clear();
	trim_all(query);
	mergeFlag.clear();
	while(true){
		//=======================================>>Stage 1 Declare
		vector<string> declares = getDeclares(query);
		if(declares.size()<=0){
			break;
		} 
		if(!setupVarTable(declares)){
			break;
		}
		//=======================================>>Stage 2 Select
		vector<string> targets = getTargets(query);
		if(targets.size()<=0){
			break;
		}
		if(!setupTarTable(targets)){
			break;
		}
		//=======================================>>Stage 3 Clause
		vector<string> clauses = getClauses(query); 
		if(clauses.size()<=0){
			break;
		}
		cout<<endl;
		if(!setupClaTable(clauses)){
			break;
		}
		//=======================================>>Stage 4 Dependency
		if(!setDependency()){
			break;
		}
		cout<<"@parse: succeed!\n"<<endl;
		return 0;
	}
	cout<<"@parse: return false, interrupted!"<<endl;
	return -1;
} 
/********************************************//**
 *  getter for queryVarTable 
 * @todo confirm is it better to pass by pointer?
 ***********************************************/
vector<QueryVariable> QueryPreprocessor::getQueryVariableTable(){
	return queryVarTable;
}
/********************************************//**
 *  getter for queryTarTable 
 * @todo confirm is it better to pass by pointer?
 ***********************************************/
vector<QueryTarget> QueryPreprocessor::getQueryTargetTable(){
	return queryTarTable;
}
/********************************************//**
 *  getter for queryClaTable 
 * @todo confirm is it better to pass by pointer?
 ***********************************************/
vector<QueryClause> QueryPreprocessor::getQueryClauseTable(){
	return queryClaTable;
}

