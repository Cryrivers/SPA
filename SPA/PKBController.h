
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
#include "Operators.h"
#include "AffectsBip.h"
#include "NextBip.h"

class DesignExtractor;
class AST;
class CFG;
class AffectsBip;
class NextBip;

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
	CFG* getCFGBip();

	//operator api
	void addPlusOperator(int ASTNodeId);
	void addMinusOperator(int ASTNodeId);
	void addMultiplyOperator(int ASTNodeId);

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
	BOOLEAN callsStar(PROC_INDEX_LIST* callers_ptr, PROC_INDEX_LIST* callees_ptr, int arg);
	PROC_INDEX getCallee(STMT stmt);
	PROC_INDEX_LIST getAllCallers();
	PROC_INDEX_LIST getAllCallees();

	//next api
	void addNext(STMT stmt1, STMT stmt2, CFGBipType bipType);
	BOOLEAN next(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg);
	BOOLEAN nextStar(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg);
	STMT_LIST getAllNextFirst();
	STMT_LIST getAllNextSecond();

	//affect api
	BOOLEAN affects(STMT_LIST*, STMT_LIST*, int);
	BOOLEAN affectsStar(STMT_LIST*, STMT_LIST*, int);

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
	STMT getProcStart(PROC_INDEX ind);
	STMT getProcEnd(PROC_INDEX ind);

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


	BOOLEAN affectsBip(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg);
	BOOLEAN isAffectsBip(STMT stmt1, STMT stmt2);
	BOOLEAN affectsBipStar(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg);
	BOOLEAN isAffectsBipStar(STMT stmt1, STMT stmt2);

	BOOLEAN nextBip(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg);
	BOOLEAN isNextBip(STMT stmt1, STMT stmt2);
	BOOLEAN nextBipStar(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg);
	BOOLEAN isNextBipStar(STMT stmt1, STMT stmt2);

	BOOLEAN isContains(int indexA, int indexB, ASTNodeType argA, ASTNodeType argB);
	BOOLEAN isContainsStar(int indexA, int indexB, ASTNodeType argA, ASTNodeType argB);
	BOOLEAN isSibling(int indexA, int indexB, ASTNodeType argA, ASTNodeType argB);
	BOOLEAN contains(vector<int>*, vector<int>*, ASTNodeType argA, ASTNodeType argB, int arg);
	BOOLEAN containsStar(vector<int>*, vector<int>*, ASTNodeType argA, ASTNodeType argB, int arg);
	BOOLEAN sibling(vector<int>* indexAs, vector<int>* indexBs, ASTNodeType argA, ASTNodeType argB, int arg);

	BOOLEAN isAffects(STMT, STMT);
	BOOLEAN isAffectsStar(STMT, STMT);

	BOOLEAN isFollows(STMT stmt1, STMT stmt2);
	BOOLEAN isFollowsStar(STMT stmt1, STMT stmt2);
	BOOLEAN isParent(STMT stmt1, STMT stmt2);
	BOOLEAN isParentStar(STMT stmt1, STMT stmt2);
	BOOLEAN isModifies(STMT stmt, VAR_INDEX var);
	BOOLEAN isModifiesP(PROC_INDEX proc, VAR_INDEX var);
	BOOLEAN isUses(STMT stmt, VAR_INDEX var);
	BOOLEAN isUsesP(PROC_INDEX proc, VAR_INDEX var);
	BOOLEAN isNext(STMT stmt1, STMT stmt2);
	BOOLEAN isNextStar(STMT stmt1, STMT stmt2);
	BOOLEAN isCalls(PROC_INDEX caller, PROC_INDEX callee);
	BOOLEAN isCallsStar(PROC_INDEX caller, PROC_INDEX callee);

	int followsSize(){
		return _follows->size();
	}
	int parentSize(){
		return _parent->size();
	}
	int modifiesSize(){
		return _modifies->size();
	}
	int usesSize(){
		return _uses->size();
	}
	int modifiesPSize(){
		return _modifiesP->size();
	}
	int usesPSize(){
		return _usesP->size();
	}
	int nextSize(){
		return _next->size();
	}
	int callsSize(){
		return _calls->size();
	}
	int varTableSize(){
		return _varTable->size();
	}
	int procTableSize(){
		return _procTable->size();
	}
	int stmtSize();
	int assignSize();
	int whileSize();
	int ifSize();
	int callSize();
	int stmtListSize();
	int constSize(){
		_constTable->size();
	}

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
	CFG* _cfgbip;
	ASTNode* _astNode;
	AffectsBip* _affectsBip;
	NextBip* _nextBip;
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

	//operator table
	Operators* _plus;
	Operators* _minus;
	Operators* _multiply;

	PKBController();
	void build_proc_var();
	void build_proc_call();
	void build_var_call();
	void build_call_call();
	void build_stmt_const();
};