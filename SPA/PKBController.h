
#pragma once

#include "DesignExtractor.h"
#include "Follows.h"
#include "Parent.h"
#include "Modifies.h"
#include "ModifiesP.h"
#include "Uses.h"
#include "UsesP.h"
#include "Next.h"
#include "VarTable.h"
#include "ASTNode.h"
#include "AST.h"
#include "SPAType.h"
#include "Calls.h"
#include "ProcTable.h"
#include "CFG.h"

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
	CFG* getCFG();

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

	//modifiesP api
	void addModifiesP(PROC_INDEX proc, VAR_INDEX var);
	BOOLEAN modifiesP(PROC_INDEX_LIST* ps_ptr, VAR_INDEX_LIST* vs_ptr,int arg);
	STMT_LIST getAllModifiesPFirst();
	VAR_INDEX_LIST getAllModifiesPSecond();

	//uses api
	void addUses(STMT stmt, VAR_INDEX var);
	BOOLEAN uses(STMT_LIST* sts_ptr, VAR_INDEX_LIST* vs_ptr,int arg);
	STMT_LIST getAllUsesFirst();
	STMT_LIST getAllUsesSecond();

	//usesP api
	void addUsesP(PROC_INDEX proc, VAR_INDEX var);
	BOOLEAN usesP(PROC_INDEX_LIST* ps_ptr, VAR_INDEX_LIST* vs_ptr,int arg);
	STMT_LIST getAllUsesPFirst();
	STMT_LIST getAllUsesPSecond();

	//calls api
	void addCalls(STMT stmt, PROC_INDEX caller, PROC_INDEX callee);
	BOOLEAN calls(PROC_INDEX_LIST* callers_ptr, PROC_INDEX_LIST* callees_ptr, int arg);
	PROC_INDEX getCallee(STMT stmt);
	PROC_INDEX_LIST getAllCallers();
	PROC_INDEX_LIST getAllCallees();

	//next api
	void addNext(STMT stmt1, STMT stmt2);
	BOOLEAN next(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg);
	BOOLEAN nextStar(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg);
	STMT_LIST getAllNextFirst();
	STMT_LIST getAllNextSecond();


	//vartable api
	VAR_INDEX addVar(STRING varName);
	STRING getVarName(VAR_INDEX ind);
	VAR_INDEX getVarIndex(STRING varName);
	STRING_LIST getAllVarName(VAR_INDEX_LIST inds);
	STRING_LIST getAllVarName();

	//procTable api
	PROC_INDEX addProc(STRING procName,STMT,STMT);
	STRING getProcName(PROC_INDEX ind);
	PROC_INDEX getProcIndex(STRING procName);
	STRING_LIST getAllProcName(PROC_INDEX_LIST inds);
	STRING_LIST getAllProcName();

	//additional api
	void addConstant(int value);
	vector<int>* getConstantTable();
	void addAssignmentNode(ASTNode* node);
	void addWhileLoopNode(ASTNode* node);
	void addIfBranchNode(ASTNode* node);
	vector<ASTNode*>* getAssignmentTable();
	vector<ASTNode*>* getWhileTable();
	vector<ASTNode*>* getIfBranchTable();

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
	BOOLEAN pattern(vector<int>*, vector<int>*, string, PatternQueryType, int);
	BOOLEAN with(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr,int arg1,int arg2, int arg);
	void completePKB();

private:
	Follows* _follows;
	Parent* _parent;
	Modifies* _modifies;
	ModifiesP* _modifiesP;
	Uses* _uses;
	UsesP* _usesP;
	Calls* _calls;
	Next* _next;
	VarTable* _varTable;
	ProcTable* _procTable;
	AST* _ast;
	CFG* _cfg;
	ASTNode* _astNode;
	DesignExtractor* _designExtractor;
	vector<statement>* _preprocessedProgram;

	vector<int>* _constTable;
	vector<ASTNode*>* _assignmentTable;
	vector<ASTNode*>* _whileLoopTable;
	vector<ASTNode*>* _ifBranchTable;

	//with
	Follows* _proc_var;
	Modifies* _call_call;
	Parent* _proc_call;
	Parent* _var_call;
	Follows* _stmt_const;

	PKBController();
	void build_proc_var();
	void build_proc_call();
	void build_var_call();
	void build_call_call();
	void build_stmt_const();
};