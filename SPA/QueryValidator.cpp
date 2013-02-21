/********************************************//**
 * @file QueryValidator.cpp
 * @brief Query validator
 * @see QueryValidator
 * @author Zhang Haoqiang
 * @date 30.01.2012
 * @version 1.01
 ***********************************************/
#include "stdafx.h"
#include "QueryValidator.h"
/********************************************//**
 *  Constructor
 ***********************************************/
QueryValidator::QueryValidator(void){
}
/********************************************//**
 *  Destructor
 ***********************************************/
QueryValidator::~QueryValidator(void){
}
//==================
//===Lexical rules==
//==================
/********************************************//**
 * Lexical rule: A-Z | a-z
 * @param a a char
 * @return boolean
 ***********************************************/
bool QueryValidator::isLetter(char a){
	if((a>='A'&&a<='Z')||(a>='a'&&a<='z')){
		return true;
	}
	return false;
}
/********************************************//**
 * Lexical rule: 0-9
 * @param a a char
 * @return boolean
 ***********************************************/
bool QueryValidator::isDigit(char a){
	int tmp = (int) a;
	if(tmp>47&&tmp<58){
		return true;
	}
	return false;
}
/********************************************//**
 * Lexical rule: LETTER ( LETTER | DIGIT | '#' )*
 * @param str string
 * @see isLetter
 * @see isDigit
 * @return boolean
 ***********************************************/
bool QueryValidator::isIndent(string str){
	if(str.size()<1){
		cout<<"1"<<endl;
		return false;
	}
	if(!isLetter(str[0])){
		return false;
	}
	for(unsigned int i = 1; i<str.size(); i++){
		if(isLetter(str[i])||isDigit(str[i])||str[i]=='#'){
 			continue;
 		}else{
			return false;
		}
	}
	return true;
}
/********************************************//**
 * Lexical rule: DIGIT+
 * @param str string
 * @see isDigit
 * @return boolean
 ***********************************************/
bool QueryValidator::isInteger(string str){
	for(unsigned int i = 0; i<str.size(); i++){
		if(isDigit(str[i])==true)
			continue;
		else
			return false;
	}
	return true;
}
//==================
//Auxiliary grammar=
//==================
/********************************************//**
 * Auxiliary grammar rule: synonym | attrRef
 * @param str string
 * @see isSynonym
 * @see isAttrRef
 * @return boolean
 ***********************************************/
bool QueryValidator::isElem(string str){
	if(isSynonym(str)){
		return true;
	}else if(isAttrRef(str)){
		return true;
	}
	return false;
}
/********************************************//**
 * Auxiliary grammar rule: '"' IDENT '"' | INTEGER | attrRef
 * @param str string
 * @see isIndent
 * @see isInteger
 * @see isAttrRef
 * @return boolean
 ***********************************************/
bool QueryValidator::isRef(string str){
	if(isAttrRef(str)){
		return true;
	}else if(isInteger(str)){
		return true;
	}else if(isSynonym(str)){
		return true;
	}else if(str.find("\"")<=str.size()){
		str = str.substr(str.find("\"")+1);
		if(str.find("\"")<=str.size()){
			str = str.substr(0,str.find("\""));
			if(isSynonym(str)){
				return true;
			}
		}													
	}
	return false;
}
/********************************************//**
 * Auxiliary grammar rule: synonym | attrRef
 * @param str string
 * @see isSynonym
 * @see isAttrRef
 * @return boolean
 ***********************************************/
bool QueryValidator::isRefpl(string str){
	if(isSynonym(str)){
		return true;
	}else if(isAttrRef(str)){
		return true;
	}
	return false;
}
/********************************************//**
 * Auxiliary grammar rule: synonym '.' attrName
 * @param str string
 * @see isSynonym
 * @return boolean
 ***********************************************/
bool QueryValidator::isAttrRef(string str){
	if(str.find(".")<str.size()){
		if(isSynonym(str.substr(0, str.find(".")))){
			return true;
		}
	}
	return false;
}
/********************************************//**
 * Auxiliary grammar rule: IDENT
 * @param str string
 * @see isIndent
 * @return boolean
 ***********************************************/
bool QueryValidator::isSynonym(string str){
	if(isIndent(str)){
		return true;
 	}
 	return false;
}
/********************************************//**
 * Auxiliary grammar rule: synonym | '_' | '"' IDENT '"' | INTEGER
 * @param str string
 * @see isInteger
 * @see isSynonym
 * @return boolean
 ***********************************************/
bool QueryValidator::isEntRef(string str){
	if(isSynonym(str)){
		return true;
	}else if(str=="_"){
		return true;
	}else if(str.find("\"")<=str.size()){
	str = str.substr(str.find("\"")+1);
		if(str.find("\"")<=str.size()){
			str = str.substr(0,str.find("\""));
			if(isSynonym(str)){
				return true;
			}
		}													
	}
	return false;
}
/********************************************//**
 * Auxiliary grammar rule: synonym | '_' | INTEGER
 * @param str string
 * @see isInteger
 * @see isSynonym
 * @return boolean
 ***********************************************/
bool QueryValidator::isStmtRef(string str){
	if (isInteger(str)) {
		return true;
	}else if(str=="_"){
		return true;
	}else if(isSynonym(str)){
		return true;
	}
	return false;
}
/********************************************//**
 * Auxiliary grammar rule: synonym | '_' | INTEGER
 * @param str string
 * @see isInteger
 * @see isSynonym
 * @return boolean
 ***********************************************/
bool QueryValidator::isLineRef(string str){
	if (isInteger(str)) {
		return true;
	}else if(str=="_"){
		return true;
	}else if(isSynonym(str)){
		return true;
	}
	return false;
}
/********************************************//**
 * Auxiliary grammar rule: synonym | '_' | INTEGER
 * @param str string
 * @see isInteger
 * @see isSynonym
 * @return boolean
 ***********************************************/
bool QueryValidator::isVarRef(string str){
	if(str=="_"){
		return true;
	}else if(isSynonym(str)){
		return true;
	}else if(str.find("\"")<=str.size()){
	str = str.substr(str.find("\"")+1);
		if(str.find("\"")<=str.size()){
			str = str.substr(0,str.find("\""));
			if(isSynonym(str)){
				return true;
			}
		}													
	}
	return false;
}
/********************************************//**
 * Auxiliary grammar rule: '"' expr '"' | '_' '"' expr '"' '_' | '_'
 * @param str string
 * @return boolean
 ***********************************************/
bool QueryValidator::isExprSpec(string str){
	if(str=="_"){
		return true;
	}else if(str[0]=='_'&&str[str.size()-1]=='_'){
		if(str[1]=='\"'&&str[str.size()-2]=='\"'){
			return true;
		}
	}
	return false;
}



