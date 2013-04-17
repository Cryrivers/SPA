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
	dicDesignEntity[10] ="plus ";
	dicDesignEntity[11] ="minus ";
	dicDesignEntity[12] ="times "; 		 
	dicRelationRef[0] = "Parent*";
	dicRelationRef[1] = "Parent";
	dicRelationRef[2] = "Follows*";
	dicRelationRef[3] = "Follows";
	dicRelationRef[4] = "Modifies";
	dicRelationRef[5] = "Uses";	   
	dicRelationRef[6] = "AffectsBip*";
	dicRelationRef[7] = "AffectsBip";
	dicRelationRef[8] = "Affects*";
	dicRelationRef[9] = "Affects";	  
	dicRelationRef[10] = "NextBip*";
	dicRelationRef[11] = "NextBip";
	dicRelationRef[12] = "Next*";
	dicRelationRef[13] = "Next";
	dicRelationRef[14] = "Calls*"; 
	dicRelationRef[15] = "Calls";
	dicRelationRef[16] = "Contains*";
	dicRelationRef[17] = "Contains";
	dicRelationRef[18] = "Sibling";	
	dicAttribute[0] = ".procName";
	dicAttribute[1] = ".varName";
	dicAttribute[2] = ".value";
	dicAttribute[3] = ".stmt#";
	dicAttribute[4] = ".prog_line#";
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
		if(DEBUGMODE) cout<<"@getTargets: Cannot find select clause. Got["<<str<<"]. "<<endl;
		return result;
	}
	str = str.substr(7);
	//split select
	int next = findNextIndexFrom(str, dicClause, 4);
	if(next == -1){
		if(DEBUGMODE) cout<<"@getTargets: No clause. Got["<<str<<"]. "<<endl;
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
				if(DEBUGMODE) cout<<"@getClauses: No previous deaclearation for and, got ["<<local<<"].";
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
		if(dicAttribute[a]==".procName"&&(queryVarTable[b].variableType==DT_PROCEDURE||queryVarTable[b].variableType==KT_KNOWN_PROCEDURE)){
			return AT_PROC_NAME;
		}else if(dicAttribute[a]==".procName"&&queryVarTable[b].variableType==DT_CALL){
			return AT_CALL_PROC_NAME;
		}else if(dicAttribute[a]==".varName"&&(queryVarTable[b].variableType==DT_VARIABLE||queryVarTable[b].variableType==KT_KNOWN_VARIABLE)){
			return AT_VAR_NAME;
		}else if(dicAttribute[a]==".value"&&(queryVarTable[b].variableType==KT_KNOWN_CONSTANT||queryVarTable[b].variableType==DT_CONSTANT)){
			return AT_VALUE;
		}else if(dicAttribute[a]==".stmt#"&&(queryVarTable[b].variableType==DT_STMT||queryVarTable[b].variableType==DT_ASSIGN||queryVarTable[b].variableType==DT_CALL||queryVarTable[b].variableType==DT_WHILE||queryVarTable[b].variableType==DT_IF||queryVarTable[b].variableType==KT_STMT_NUM)){
			return AT_STMT_NUM;
		}else{  
			//if(DEBUGMODE) cout<<"@getAttributeOfVariable: Not match. Got["<<b<<"] ["<<dicAttribute[a]<<"]"<<endl;
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
					}else if(queryVarTable[i].variableType==KT_STMT_NUM){
						return i;
					}else if(queryVarTable[i].variableType==DT_UNDERSCORE){
						return i;
					}else if(queryVarTable[i].variableType==DT_PROGLINE){
						return i;
					}else{
						if(DEBUGMODE) cout<<"@getIndexFromVarTable: Query variable ["<<str<<"] not stmt."<<endl;
						return -1;
					}
				}else if(a7==2){
					if(queryVarTable[i].variableType==DT_VARIABLE){
						return i;
					}else if(queryVarTable[i].variableType==KT_KNOWN_VARIABLE){
						return i;
					}else if(queryVarTable[i].variableType==DT_UNDERSCORE){
						return i;
					}else{
						if(DEBUGMODE) cout<<"@getIndexFromVarTable: Query variable ["<<str<<"] not var."<<endl;
						return -1;
					}
				}else if(a7==3){
					if(queryVarTable[i].variableType==DT_PROCEDURE){
						return i;
					}else if(queryVarTable[i].variableType==KT_KNOWN_PROCEDURE){
						return i;
					}else if(queryVarTable[i].variableType==DT_UNDERSCORE){
						return i;
					}else{
						if(DEBUGMODE) cout<<"@getIndexFromVarTable: Query variable ["<<str<<"] not var."<<endl;
						return -1;
					}
				}else if(a7==4){
					if(queryVarTable[i].variableType==DT_STMTLST){
						return i;
					}else if(queryVarTable[i].variableType==DT_UNDERSCORE){
						return i;
					}else{
						if(DEBUGMODE) cout<<"@getIndexFromVarTable: Query variable ["<<str<<"] not var."<<endl;
						return -1;
					}
				}else if(a7==5){
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
					}else if(queryVarTable[i].variableType==DT_PROCEDURE){
						return i;
					}else if(queryVarTable[i].variableType==DT_VARIABLE){
						return i;
					}else if(queryVarTable[i].variableType==DT_PROGLINE){
						return i;
					}else if(queryVarTable[i].variableType==DT_STMTLST){
						return i;
					}else if(queryVarTable[i].variableType==DT_CONSTANT){
						return i;
					}else{
						if(DEBUGMODE) cout<<"@getIndexFromVarTable: Query variable ["<<str<<"] not stmt."<<endl;
						return -1;
					}
				}else{
					return i;
				}
			}
		}
	}
	if((str=="BOOLEAN"||str=="boolean")&&a2!=0){ 
		QueryVariable var; 
		var.variableType=DT_BOOLEAN;
		var.dependency=-1;
		var.content=-1;
		var.origin=str;
		var.countAppear=0;
		var.mapTo=size;
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
				var.mapTo=size;
				queryVarTable.push_back(var); 
				return size;
			}else if(a4!=0&&inP>=0){ 
				var.variableType=KT_KNOWN_PROCEDURE; 
				var.content=inP;
				var.origin=str;
				var.dependency=-1; 
				var.countAppear=0;
				var.mapTo=size;
				queryVarTable.push_back(var); 
				return size;
			}else{
				//if(DEBUGMODE) cout<<"@getIndexFromVarTable: Query variable ["<<str<<"] not in pkb."<<a3<<";"<<inV<<";"<<a4<<";"<<inP<<endl;
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
		var.mapTo=queryVarTable.size();
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
		var.mapTo=queryVarTable.size();
		queryVarTable.push_back(var);
		return size;
	}
	if(a0==0){
		//if(DEBUGMODE) cout<<"@getIndexFromVarTable: Variable ["<<str<<"] not declared."<<endl;
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
BOOLEAN QueryPreprocessor::setupVarTable(vector<string> declares){ 
	for(unsigned int i=0; i<declares.size(); i++){
		int p = getIndexOfFirstFrom(declares[i], dicDesignEntity, DICDESIGNENTITYSIZE); 
		if(p==-1){
			//================================
			//The declares is not in the design entity dictionar
			//================================
			if(DEBUGMODE) cout<<"@setupVarTable(): Design entity type not found, recieve:["<<declares[i]<<"]"<<endl;
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
				var=DT_PROGLINE;
			}else if(dicDesignEntity[p]=="stmtLst "){
				var=DT_STMTLST;
			}else if(dicDesignEntity[p]=="procedure "){
				var=DT_PROCEDURE;
			}else if(dicDesignEntity[p]=="plus "){
				var=DT_PLUS;
			}else if(dicDesignEntity[p]=="minus "){
				var=DT_MINUS;
			}else if(dicDesignEntity[p]=="times "){
				var=DT_TIMES;
			}else{
				if(DEBUGMODE) cout<<"@setupVarTable: Design entity not exist. Got:["<<declares[i]<<"]"<<endl;
				return false;
			}
			string sub=declares[i].substr(declares[i].find(" "));
			trim_all(sub);
			if(sub.find(",")>sub.size()){ 
				//================================
				//Only one variable is declared
				//================================
				if(!qv.isSynonym(sub)){
					if(DEBUGMODE) cout<<"@setupVarTable: Declared variable is not a synonym, recieve:["<<sub<<"]"<<endl;
					return false;
				}
				int index = getIndexFromVarTable(sub, 1,0,0,0,0,0,0,0);
				if(index!=-1){
					if(DEBUGMODE) cout<<"@setupVarTable: Duplicate variable at index"<<index<<", recieve:["<<sub<<"]"<<endl;
					return false;
				}  
				QueryVariable newvar;
				newvar.variableType=var;
				newvar.dependency=-1;
				newvar.content=-1;
				newvar.origin=sub; 
				newvar.countAppear=0;
				newvar.mapTo=queryVarTable.size();
				queryVarTable.push_back(newvar);
			}else{
				//================================
				//More than one variable is declared
				//================================
				while(sub.find(",")<sub.size()){  
					string front = sub.substr(0, sub.find(","));
					trim_all(front); 
					if(!qv.isSynonym(front)){
						if(DEBUGMODE) cout<<"@setupVarTable: Declared variable is not a synonym, recieve:["<<front<<"]"<<endl;
						return false;
					} 
					int index = getIndexFromVarTable(front, 1,0,0,0,0,0,0,0);
					if(index!=-1){
						if(DEBUGMODE) cout<<"@setupVarTable: Dupulicate variable at index"<<index<<", recieve:["<<front<<"]"<<endl;
						return false;
					}
					QueryVariable newvar;
					newvar.variableType=var;
					newvar.dependency=-1;
					newvar.content=-1;
					newvar.origin=front;
					newvar.countAppear=0;
					newvar.mapTo=queryVarTable.size();
					queryVarTable.push_back(newvar);
					sub = sub.substr(sub.find(",")+1);
					trim_all(sub);
				} 
				if(!qv.isSynonym(sub)){
					if(DEBUGMODE) cout<<"@setupVarTable: Declared variable is not a synonym, recieve:["<<sub<<"]"<<endl;
					return false;
				}
				int index = getIndexFromVarTable(sub, 1,0,0,0,0,0,0,0);
				if(index!=-1){
					if(DEBUGMODE) cout<<"@setupVarTable: Dupulicate variable at index"<<index<<", recieve:["<<sub<<"]"<<endl;
					return false;
				}
				QueryVariable newvar;
				newvar.variableType=var;
				newvar.dependency=-1;
				newvar.origin=sub;
				newvar.content=-1;
				newvar.countAppear=0;	 
				newvar.mapTo=queryVarTable.size();
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
BOOLEAN QueryPreprocessor::setupTarTable(vector<string> tarTable){
	for(unsigned int i = 0; i<tarTable.size(); i++){   
		if(tarTable[i].find(".")<tarTable[i].size()){
			//================================
			//If the target contains attribute
			//================================
			string attrib = tarTable[i].substr(tarTable[i].find("."));
			tarTable[i] = tarTable[i].substr(0, tarTable[i].find("."));
			int index = getIndexFromVarTable(tarTable[i], 0,1,1,0,0,0,0,5);
			if(index==-1){
				if(DEBUGMODE) cout<<"@setupTarTable: Variable not declared. Got["<<tarTable[i]<<"]"<<endl;
				return false;
			}
			QueryTarget newTarget;
			newTarget.varIndex=index;
			newTarget.hasAttribute=true;
			int attribu = getIndexOfNextFrom(attrib, dicAttribute, DICATTRIBUTESIZE);
			if(attribu == -1){
				if(DEBUGMODE) cout<<"@setupTarTable: Attribute not exist. Got["<<tarTable[i]<<"]"<<endl;
				return false;
			}  

			Type newType= getAttributeOfVariable(index, attribu);
			if(newType==ERROR){
				return false;
			}
			newTarget.attributeType=newType;	   
			queryTarTable.push_back(newTarget);
		}else{
			//================================
			//Single target
			//================================
			int index = getIndexFromVarTable(tarTable[i], 0,1,1,0,0,0,0,5);
			if(index==-1){
				if(DEBUGMODE) cout<<"@setupTarTable: Variable not declared. Got["<<tarTable[i]<<"]"<<endl;
				return false;
			}
			QueryTarget newTarget;
			newTarget.varIndex=index;
			newTarget.hasAttribute=false;			 
			queryTarTable.push_back(newTarget);
		} 
	}
	return true;
} 
/********************************************//** 
 * Unify the mapTo field of QueryVariable
 ***********************************************/ 
void QueryPreprocessor::changeMapTo(int from, int to){
	  for(int i=0; i<queryVarTable.size(); i++){
		  if(queryVarTable[i].mapTo==from){
			 queryVarTable[i].mapTo=to; 
		  }
	  }
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
BOOLEAN QueryPreprocessor::setupClaTable(vector<string> claTable){ 
	for(unsigned int i=0; i<claTable.size(); i++){
		//if(DEBUGMODE) cout<<queryClaTable.size()<<"  "<<claTable[i]<<endl;
		if(claTable[i].find("with ")==0){
			claTable[i] = claTable[i].substr(5);
		//split left and right
			string front = claTable[i].substr(0, claTable[i].find("="));
			string back = claTable[i].substr(claTable[i].find("=")+1);
			trim_all(front);	trim_all(back);
		//create new query variable
			QueryClause qc;
			qc.index=queryClaTable.size();
			qc.relationType=CT_WITH;
			if(qv.isRef(front)&&qv.isRef(back)){
				int frontAttribute = getIndexOfNextFrom(front, dicAttribute, DICATTRIBUTESIZE); 
				int backAttribute = getIndexOfNextFrom(back, dicAttribute, DICATTRIBUTESIZE);  
		//Both have attribute
				if(frontAttribute != -1 && backAttribute != -1){
					front = front.substr(0, front.find("."));
					back = back.substr(0, back.find("."));
					int ia = getIndexFromVarTable(front, 0,1,0,0,0,0,0,0);
					int ib = getIndexFromVarTable(back, 0,1,0,0,0,0,0,0);
			//all ia ib is the real one!!
					if(ia==-1||ib==-1)	return false;  	  
					ia = queryVarTable[ia].mapTo;
					ib = queryVarTable[ib].mapTo;
					qc.variable1 = ia; 
					qc.variable2 = ib;
					Type ta = getAttributeOfVariable(ia, frontAttribute);
					Type tb = getAttributeOfVariable(ib, backAttribute);
					if(ta==ERROR||tb==ERROR)	return false; 
					qc.attribute1 = ta;
					qc.attribute2 = tb;

			//Attribute	the same
					if(ta==tb){	
						if(ta==AT_STMT_NUM){
							if(queryVarTable[ia].variableType==queryVarTable[ib].variableType){
								changeMapTo(ia, ib);
							}else if(queryVarTable[ia].variableType==DT_STMT&&queryVarTable[ib].variableType!=DT_STMT){
								changeMapTo(ia, ib);
							}else if(queryVarTable[ia].variableType!=DT_STMT&&queryVarTable[ib].variableType==DT_STMT){
								changeMapTo(ib, ia);
							}else
								return false;
						}else{ 	 
							if(queryVarTable[ia].variableType==KT_STMT_NUM||queryVarTable[ia].variableType==KT_KNOWN_VARIABLE||queryVarTable[ia].variableType==KT_KNOWN_PROCEDURE||queryVarTable[ia].variableType==KT_KNOWN_CONSTANT){
								changeMapTo(ib, ia);
							}else{
								changeMapTo(ia, ib);
							}
						}
					}else{
			//Attribute not the same but comparable
						if((ta==AT_PROC_NAME||ta==AT_CALL_PROC_NAME||ta==AT_VAR_NAME)&&(tb==AT_PROC_NAME||tb==AT_CALL_PROC_NAME||tb==AT_VAR_NAME)){

						}else if((ta==AT_STMT_NUM||ta==AT_VALUE)&&(tb==AT_STMT_NUM||tb==AT_VALUE)){

						}else{
							return false;
						}
							
					} 
					queryClaTable.push_back(qc);   
					continue;
				}
		//only one of it has attribute
				else if((frontAttribute != -1 && backAttribute == -1)||(frontAttribute == -1 && backAttribute != -1)){
			//swap front and back to a.b=c
					if(frontAttribute == -1 && backAttribute != -1){
						int tmpt = frontAttribute;
						frontAttribute = backAttribute;
						backAttribute = tmpt;
						string tmp = front;
						front = back;
						back = tmp;	 
					}
					front = front.substr(0, front.find(".")); 
					int ia = getIndexFromVarTable(front, 0,1,0,0,0,0,0,0); 	
					int ib = getIndexFromVarTable(back, 0,1,0,0,0,0,0,0);
					if(ia==-1)	return false;
					ia = queryVarTable[ia].mapTo;
					if(ib!=-1){
			//progline
					   if(queryVarTable[queryVarTable[ib].mapTo].content!=-1){
							string intStr = static_cast<ostringstream*>( &(ostringstream() << queryVarTable[queryVarTable[ia].mapTo].content) )->str();
							back=string(string(intStr));
					   }else{
							changeMapTo(queryVarTable[ib].mapTo, ia);
					   }
					}
			//front attrib match back type
					if(qv.isInteger(back)){
						if(getAttributeOfVariable(ia, frontAttribute)==AT_STMT_NUM){
							queryVarTable[ia].content=atoi(back.c_str());
					//check if that stmt has type
							if(queryVarTable[ia].variableType==DT_ASSIGN){
								STMT_LIST st;
								pkb->getAllAssignment(&st);
								int cou=0;
								for(cou; cou<st.size(); cou++){					 
									if(st[cou]==queryVarTable[ia].content){
										 queryVarTable[ia].variableType=KT_STMT_NUM;
										 break;
									}
								}
								if(cou==st.size())
									return false;
							}else if(queryVarTable[ia].variableType==DT_CALL){ 
								STMT_LIST st;
								pkb->getAllCall(&st);
								int cou=0;
								for(cou; cou<st.size(); cou++){					 
									if(st[cou]==queryVarTable[ia].content){
										 queryVarTable[ia].variableType=KT_STMT_NUM;
										 break;
									}
								}
								if(cou==st.size())
									return false;
							}else if(queryVarTable[ia].variableType==DT_WHILE){	
								STMT_LIST st;			  
								pkb->getAllWhile(&st);
								int cou=0;
								for(cou; cou<st.size(); cou++){					 
									if(st[cou]==queryVarTable[ia].content){
										 queryVarTable[ia].variableType=KT_STMT_NUM;
										 break;
									}
								}
								if(cou==st.size())
									return false;
							}else if(queryVarTable[ia].variableType==DT_IF){  
								STMT_LIST st;
								pkb->getAllIf(&st);
								int cou=0;
								for(cou; cou<st.size(); cou++){					 
									if(st[cou]==queryVarTable[ia].content){
										 queryVarTable[ia].variableType=KT_STMT_NUM;
										 break;
									}
								}
								if(cou==st.size())
									return false;
							}else if(queryVarTable[ia].variableType==DT_STMT){
								STMT_LIST st;
								pkb->getAllStmt(&st);
								int cou=0;
								for(cou; cou<st.size(); cou++){					 
									if(st[cou]==queryVarTable[ia].content){			 
										queryVarTable[ia].variableType=KT_STMT_NUM;
										break;
									}
								}
								if(cou==st.size())
									return false;
							} else{
								cout<<"This should really not happen!"<<endl;
								return false;
							}
						}else if(getAttributeOfVariable(ia, frontAttribute)==AT_VALUE){	 
								queryVarTable[ia].content=atoi(back.c_str());   
								STMT_LIST st;
								pkb->getAllConstant(&st);
								int cou=0;
								for(cou; cou<st.size(); cou++){					 
									if(st[cou]==queryVarTable[ia].content){
										 queryVarTable[ia].variableType=KT_KNOWN_CONSTANT;
										 break;
									}
								}
								if(cou==st.size())
									return false;				  
						}else
							return false;  
						discardClause++;  
						continue;
					}else if(back.find("\"")<back.size()){
						back = back.substr(back.find("\"")+1);
						if(back.find("\"")<=back.size()){
							back = back.substr(0,back.find("\""));
							if(qv.isSynonym(back)){	
								Type frontAttr=getAttributeOfVariable(ia, frontAttribute);
								if(frontAttr==ERROR) return false;
								if(frontAttr==AT_VAR_NAME){ 	  
									int inV = pkb->getVarIndex(back);
									if(inV<0) return false;  
									queryVarTable[ia].variableType=KT_KNOWN_VARIABLE; 	
									queryVarTable[ia].content=inV; 
								}else if(frontAttr==AT_PROC_NAME&&queryVarTable[ia].variableType==DT_CALL){
									int inP = pkb->getProcIndex(back);
									if(inP<0) return false;	   
									QueryVariable var;
									var.variableType=KT_KNOWN_PROCEDURE;
									var.content=inP;
									var.origin=back;
									var.countAppear=0;
									var.dependency=-1;
									var.mapTo=queryVarTable.size();
									queryVarTable.push_back(var);

									qc.variable1=ia;
									qc.attribute1=AT_CALL_PROC_NAME;
									qc.variable2=queryVarTable.size()-1;
									qc.attribute2=AT_PROCTABLEINDEX;
									queryClaTable.push_back(qc);
									continue;
								}else if(frontAttr==AT_PROC_NAME){	 
									int inP = pkb->getProcIndex(back);
									if(inP<0) return false; 
									queryVarTable[ia].variableType=KT_KNOWN_PROCEDURE; 
									queryVarTable[ia].content=inP; 
								}
							}
						}													
					}else{
							 return false;
					}
					discardClause++;
					continue;
				}else if(frontAttribute == -1 && backAttribute == -1){
					if(front!=back){
						int ia = getIndexFromVarTable(front, 0,1,0,0,0,0,0,0); 	
						int ib = getIndexFromVarTable(back, 0,1,0,0,0,0,0,0);
						if(ia==-1||ib==-1)	return false; 
						if(queryVarTable[ia].variableType!=DT_PROGLINE&&queryVarTable[ia].variableType!=KT_STMT_NUM)
							return false;
						if(queryVarTable[ib].variableType!=DT_PROGLINE&&queryVarTable[ib].variableType!=KT_STMT_NUM)
							return false;
						if(queryVarTable[ia].mapTo!=ia)
							changeMapTo(queryVarTable[ib].mapTo, queryVarTable[ia].mapTo);
						else
							changeMapTo(queryVarTable[ia].mapTo, queryVarTable[ib].mapTo);
					}
					discardClause++; 
					continue;
				}else{
					if(DEBUGMODE) cout<<"@setupClaTable: Unkown ref! Got["<<front<<"], ["<<back<<"]"<<endl;
					return false;
				}
			}else{
				if(DEBUGMODE) cout<<"@setupClaTable: with clause fromat wrong. Got[with "<<front<<"="<<back<<"]"<<endl;
				return false;
			}
		}else if(claTable[i].find("pattern ")==0){
			claTable[i] = claTable[i].substr(8);
			vector<string> element = extractRelation(claTable[i]);
			int ia = getIndexFromVarTable(element[0], 0,1,0,0,0,0,0,1);
			if(ia==-1)
				return false;
			ia = queryVarTable[ia].mapTo; 
			QueryClause qc;
			qc.index=queryClaTable.size();
			if(queryVarTable[ia].variableType==DT_ASSIGN){
				if(!qv.isVarRef(element[1]))	
					return false;
				int ib = getIndexFromVarTable(element[1], 0,1,0,1,0,1,0,2);
				if(ib==-1)
					return false;
				ib = queryVarTable[ib].mapTo;
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
				continue;
			}else if(queryVarTable[ia].variableType==DT_WHILE){
				if(!qv.isVarRef(element[1]))	return false;
				int ib = getIndexFromVarTable(element[1], 0,1,0,1,0,1,0,2);
				if(ib==-1){
					return false;
				} 	
				int stmlst = getIndexFromVarTable(element[2], 0,1,0,1,0,1,0,4);
				if(stmlst==-1){
					return false;
				}
				ib = queryVarTable[ib].mapTo;
				qc.relationType=CT_PATTERN;
				qc.variable1 = ia;
				qc.variable2 = ib;
				qc.patternType=PATTERN_WHILE;
				qc.variable3 = "";	   
				qc.variable4 = stmlst;
				qc.variable5 = stmlst;
				queryClaTable.push_back(qc); 
				continue;
			}else if(queryVarTable[ia].variableType==DT_IF){
				if(!qv.isVarRef(element[1]))	return false;
				int ib = getIndexFromVarTable(element[1], 0,1,0,1,0,1,0,2);
				if(ib==-1)	return false;		
																		  
				string var1 = element[2].substr(0,element[2].find(","));
				string var2 = element[2].substr(element[2].find(",")+1);	 
				trim_all(var1);
				trim_all(var2);
							
				int stmlst1 = getIndexFromVarTable(var1, 0,1,0,1,0,1,0,4);   
				int stmlst2 = getIndexFromVarTable(var2, 0,1,0,1,0,1,0,4);
												   
				if(stmlst1==-1||stmlst2==-1){
					return false;
				}
				if(queryVarTable[stmlst1].variableType!=DT_UNDERSCORE&&stmlst1==stmlst2){
					return false;
				}
				ib = queryVarTable[ib].mapTo;
				qc.relationType=CT_PATTERN;
				qc.variable1 = ia;
				qc.variable2 = ib;
				qc.variable3 = ""; 	  
				qc.variable4 = stmlst1;
				qc.variable5 = stmlst2;
				qc.patternType=PATTERN_IF;
				queryClaTable.push_back(qc);
				continue;
			}else{
				if(DEBUGMODE) cout<<"@setupClaTable(): Pattern has to work on assign,if,while Got:"<<queryVarTable[ia].variableType<<endl;
				return false;
			}
		}else{
			//================================
			//If the clauses is "such that "
			//================================
			claTable[i] = claTable[i].substr(10); 
			vector<string> element = extractRelation(claTable[i]);
			if(element.size()!=3){
				if(DEBUGMODE) cout<<"@setupClaTable: the format of relation is wrong, recieve: ["<<claTable[i]<<"]"<<endl;
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
				if(element[1]==element[2]&&element[1]!="_"){
					if(DEBUGMODE) cout<<"@setupClaTable(): Variable cannot be the same for Parent and Follows a:["<<element[1]<<"] b:["<<element[2]<<"]"<<endl;
					return false;
				}
				if(!qv.isStmtRef(element[1])||!qv.isStmtRef(element[2])){
					if(DEBUGMODE) cout<<"@setupClaTable(): Variable not right for Parent, Follows: a:["<<element[1]<<"] b:["<<element[2]<<"]"<<endl;
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
					if(DEBUGMODE) cout<<"@setupClaTable(): Variable not exist: a:["<<element[1]<<"] b:["<<element[2]<<"]"<<endl;
					return false;
				}  
				if(m==4){
					if(queryVarTable[ia].variableType==DT_PROCEDURE||queryVarTable[ia].variableType==KT_KNOWN_PROCEDURE){
						qc.relationType=RT_MODIFIESP;
					}else if(queryVarTable[ia].variableType!=DT_VARIABLE&&queryVarTable[ia].variableType!=KT_KNOWN_VARIABLE){
						qc.relationType=RT_MODIFIESS;
					}else{
						if(DEBUGMODE) cout<<"@setupClaTable(): Var1 of Modifies cannot be variable"<<endl;
						return false;
					}
				}else if(m==5){
					if(queryVarTable[ia].variableType==DT_PROCEDURE||queryVarTable[ia].variableType==KT_KNOWN_PROCEDURE){
						qc.relationType=RT_USESP;
					}else if(queryVarTable[ia].variableType!=DT_VARIABLE&&queryVarTable[ia].variableType!=KT_KNOWN_VARIABLE){
						qc.relationType=RT_USESS;
					}else{
						if(DEBUGMODE) cout<<"@setupClaTable(): Var1 of Modifies cannot be variable"<<endl;
						return false;
					}
				}
				qc.variable1 = ia;
				qc.variable2 = ib;
			}else if(m==6||m==7||m==8||m==9){
				//================================
				//Relation is "Affects*", "Affects", "AffectsBip*", "AffectsBip"
				//
				// 
				//var1 must be stmtRef
				//var2 must be stmtRef
				//var1 must be assign or '_'
				//var2 must be assign or '_'
				//cannot be the same, except when '_'
				//integer argument means statement number
				//================================ 
				if(!qv.isStmtRef(element[1])||!qv.isStmtRef(element[2])){
					if(DEBUGMODE) cout<<"@setupClaTable(): Variable right for Affect. a:["<<element[1]<<"] b:["<<element[2]<<"]"<<endl;
					return false;
				}
				int ia = getIndexFromVarTable(element[1], 0,1,0,0,0,1,1,1);
				int ib = getIndexFromVarTable(element[2], 0,1,0,0,0,1,1,1);
				if(ia==-1||ib==-1){
					 cout<<"@"<<endl;
					return false;
				} 
				if(queryVarTable[ia].variableType!=DT_ASSIGN&&queryVarTable[ia].variableType!=DT_UNDERSCORE&&queryVarTable[ia].variableType!=KT_STMT_NUM){
					if(DEBUGMODE) cout<<"@setupClaTable(): Variable 1 not assign: ["<<ia<<"]"<<endl;
					return false;
				} else if (queryVarTable[ia].variableType==KT_STMT_NUM){
					STMT_LIST st;
					pkb->getAllAssignment(&st);
					int cou=0;
					for(cou; cou<st.size(); cou++){					 
						if(st[cou]==queryVarTable[ia].content){			 
							break;
						}
					}
					if(cou==st.size()) {
						if(DEBUGMODE) cout<<"@setupClaTable(): Variable 1 not assign: ["<<ia<<"]"<<endl;
						return false;
					}
				}
				if(queryVarTable[ib].variableType!=DT_ASSIGN&&queryVarTable[ib].variableType!=DT_UNDERSCORE&&queryVarTable[ib].variableType!=KT_STMT_NUM){
					if(DEBUGMODE) cout<<"@setupClaTable(): Variable 2 not assign: ["<<ib<<"]"<<endl;
					return false;
				}  else if (queryVarTable[ib].variableType==KT_STMT_NUM){
					STMT_LIST st;
					pkb->getAllAssignment(&st);
					int cou=0;
					for(cou; cou<st.size(); cou++){					 
						if(st[cou]==queryVarTable[ib].content){			 
							break;
						}
					}
					if(cou==st.size()) {
						if(DEBUGMODE) cout<<"@setupClaTable(): Variable 1 not assign: ["<<ib<<"]"<<endl;
						return false;
					}
				}
				if(m==8){
					qc.relationType=RT_AFFECTST;
				}else if(m==9){
					qc.relationType=RT_AFFECTS;
				}else if(m==6){
					qc.relationType=RT_AFFECTSBIPT;
				}else if(m==7){
					qc.relationType=RT_AFFECTSBIP;
				}
				qc.variable1 = ia;
				qc.variable2 = ib;	   
			}else if(m==10||m==11||m==12||m==13){
				//================================
				//Relation is "Next*", "Next", "NextBip*", "NextBip"
				//
				//
				//var1 must be lineRef
				//var2 must be lineRef
				//var1 must be stmt or stmt number or '_'
				//var2 must be stmt or stmt number or '_'
				//integer arguments mean program line numbers
				//================================ 
				if(element[1]==element[2]&&element[1]!="_"&&(m==9||m==18)){
					if(DEBUGMODE) cout<<"@setupClaTable(): Variable cannot be the same for Parent and Follows a:["<<element[1]<<"] b:["<<element[2]<<"]"<<endl;
					return false;
				}
				if(!qv.isLineRef(element[1])||!qv.isLineRef(element[2])){
					if(DEBUGMODE) cout<<"@setupClaTable(): Variable not right for Next. a:["<<element[1]<<"] b:["<<element[2]<<"]"<<endl;
					return false;
				}
				int ia = getIndexFromVarTable(element[1], 0,1,0,0,0,1,1,0);
				int ib = getIndexFromVarTable(element[2], 0,1,0,0,0,1,1,0);
				if(ia==-1||ib==-1){
					return false;
				}  
				if(m==12){
					qc.relationType=RT_NEXTT;
				}else if(m==13){
					qc.relationType=RT_NEXT;
				}else if(m==10){
					qc.relationType=RT_NEXTBIPT;
				}else if(m==11){
					qc.relationType=RT_NEXTBIP;
				}
				qc.variable1 = ia;
				qc.variable2 = ib;
			}else if(m==14||m==15){
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
					if(DEBUGMODE) cout<<"@setupClaTable(): Variable not right for Calls. a:["<<element[1]<<"] b:["<<element[2]<<"]"<<endl;
					return false;
				}
				int ia = getIndexFromVarTable(element[1], 0,1,0,0,1,1,0,3);
				int ib = getIndexFromVarTable(element[2], 0,1,0,0,1,1,0,3);
				if(ia==-1||ib==-1){
					return false;
				}  
				if(m==14){
					qc.relationType=RT_CALLST;
				}else if(m==15){
					qc.relationType=RT_CALLS;
				}
				qc.variable1 = ia;
				qc.variable2 = ib;
			}else if(m==16||m==17||m==18){
				//================================
				//Relation is "Contains*", "Contains", "Sibling"
				//
				//
				//var1 must be nodeRef
				//var2 must be nodeRef
				//var1 must be stmt or stmt num
				//var2 must be stmt or stmt num
				//cannot be the same 
				//integer argument means statement number
				//================================ 
				if(element[1]==element[2]){
					if(DEBUGMODE) cout<<"@setupClaTable(): Variable cannot be the same for Contain and Sibling a:["<<element[1]<<"] b:["<<element[2]<<"]"<<endl;
					return false;
				}
				if(!qv.isNodeRef(element[1])||!qv.isNodeRef(element[2])){
					if(DEBUGMODE) cout<<"@setupClaTable(): Variable not right for Contain and Sibling: a:["<<element[1]<<"] b:["<<element[2]<<"]"<<endl;
					return false;
				}
				int ia = getIndexFromVarTable(element[1], 0,1,0,0,0,0,1,0);
				int ib = getIndexFromVarTable(element[2], 0,1,0,0,0,0,1,0);
				if(ia==-1||ib==-1){
					return false;
				}  
				if(m==16){
					qc.relationType=RT_CONTAINST;
				}else if(m==17){
					qc.relationType=RT_CONTAINS;
				}else if(m==18){
					qc.relationType=RT_SIBLING;
				} 
				qc.variable1 = ia;
				qc.variable2 = ib;
			}else{
				if(DEBUGMODE) cout<<"@setupClaTable(): Unknow relation. a:["<<element[0]<<endl;
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
 *		
 *		
 * @return The complitance of this function, true for done, false for error
 ***********************************************/
BOOLEAN  QueryPreprocessor::makeOptimize(){
	//merge
	for(int i=0; i<queryClaTable.size(); i++){
		if(queryClaTable[i].relationType==CT_PATTERN){
			continue;
		}else if(queryClaTable[i].relationType==CT_WITH){
			if(queryVarTable[queryClaTable[i].variable1].mapTo==queryVarTable[queryClaTable[i].variable2].mapTo){		  
				queryClaTable[i].index=-1;
				mergedClause++;	    
			}
		}else if(queryVarTable[queryClaTable[i].variable1].mapTo==queryVarTable[queryClaTable[i].variable2].mapTo){
			if(queryClaTable[i].relationType!=RT_NEXTT&&queryClaTable[i].relationType!=RT_NEXTBIPT&&queryClaTable[i].relationType!=RT_AFFECTS&&queryClaTable[i].relationType!=RT_AFFECTST&&queryClaTable[i].relationType!=RT_AFFECTSBIP&&queryClaTable[i].relationType!=RT_AFFECTSBIPT){
				if(queryVarTable[queryClaTable[i].variable1].variableType!=DT_UNDERSCORE){
					return false;
				}
			}
		}
	}		
	//if(!DEBUGMODE){					   
	//change select target to mapTo target
		for(int i=0; i<queryTarTable.size(); i++){
			if(queryVarTable[queryTarTable[i].varIndex].mapTo!=queryTarTable[i].varIndex){
				 queryTarTable[i].varIndex=queryVarTable[queryTarTable[i].varIndex].mapTo;
			}
		}
	//change clause to mapTo target
		for(int i=0; i<queryClaTable.size(); i++){	 
			if(queryVarTable[queryClaTable[i].variable1].mapTo!=queryClaTable[i].variable1){
				 queryClaTable[i].variable1=queryVarTable[queryClaTable[i].variable1].mapTo;
			}
			if(queryVarTable[queryClaTable[i].variable2].mapTo!=queryClaTable[i].variable2){
				 queryClaTable[i].variable2=queryVarTable[queryClaTable[i].variable2].mapTo;
			}
		}
	//remove unused clause 
		for (vector<QueryClause>::iterator it=queryClaTable.begin();it!=queryClaTable.end();){
			if(it->index==-1) 
				it = queryClaTable.erase(it);
			else 
				++it;
		}
		for(int i=0; i<queryClaTable.size(); i++){
			 queryClaTable[i].index=i;
		}
	//}
	return true;
}
/********************************************//**
 * @brief 
 * @details
 * Process: \n													  
 *		1. count appearance \n
 *		2. set dependency \n
 *		3. sort to groups \n	
 * @return The complitance of this function, true for done, false for error
 ***********************************************/
BOOLEAN QueryPreprocessor::setDependency(){
	//set appearer count
	for(int i = 0; i<(int)queryVarTable.size(); i++){ 
		for(int j = 0; j<(int)queryClaTable.size(); j++){
			if(queryClaTable[j].variable1==i||queryClaTable[j].variable2==i||queryClaTable[j].variable4==i||queryClaTable[j].variable5==i){
				queryVarTable[queryVarTable[i].mapTo].countAppear++;
			}
		}
	}
	for(int i = 0; i<(int)queryVarTable.size(); i++){ 
		for(int j = 0; j<(int)queryTarTable.size(); j++){
			if(queryTarTable[j].varIndex==i){
				queryVarTable[queryVarTable[i].mapTo].countAppear++;
			}
		}
	}
	for(int i = 0; i<(int)queryVarTable.size(); i++){ 
		if(queryVarTable[i].mapTo==i){
			if((queryVarTable[i].variableType==KT_STMT_NUM)||(queryVarTable[i].variableType==DT_UNDERSCORE)
				||(queryVarTable[i].variableType==KT_KNOWN_VARIABLE)||(queryVarTable[i].variableType==KT_KNOWN_PROCEDURE)
				||(queryVarTable[i].variableType==DT_CONSTANT)||(queryVarTable[i].variableType==DT_MINUS)||(queryVarTable[i].variableType==DT_TIMES)
				||(queryVarTable[i].variableType==DT_PLUS)||(queryVarTable[i].variableType==KT_KNOWN_CONSTANT)){
				continue;
			}else if(queryVarTable[i].countAppear>1&&queryVarTable[i].dependency==-1){
				queryVarTable[i].dependency=i;
			}
		}
	}
	//match variables that has dependency
	for(int i = 0; i<(int)queryVarTable.size(); i++){ 
		if(queryVarTable[i].dependency==-1){
			continue;
		}
		for(int j = 0; j<(int)queryClaTable.size(); j++){
			int left = queryClaTable[j].variable1;
			int right = queryClaTable[j].variable2;	 		  
		//if right and left not match than match right away
		//if any part match i, match right away
			if(left==i&&queryVarTable[right].dependency!=-1){
				 queryVarTable[right].dependency=queryVarTable[i].dependency;
			}else if(right==i&&queryVarTable[left].dependency!=-1){							   
				 queryVarTable[left].dependency=queryVarTable[i].dependency;
			}else if(queryVarTable[left].dependency!=-1&&queryVarTable[right].dependency!=-1){
				 queryVarTable[left].dependency=queryVarTable[right].dependency;
			}
		} 
	}											
	for(int j = 0; j<queryClaTable.size(); j++){   
		int var1Dep = queryVarTable[queryClaTable[j].variable1].dependency;
		int var2Dep = queryVarTable[queryClaTable[j].variable2].dependency;

		if(var1Dep==-1&&var2Dep==-1){
			if(clasueTable.count(var1Dep)>0){
				clasueTable.find(var1Dep)->second.push_back(queryClaTable[j]);
			}else{
				vector<QueryClause> newVector;
				newVector.push_back(queryClaTable[j]);
				clasueTable.insert(pair<int, vector<QueryClause>>(var1Dep, newVector));
			}
		}else if(var1Dep!=-1){		  
			if(clasueTable.count(var1Dep)>0){
				clasueTable.find(var1Dep)->second.push_back(queryClaTable[j]);
			}else{
				vector<QueryClause> newVector;
				newVector.push_back(queryClaTable[j]);					
				clasueTable.insert(pair<int, vector<QueryClause>>(var1Dep, newVector));
			}
		}else if(var2Dep!=-1){	 
			if(clasueTable.count(var2Dep)>0){
				clasueTable.find(var2Dep)->second.push_back(queryClaTable[j]);
			}else{											   
				vector<QueryClause> newVector;
				newVector.push_back(queryClaTable[j]);				 
				clasueTable.insert(pair<int, vector<QueryClause>>(var2Dep, newVector));
			}
		}
	}
	return true;
}
/********************************************//**
 * @param query string free format, can be anything 
 * @return
 *		0	suceed	definitly success
 *		1	false	if the boolean needs to return false instead of none
 *		-1	error	just not pass the validation
 ***********************************************/
int QueryPreprocessor::parse(string query){
	queryVarTable.clear();
	queryTarTable.clear();	 
	queryClaTable.clear();
	clasueTable.clear();
	trim_all(query);
	mergedClause=0;
	discardClause=0;
	while(true){
		//=======================================>>Stage 1 Declare
		vector<string> declares = getDeclares(query);
		if(!setupVarTable(declares))
			break;
		declares.clear();
		//=======================================>>Stage 2 Select
		vector<string> targets = getTargets(query);
		if((targets.size()<=0)||(!setupTarTable(targets)))		
			break;
		targets.clear();
		//=======================================>>Stage 3 Clause
		vector<string> clauses = getClauses(query); 
		if(!setupClaTable(clauses))	
			break;		
		//=======================================>>Stage 5 Dependency
		if(!makeOptimize())		
			break;
		//=======================================>>Stage 4 Dependency
		if(!setDependency())	
			break;
		
		cout<<"finish pql"<<endl;
		if(DEBUGMODE||PRINTTABLE){
			printf("QP: parsed: %2d, merged: %2d, discard: %2d, result: %2d.\n", clauses.size(), mergedClause, discardClause, queryClaTable.size()); 
			QueryPreprocessorDebug qpd;
			qpd.printQueryVariableTable(queryVarTable);
			qpd.printTargetVariableTable(queryTarTable);
			qpd.printClauseTable(clasueTable);
		}
		clauses.clear();
		return 0;
	}					  
	cout<<"false"<<endl;
	if(DEBUGMODE||PRINTTABLE){		
			QueryPreprocessorDebug qpd;
			qpd.printQueryVariableTable(queryVarTable);
			qpd.printTargetVariableTable(queryTarTable);
			qpd.printQueryClauseTable(queryClaTable);
		}
	return -1;
} 
/********************************************//**
 *  getter for queryVarTable 
 ***********************************************/
vector<QueryVariable> QueryPreprocessor::getQueryVariableTable(){
	return queryVarTable;
}
/********************************************//**
 *  getter for queryTarTable 
 ***********************************************/
vector<QueryTarget> QueryPreprocessor::getQueryTargetTable(){
	return queryTarTable;
}  
/********************************************//**
 *  getter for queryClaTable 
 ***********************************************/
vector<QueryClause> QueryPreprocessor::getQueryClauseTable(){
	return queryClaTable;
}
/********************************************//**
 *  getter for clauseTable 
 ***********************************************/
map<int, vector<QueryClause>> QueryPreprocessor::getClauseTable(){
	return clasueTable;
}

