
#pragma once

#include "DesignExtractor.h"
#include "Follows.h"
#include "Parent.h"
#include "Modifies.h"
#include "Uses.h"
#include "VarTable.h"
#include "ASTNode.h"
#include "AST.h"
#include "SPAType.h"
#include "ProcTable.h"

class DesignExtractor;
class AST;

class PKBController
{

private:
	void init();
public:
	static PKBController* createInstance();
	~PKBController();

	//ASTNode* getASTRoot();
	ASTNode* astGetRoot();
	AST* getAST();

	//follows api
	void addFollows(STMT stmt1, STMT stmt2);
	BOOLEAN follows(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr,int arg);
	BOOLEAN followsStar(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg);

	//parent api
	void addParent(STMT stmt1, STMT stmt2);
	BOOLEAN parent(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr,int arg);
	BOOLEAN parentStar(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr,int arg);
	//STMT_LIST getAllParentFirst();
	//STMT_LIST getAllParentSecond();

	//modifies api
	void addModifies(STMT stmt, VAR_INDEX var);
	BOOLEAN modifies(STMT_LIST* sts_ptr, VAR_INDEX_LIST* vs_ptr,int arg);
	STMT_LIST getAllModifiesFirst();
	VAR_INDEX_LIST getAllModifiesSecond();

	//uses api
	void addUses(STMT stmt, VAR_INDEX var);
	BOOLEAN uses(STMT_LIST* sts_ptr, VAR_INDEX_LIST* vs_ptr,int arg);
	STMT_LIST getAllUsesFirst();
	STMT_LIST getAllUsesSecond();

	//vartable api
	VAR_INDEX addVar(STRING varName);
	STRING getVarName(VAR_INDEX ind);
	VAR_INDEX getVarIndex(STRING varName);
	STRING_LIST getAllVarName(VAR_INDEX_LIST inds);
	STRING_LIST getAllVarName();

	//procTable api
	PROC_INDEX addProc(STRING procName);
	STRING getProcName(PROC_INDEX ind);
	PROC_INDEX getProcIndex(STRING procName);
	STRING_LIST getAllProcName(PROC_INDEX_LIST inds);
	STRING_LIST getAllProcName();

	//additional api
	void addConstant(int value);
	vector<int>* getConstantTable();
	void addAssignmentNode(ASTNode* node);
	vector<ASTNode*>* getAssignmentTable();

	//parser api
	vector<statement>* getPreprocessedProgram();
	void setPreprocessedProgram(vector<statement>* p);

	//design extractor
	BOOLEAN getAllStmt(STMT_LIST *);
	BOOLEAN getAllAssignment(STMT_LIST *);
	BOOLEAN getAllWhile(STMT_LIST *);
	BOOLEAN getAllConstant(vector<int> *);
	BOOLEAN getAllIf(STMT_LIST*);
	BOOLEAN getAllCall(STMT_LIST*);
	BOOLEAN getAllProc(vector<int>*);
	BOOLEAN pattern(vector<int>*, vector<int>*, string, int);
	void completePKB();

private:
	Follows* _follows;
	Parent* _parent;
	Modifies* _modifies;
	Uses* _uses;
	VarTable* _varTable;
	ProcTable* _procTable;
	AST* _ast;
	ASTNode* _astNode;
	DesignExtractor* _designExtractor;
	vector<statement>* _preprocessedProgram;

	vector<int>* _constTable;
	vector<ASTNode*>* _assignmentTable;

	PKBController();

};