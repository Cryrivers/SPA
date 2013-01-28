/*
* TestFollows.cpp
*
*  Created on: 30 Oct 2012
*      Author: ray
*/

#include <cppunit/config/SourcePrefix.h>
#include "stdafx.h"
#include "Parent.h"
#include "TestParent.h"
#include <iostream>
#include <string>

void 
	ParentTest::setUp()
{
}


void 
	ParentTest::tearDown()
{
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ParentTest ); // Note 4 

void ParentTest::f0() {
	Parent f;

	//case 1a, should print false
	vector<int> va; vector<int> vb;
	CPPUNIT_ASSERT(f.parent(&va,&vb,0) == false);

	f.addParent(7,8);
	f.addParent(7,9);
	f.addParent(7,15);
	f.addParent(9,10);
	f.addParent(9,11);
	f.addParent(9,12);
	f.addParent(9,13);
	f.addParent(13,14);
	/* Parent table is
	* 7 7 7  9  9  9  9  13
	* 8 9 15 10 11 12 13 14
	*
	*/
	//case 1a, should print true
	CPPUNIT_ASSERT(f.parent(&va,&vb,0) == true);

	//case 1b, should print false
	vb.push_back(3);
	CPPUNIT_ASSERT(f.parent(&va,&vb,0) == false);
	//case 1b, should print false
	vb.push_back(5);
	CPPUNIT_ASSERT(f.parent(&va,&vb,0) == false);
	//case 1b, should print true
	vb.push_back(8);
	CPPUNIT_ASSERT(f.parent(&va,&vb,0) == true);

	vb.clear();

	//case 1c, should print false
	va.push_back(11);
	CPPUNIT_ASSERT(f.parent(&va,&vb,0) == false);
	//case 1c, should print false
	va.push_back(18);
	CPPUNIT_ASSERT(f.parent(&va,&vb,0) == false);
	//case 1c, should print true
	va.push_back(7);
	CPPUNIT_ASSERT(f.parent(&va,&vb,0) == true);

	//case 1d, should print false
	vb.push_back(10);
	CPPUNIT_ASSERT(f.parent(&va,&vb,0) == false);
	//case 1d, should print true
	vb.push_back(8);
	CPPUNIT_ASSERT(f.parent(&va,&vb,0) == true);

	//should print 0 1 0 0 1 0 0 1 0 1
}

void ParentTest::f1() {
	Parent f;
	f.addParent(7,8);
	f.addParent(7,9);
	f.addParent(7,15);
	f.addParent(9,10);
	f.addParent(9,11);
	f.addParent(9,12);
	f.addParent(9,13);
	f.addParent(13,14);
	/* Parent table is
	* 7 7 7  9  9  9  9  13
	* 8 9 15 10 11 12 13 14
	*
	*/
	vector<int> va; vector<int> vb;

	//case 2a, should print true\n 8 9 15 10 11 12 13 14
	int myints[] = {8, 9, 15, 10, 11, 12, 13, 14};

	CPPUNIT_ASSERT(f.parent(&va,&vb,1) == true);
	CPPUNIT_ASSERT(vb == vector<int>(myints, myints + sizeof(myints) / sizeof(int)));

	//case 2b, should print false\n no element
	vb.clear();
	va.push_back(11);

	CPPUNIT_ASSERT(f.parent(&va,&vb,1) == false);
	CPPUNIT_ASSERT(vb == vector<int>());

	//case 2b, should print true\n 10 11 12 13
	va.clear();
	va.push_back(9);

	int myints2[] = {10, 11, 12, 13};
	CPPUNIT_ASSERT(f.parent(&va,&vb,1) == true);
	CPPUNIT_ASSERT(vb == vector<int>(myints2, myints2 + sizeof(myints2) / sizeof(int)));

	va.clear();
	vb.clear();

	//case 2c, should print true\n 8 9 15 12 13
	vb.push_back(2);
	vb.push_back(8);
	vb.push_back(9);
	vb.push_back(15);
	vb.push_back(12);
	vb.push_back(13);
	vb.push_back(16);

	int myint3[] = {8, 9, 15, 12, 13};
	CPPUNIT_ASSERT(f.parent(&va,&vb,1) == true);
	CPPUNIT_ASSERT(vb == vector<int>(myint3, myint3 + sizeof(myint3) / sizeof(int)));


	//case 2d, should print false\n no element
	va.clear();
	vb.clear();
	va.push_back(9);
	vb.push_back(8);vb.push_back(1);vb.push_back(15);vb.push_back(14);

	CPPUNIT_ASSERT(f.parent(&va,&vb,1) == false);
	CPPUNIT_ASSERT(vb == vector<int>());

	//case 2d, should print true\n 8 15 9
	va.clear();
	va.push_back(7);
	vb.push_back(8);vb.push_back(1);vb.push_back(15);vb.push_back(14);
	vb.push_back(5);vb.push_back(9);vb.push_back(12);
	int myint4[] = {8, 15, 9};
	CPPUNIT_ASSERT(f.parent(&va,&vb,1) == true);
	CPPUNIT_ASSERT(vb == vector<int>(myint4, myint4 + sizeof(myint4) / sizeof(int)));
}

void ParentTest::f2() {
	Parent f;
	f.addParent(7,8);
	f.addParent(7,9);
	f.addParent(7,15);
	f.addParent(9,10);
	f.addParent(9,11);
	f.addParent(9,12);
	f.addParent(9,13);
	f.addParent(13,14);
	/* Parent table is
	* 7 7 7  9  9  9  9  13
	* 8 9 15 10 11 12 13 14
	*
	*/
	vector<int> va; vector<int> vb;

	//case 3a, should print true\n1 7 9 13
	int myints[] = {7, 9, 13};

	CPPUNIT_ASSERT(f.parent(&va,&vb,2) == true);
	CPPUNIT_ASSERT(va == vector<int>(myints, myints + sizeof(myints) / sizeof(int)));

	//case 3b, should print false\n no element
	va.clear();
	vb.push_back(7);

	CPPUNIT_ASSERT(f.parent(&va,&vb,2) == false);
	CPPUNIT_ASSERT(va == vector<int>());

	//case 3b, should print true\n 9
	vb.clear();
	vb.push_back(11);

	CPPUNIT_ASSERT(f.parent(&va,&vb,2) == true);
	CPPUNIT_ASSERT(va == vector<int>(1,9));

	va.clear();
	vb.clear();

	//case 3c, should print true\n 7 13
	va.push_back(7);
	va.push_back(1);
	va.push_back(4);
	va.push_back(15);
	va.push_back(12);
	va.push_back(13);
	va.push_back(16);

	int myint2[] = {7, 13};

	CPPUNIT_ASSERT(f.parent(&va,&vb,2) == true);
	CPPUNIT_ASSERT(va == vector<int>(myint2, myint2 + sizeof(myint2) / sizeof(int)));

	//case 3d, should print false\n no element
	va.clear();
	vb.clear();
	vb.push_back(6);
	va.push_back(1);va.push_back(2);va.push_back(3);

	CPPUNIT_ASSERT(f.parent(&va,&vb,2) == false);
	CPPUNIT_ASSERT(va == vector<int>());

	//case 3d, should print true\n 7
	vb.clear();
	vb.push_back(15);
	va.push_back(9);va.push_back(7);va.push_back(13);

	CPPUNIT_ASSERT(f.parent(&va,&vb,2) == true);
	CPPUNIT_ASSERT(va == vector<int>(1,7));
}

void ParentTest::f3() {
	Parent f;
	f.addParent(7,8);
	f.addParent(7,9);
	f.addParent(7,15);
	f.addParent(9,10);
	f.addParent(9,11);
	f.addParent(9,12);
	f.addParent(9,13);
	f.addParent(13,14);
	/* Parent table is
	* 7 7 7  9  9  9  9  13
	* 8 9 15 10 11 12 13 14
	*
	*/
	vector<int> va; vector<int> vb;

	//case 4a, should print true
	// 7 7 7  9  9  9  9  13
	// 8 9 15 10 11 12 13 14
	int myintA[] = {7, 7, 7,  9,  9,  9,  9,  13};
	int myintB[] = {8, 9, 15, 10, 11, 12, 13, 14};

	CPPUNIT_ASSERT(f.parent(&va,&vb,3) == true);
	CPPUNIT_ASSERT(va == vector<int>(myintA, myintA + sizeof(myintA) / sizeof(int)));
	CPPUNIT_ASSERT(vb == vector<int>(myintB, myintB + sizeof(myintB) / sizeof(int)));

	//case 4b, should print true
	//9 7 7
	//12 15 9
	va.clear();
	vb.clear();
	vb.push_back(12); vb.push_back(15);vb.push_back(9);vb.push_back(16);

	int myintA2[] = {9, 7, 7};
	int myintB2[] = {12, 15, 9};

	CPPUNIT_ASSERT(f.parent(&va,&vb,3) == true);
	CPPUNIT_ASSERT(va == vector<int>(myintA2, myintA2 + sizeof(myintA2) / sizeof(int)));
	CPPUNIT_ASSERT(vb == vector<int>(myintB2, myintB2 + sizeof(myintB2) / sizeof(int)));

	va.clear();
	vb.clear();

	//case 4c, should print true
	//7 7 7 13 9 9 9 9
	//8 9 15 14 10 11 12 13
	va.push_back(7); va.push_back(6);va.push_back(13);va.push_back(9);va.push_back(19);
	int myintA3[] = {7, 7, 7, 13, 9, 9, 9, 9};
	int myintB3[] = {8, 9, 15, 14, 10, 11, 12, 13};

	CPPUNIT_ASSERT(f.parent(&va,&vb,3) == true);
	CPPUNIT_ASSERT(va == vector<int>(myintA3, myintA3 + sizeof(myintA3) / sizeof(int)));
	CPPUNIT_ASSERT(vb == vector<int>(myintB3, myintB3 + sizeof(myintB3) / sizeof(int)));

	//case 4d, should print false
	//no element
	//no element
	va.clear();
	vb.clear();
	vb.push_back(8);vb.push_back(12);vb.push_back(16);
	va.push_back(9);va.push_back(7);va.push_back(6);

	CPPUNIT_ASSERT(f.parent(&va,&vb,3) == false);
	CPPUNIT_ASSERT(va == vector<int>());
	CPPUNIT_ASSERT(vb == vector<int>());

	//case 4d, should print true
	//9 7
	//12 9
	vb.push_back(12);vb.push_back(7);vb.push_back(7);vb.push_back(9);
	va.push_back(9);va.push_back(8);va.push_back(15);va.push_back(7);

	int myintA4[] = {9, 7};
	int myintB4[] = {12, 9};

	CPPUNIT_ASSERT(f.parent(&va,&vb,3) == true);
	CPPUNIT_ASSERT(va == vector<int>(myintA4, myintA4 + sizeof(myintA4) / sizeof(int)));
	CPPUNIT_ASSERT(vb == vector<int>(myintB4, myintB4 + sizeof(myintB4) / sizeof(int)));
}
