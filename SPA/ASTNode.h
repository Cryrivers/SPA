#pragma once
#include "SPAType.h"

using namespace std;

class ASTNode
{
private:

ASTNode(void);
ASTNodeType _nodeType;
int _nodeValue;
ASTNode* _siblings;
ASTNode* _children;
int _stmtNumber;
string _hashcode;
bool hashCodeComputed;

public:
~ASTNode(void);

static ASTNode* createNode (ASTNodeType t, int value);

//Get the type of this ASTNode
ASTNodeType getNodeType();
void setNodeType(ASTNodeType t);

int getNodeValue();
void setNodeValue(int v);

ASTNode* getChildren(int index = 0);
void addChild(ASTNode* node);
ASTNode* createChild(ASTNodeType t, int value);

ASTNode* getSibling();
void addSibling(ASTNode* node);
ASTNode* createSibling(ASTNodeType t, int value);

int getStmtNumber();
void setStmtNumber(int number);

string getHashcode();
string appendHashcode(string hashCode);

void markHashcodeComputed();
bool isHashcodeComputed();
};

