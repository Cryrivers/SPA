/*
* TestUses.cpp
*
*  Created on: 30 Oct 2012
*      Author: ray
*/

#include <cppunit/config/SourcePrefix.h>
#include "stdafx.h"
#include "Uses.h"
#include "TestUses.h"
#include <iostream>
#include <string>

void 
	UsesTest::setUp()
{
}


void 
	UsesTest::tearDown()
{
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( UsesTest ); // Note 4 

void UsesTest::f0() {
	Uses f;

	//case 1a, should print false
	vector<int> va; vector<int> vb;
	CPPUNIT_ASSERT(f.uses(&va,&vb,0) == false);

	f.addUses(2,0);
	f.addUses(3,1);
	f.addUses(3,0);
	f.addUses(4,1);
	f.addUses(4,2);
	f.addUses(5,3);
	f.addUses(5,0);
	f.addUses(5,1);
	f.addUses(6,3);
	f.addUses(6,0);
	f.addUses(6,1);
	f.addUses(7,5);
	f.addUses(7,1);
	f.addUses(7,2);
	f.addUses(7,0);
	f.addUses(8,1);
	f.addUses(8,2);
	f.addUses(9,0);
	f.addUses(9,2);
	/* Uses table is
	* 2, 3, 3, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 7, 8, 8, 9, 9
	* 0, 1, 0, 1, 2, 3, 0, 1, 3, 0, 1, 5, 1, 2, 0, 1, 2, 0, 2
	*
	*/
	//case 1a, should print true
	CPPUNIT_ASSERT(f.uses(&va,&vb,0) == true);

	//case 1b, should print false
	vb.push_back(6);
	CPPUNIT_ASSERT(f.uses(&va,&vb,0) == false);
	//case 1b, should print false
	vb.push_back(9);
	CPPUNIT_ASSERT(f.uses(&va,&vb,0) == false);
	//case 1b, should print true
	vb.push_back(3);
	CPPUNIT_ASSERT(f.uses(&va,&vb,0) == true);

	vb.clear();

	//case 1c, should print false
	va.push_back(15);
	CPPUNIT_ASSERT(f.uses(&va,&vb,0) == false);
	//case 1c, should print false
	va.push_back(18);
	CPPUNIT_ASSERT(f.uses(&va,&vb,0) == false);
	//case 1c, should print true
	va.push_back(7);
	CPPUNIT_ASSERT(f.uses(&va,&vb,0) == true);

	//case 1d, should print false
	vb.push_back(10);
	CPPUNIT_ASSERT(f.uses(&va,&vb,0) == false);
	//case 1d, should print true
	vb.push_back(2);
	CPPUNIT_ASSERT(f.uses(&va,&vb,0) == true);

}

void UsesTest::f1() {
	Uses f;
	f.addUses(2,0);
	f.addUses(3,1);
	f.addUses(3,0);
	f.addUses(4,1);
	f.addUses(4,2);
	f.addUses(5,3);
	f.addUses(5,0);
	f.addUses(5,1);
	f.addUses(6,3);
	f.addUses(6,0);
	f.addUses(6,1);
	f.addUses(7,5);
	f.addUses(7,1);
	f.addUses(7,2);
	f.addUses(7,0);
	f.addUses(8,1);
	f.addUses(8,2);
	f.addUses(9,0);
	f.addUses(9,2);
	/* Uses table is
	* 2, 3, 3, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 7, 8, 8, 9, 9
	* 0, 1, 0, 1, 2, 3, 0, 1, 3, 0, 1, 5, 1, 2, 0, 1, 2, 0, 2
	*
	*/
	vector<int> va; vector<int> vb;

	//case 2a, should print true\n 0, 1, 2, 3, 5
	int myints[] = {0, 1, 2, 3, 5};

	CPPUNIT_ASSERT(f.uses(&va,&vb,1) == true);
	CPPUNIT_ASSERT(vb == vector<int>(myints, myints + sizeof(myints) / sizeof(int)));

	//case 2b, should print false\n no element
	vb.clear();
	va.push_back(1);va.push_back(0);

	CPPUNIT_ASSERT(f.uses(&va,&vb,1) == false);
	CPPUNIT_ASSERT(vb == vector<int>());

	//case 2b, should print true\n 3, 0, 1, 5, 2
	va.push_back(5);va.push_back(7);

	int myints2[] = {3, 0, 1, 5, 2};
	CPPUNIT_ASSERT(f.uses(&va,&vb,1) == true);
	CPPUNIT_ASSERT(vb == vector<int>(myints2, myints2 + sizeof(myints2) / sizeof(int)));

	va.clear();
	vb.clear();

	//case 2c, should print true\n 2 3 5
	vb.push_back(2);
	vb.push_back(3);
	vb.push_back(4);
	vb.push_back(5);
	vb.push_back(6);
	vb.push_back(7);
	vb.push_back(8);

	int myint3[] = {2, 3, 5};
	CPPUNIT_ASSERT(f.uses(&va,&vb,1) == true);
	CPPUNIT_ASSERT(vb == vector<int>(myint3, myint3 + sizeof(myint3) / sizeof(int)));


	//case 2d, should print false\n no element
	va.clear();
	vb.clear();
	va.push_back(9);va.push_back(2);
	vb.push_back(8);vb.push_back(3);vb.push_back(15);vb.push_back(14);

	CPPUNIT_ASSERT(f.uses(&va,&vb,1) == false);
	CPPUNIT_ASSERT(vb == vector<int>());

	//case 2d, should print true\n 1 5
	va.clear();
	va.push_back(7);va.push_back(9);va.push_back(12);va.push_back(14);va.push_back(3);
	vb.push_back(8);vb.push_back(1);vb.push_back(15);vb.push_back(14);
	vb.push_back(5);vb.push_back(9);vb.push_back(12);vb.push_back(3);vb.push_back(4);
	int myint4[] = {1, 5};
	CPPUNIT_ASSERT(f.uses(&va,&vb,1) == true);
	CPPUNIT_ASSERT(vb == vector<int>(myint4, myint4 + sizeof(myint4) / sizeof(int)));
}

void UsesTest::f2() {
	Uses f;
	f.addUses(2,0);
	f.addUses(3,1);
	f.addUses(3,0);
	f.addUses(4,1);
	f.addUses(4,2);
	f.addUses(5,3);
	f.addUses(5,0);
	f.addUses(5,1);
	f.addUses(6,3);
	f.addUses(6,0);
	f.addUses(6,1);
	f.addUses(7,5);
	f.addUses(7,1);
	f.addUses(7,2);
	f.addUses(7,0);
	f.addUses(8,1);
	f.addUses(8,2);
	f.addUses(9,0);
	f.addUses(9,2);
	/* Uses table is
	* 2, 3, 3, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 7, 8, 8, 9, 9
	* 0, 1, 0, 1, 2, 3, 0, 1, 3, 0, 1, 5, 1, 2, 0, 1, 2, 0, 2
	*
	*/
	vector<int> va; vector<int> vb;

	//case 3a, should print true\n1 2, 3, 4, 5, 6, 7, 8, 9
	int myints[] = {2, 3, 4, 5, 6, 7, 8, 9};

	CPPUNIT_ASSERT(f.uses(&va,&vb,2) == true);
	CPPUNIT_ASSERT(va == vector<int>(myints, myints + sizeof(myints) / sizeof(int)));

	//case 3b, should print false\n no element
	va.clear();
	vb.push_back(7);vb.push_back(4);

	CPPUNIT_ASSERT(f.uses(&va,&vb,2) == false);
	CPPUNIT_ASSERT(va == vector<int>());

	//case 3b, should print true\n 3, 4, 5, 6, 7, 8, 9
	vb.push_back(1);vb.push_back(2);
	int myints2[] = {3, 4, 5, 6, 7, 8, 9};

	CPPUNIT_ASSERT(f.uses(&va,&vb,2) == true);
	CPPUNIT_ASSERT(va == vector<int>(myints2, myints2 + sizeof(myints2) / sizeof(int)));

	va.clear();
	vb.clear();

	//case 3c, should print true\n 7, 4, 5, 6
	va.push_back(7);
	va.push_back(1);
	va.push_back(4);
	va.push_back(5);
	va.push_back(12);
	va.push_back(13);
	va.push_back(6);

	int myint3[] = {7, 4, 5, 6};

	CPPUNIT_ASSERT(f.uses(&va,&vb,2) == true);
	CPPUNIT_ASSERT(va == vector<int>(myint3, myint3 + sizeof(myint3) / sizeof(int)));

	//case 3d, should print false\n no element
	va.clear();
	vb.clear();
	va.push_back(1);va.push_back(2);va.push_back(3);
	vb.push_back(3);vb.push_back(4);vb.push_back(5);

	CPPUNIT_ASSERT(f.uses(&va,&vb,2) == false);
	CPPUNIT_ASSERT(va == vector<int>());

	//case 3d, should print true\n 5, 6, 7
	vb.clear();
	va.push_back(1);va.push_back(2);va.push_back(3);va.push_back(5);va.push_back(6);va.push_back(8);va.push_back(9);va.push_back(7);
	vb.push_back(3);vb.push_back(4);vb.push_back(5);vb.push_back(6);
	int myint4[] = {5, 6, 7};

	CPPUNIT_ASSERT(f.uses(&va,&vb,2) == true);
	CPPUNIT_ASSERT(va == vector<int>(myint4, myint4 + sizeof(myint4) / sizeof(int)));
}

void UsesTest::f3() {
	Uses f;
	f.addUses(2,0);
	f.addUses(3,1);
	f.addUses(3,0);
	f.addUses(4,1);
	f.addUses(4,2);
	f.addUses(5,3);
	f.addUses(5,0);
	f.addUses(5,1);
	f.addUses(6,3);
	f.addUses(6,0);
	f.addUses(6,1);
	f.addUses(7,5);
	f.addUses(7,1);
	f.addUses(7,2);
	f.addUses(7,0);
	f.addUses(8,1);
	f.addUses(8,2);
	f.addUses(9,0);
	f.addUses(9,2);
	/* Uses table is
	* 2, 3, 3, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 7, 8, 8, 9, 9
	* 0, 1, 0, 1, 2, 3, 0, 1, 3, 0, 1, 5, 1, 2, 0, 1, 2, 0, 2
	*
	*/
	vector<int> va; vector<int> vb;

	//case 4a, should print true
	// 2, 3, 3, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 7, 8, 8, 9, 9
	// 0, 1, 0, 1, 2, 3, 0, 1, 3, 0, 1, 5, 1, 2, 0, 1, 2, 0, 2
	int myintA[] = {2, 3, 3, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 7, 8, 8, 9, 9};
	int myintB[] = {0, 1, 0, 1, 2, 3, 0, 1, 3, 0, 1, 5, 1, 2, 0, 1, 2, 0, 2};

	CPPUNIT_ASSERT(f.uses(&va,&vb,3) == true);
	CPPUNIT_ASSERT(va == vector<int>(myintA, myintA + sizeof(myintA) / sizeof(int)));
	CPPUNIT_ASSERT(vb == vector<int>(myintB, myintB + sizeof(myintB) / sizeof(int)));

	//case 4b, should print true
	//3, 4, 5, 6, 7, 8, 4, 7, 8, 9, 7
	//1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 5
	va.clear();
	vb.clear();
	vb.push_back(1);vb.push_back(7);vb.push_back(2);vb.push_back(4);vb.push_back(5);

	int myintA2[] = {3, 4, 5, 6, 7, 8, 4, 7, 8, 9, 7};
	int myintB2[] = {1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 5};

	CPPUNIT_ASSERT(f.uses(&va,&vb,3) == true);
	CPPUNIT_ASSERT(va == vector<int>(myintA2, myintA2 + sizeof(myintA2) / sizeof(int)));
	CPPUNIT_ASSERT(vb == vector<int>(myintB2, myintB2 + sizeof(myintB2) / sizeof(int)));

	va.clear();
	vb.clear();

	//case 4c, should print true
	//2, 4, 4, 6, 6, 6, 9, 9
	//0, 1, 2, 3, 0, 1, 0, 2
	va.push_back(1); va.push_back(2);va.push_back(4);va.push_back(6);va.push_back(10);va.push_back(9);
	int myintA3[] = {2, 4, 4, 6, 6, 6, 9, 9};
	int myintB3[] = {0, 1, 2, 3, 0, 1, 0, 2};

	CPPUNIT_ASSERT(f.uses(&va,&vb,3) == true);
	CPPUNIT_ASSERT(va == vector<int>(myintA3, myintA3 + sizeof(myintA3) / sizeof(int)));
	CPPUNIT_ASSERT(vb == vector<int>(myintB3, myintB3 + sizeof(myintB3) / sizeof(int)));

	//case 4d, should print false
	//no element
	//no element
	va.clear();
	vb.clear();
	vb.push_back(8);vb.push_back(12);vb.push_back(16);
	va.push_back(9);va.push_back(7);va.push_back(6);

	CPPUNIT_ASSERT(f.uses(&va,&vb,3) == false);
	CPPUNIT_ASSERT(va == vector<int>());
	CPPUNIT_ASSERT(vb == vector<int>());

	//case 4d, should print true
	//9 7 7
	//2 5 1
	va.push_back(9);va.push_back(8);va.push_back(15);va.push_back(7);va.push_back(7);
	vb.push_back(2);vb.push_back(7);vb.push_back(7);vb.push_back(5);vb.push_back(1);

	int myintA4[] = {9, 7, 7};
	int myintB4[] = {2, 5, 1};

	CPPUNIT_ASSERT(f.uses(&va,&vb,3) == true);
	CPPUNIT_ASSERT(va == vector<int>(myintA4, myintA4 + sizeof(myintA4) / sizeof(int)));
	CPPUNIT_ASSERT(vb == vector<int>(myintB4, myintB4 + sizeof(myintB4) / sizeof(int)));
}
